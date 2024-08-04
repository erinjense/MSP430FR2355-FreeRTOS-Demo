/* Standard includes. */

extern "C"
{
#include <driverlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <timers.h>
#include "msp430.h"
}

#include <ostream>
#include <stdio.h>
#include <Blinker.h>
#include <Clock.h>
#include <UART.h>
#include <DeviceSettingsManager.h>
#include <sstream>
/*
 * Configures hardware peripherals
 */
static void prvSetupHardware( void );

/*-----------------------------------------------------------*/

/* Variables that are incremented on each iteration of the reg test tasks -
provided the tasks have not reported any errors.  The check task inspects these
variables to ensure they are still incrementing as expected.  If a variable
stops incrementing then it is likely that its associate task has stalled. */
volatile unsigned short usRegTest1Counter = 0, usRegTest2Counter = 0;

/*-----------------------------------------------------------*/

void vBlinkLEDTask(void *pvParameters);
void vTaskFunction(void *pvParameters);
void
vTaskFunction2(void *pvParameters);

/*-----------------------------------------------------------*/
/* Application                                               */
/*-----------------------------------------------------------*/

// Task and stack buffers for static allocation
static StaticTask_t xPrintTaskBuffer;
static StackType_t xPrintStack[configMINIMAL_STACK_SIZE];

static StaticTask_t xCollisionTaskBuffer;
static StackType_t xCollisionStack[configMINIMAL_STACK_SIZE];

/**
 *
 */
EUSCI_A_UART_initParam terminalUartParams = {
                                .selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
                                .clockPrescalar = 13,
                                .firstModReg = 0,
                                .secondModReg = 37,
                                .parity = EUSCI_A_UART_NO_PARITY,
                                .msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
                                .numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
                                .uartMode = EUSCI_A_UART_MODE,
                                .overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
};

/**
 *
 */
static UART uart(terminalUartParams);

__attribute__((section(".device_settings"))) DeviceSettings deviceSettingsInstance;

DeviceSettingsManager settingsManager;

namespace
{
    const blinker_template<uint8_t, uint8_t, TickType_t,
                           GPIO_PORT_P1, GPIO_PIN0, pdMS_TO_TICKS(1000)>
    led1_blinker;
}

void main( void )
{
	prvSetupHardware();

    vTaskStartScheduler();

	for( ;; );
}


/*-----------------------------------------------------------*/
/* Setup                                                     */
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	taskDISABLE_INTERRUPTS();
	
	/* Disable the watchdog. */
    WDT_A_hold(WDT_A_BASE);
  
	/* Tie unused ports */
	PAOUT  = 0;
	PADIR  = 0xFFFF;
	PBOUT  = 0;
	PBDIR  = 0xFFFF;
	PCOUT  = 0;
	PCDIR  = 0xFFFF;
	PJOUT  = 0;
	PJDIR  = 0xFF;

	static Clock clock;

    /* Configure LED1 */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configure LED2 */
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);

    /* Unlock the GPIO power-on default high-impedance mode */
    PMM_unlockLPM5();

    taskENABLE_INTERRUPTS();
}

/*-----------------------------------------------------------*/
/* Tasks                                                     */
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
/* Interrupt                                                   */
/*-----------------------------------------------------------*/
void vApplicationSetupTimerInterrupt( void )
{
    const unsigned short usACLK_Frequency_Hz = 32768;

    /* Stop the timer */
    Timer_B_stop(TIMER_B0_BASE);

    /* Configure Timer B to use ACLK, clear the timer, enable interrupt, and set it to up mode */
    Timer_B_initUpModeParam timerInitParams = {
        .clockSource = TIMER_B_CLOCKSOURCE_ACLK,
        .clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1,
        .timerPeriod = usACLK_Frequency_Hz / configTICK_RATE_HZ,
        .timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE,
        .captureCompareInterruptEnable_CCR0_CCIE = TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE,
        .timerClear = TIMER_B_DO_CLEAR,
        .startTimer = false
    };

    /* Initialize the timer in up mode */
    Timer_B_initUpMode(TIMER_B0_BASE, &timerInitParams);

    /* Start the timer in up mode */
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_UP_MODE);
}

/*-----------------------------------------------------------*/
/* Hooks                                                     */
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    static unsigned long ulCounter = 0;

    /* Is it time to toggle the LED again? */
    ulCounter++;

    /* Just periodically toggle an LED to show that the tick interrupt is
    running.  Note that this access LED_PORT_OUT in a non-atomic way, so tasks
    that access the same port must do so from a critical section. */
    if( ( ulCounter & 0xff ) == 0 )
    {
        /* Blink LED 2 */
        GPIO_toggleOutputOnPin(GPIO_PORT_P6, GPIO_PIN6);
    }
}

void vApplicationIdleHook( void )
{
	/* Called on each iteration of the idle task.  In this case the idle task
	just enters a low(ish) power mode. */
	__bis_SR_register( LPM1_bits + GIE );
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues or
	semaphores. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pxTask;
	( void ) pcTaskName;
	
	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE * puxIdleTaskStackSize )
{
    // Static allocation of the Idle Task
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

