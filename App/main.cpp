
/* Standard includes. */

extern "C"
{
#include "msp430.h"
#include <driverlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <timers.h>
}

#include <stdio.h>
#include <led_template.h>
#include <Clock.h>
#include <DeviceSettingsManager.h>
#include <ostream>
#include <ios>
#include <streambuf>

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
static __attribute__((section(".device_settings"))) DeviceSettings deviceSettingsInstance;

/**
 *
 */
static DeviceSettingsManager settingsManager;



#include "UART.h"
static EUSCI_A_UART_initParam stdOutUartParams = {
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
static UART stdOutUart(stdOutUartParams);



extern "C" {
    #include <stdarg.h>
}


// UART write function
void uart_write_char(char ch) {
    stdOutUart.writeChar(ch);
}


/*-----------------------------------------------------------*/
/* Tasks                                                     */
/*-----------------------------------------------------------*/

namespace
{
    const BaseType_t LED1_BLINK_DELAY_MS = 1000;
}

void
vTaskLed1(void * pvParameters)
{
    const led_template<uint8_t, uint8_t> led1(GPIO_PORT_P1, GPIO_PIN0);

    for (;;)
    {
        led1.toggle();
        vTaskDelay(pdMS_TO_TICKS(LED1_BLINK_DELAY_MS));
    }
}

/*-----------------------------------------------------------*/
/* Application                                               */
/*-----------------------------------------------------------*/

void main( void )
{
	prvSetupHardware();

    xTaskCreate(vTaskLed1, "LED 1 Blink Task",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

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

    /* Configure LED2 */
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);

    /* Unlock the GPIO power-on default high-impedance mode */
    PMM_unlockLPM5();

    taskENABLE_INTERRUPTS();
}
