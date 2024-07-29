/* Standard includes. */
#include <App/MSP430FR2xx_4xx/driverlib.h>
#include <FreeRTOS/include/FreeRTOS.h>
#include <FreeRTOS/include/queue.h>
#include <FreeRTOS/include/task.h>
#include <FreeRTOS/include/timers.h>
#include <stdio.h>

/* FreeRTOS includes. */
#include "msp430.h"


/* The length of the queue (the number of items the queue can hold) that is used
to send messages from tasks and interrupts the the LCD task. */
#define mainQUEUE_LENGTH				( 5 )

/* Just used to ensure parameters are passed into tasks correctly. */
#define mainTASK_PARAMETER_CHECK_VALUE	( ( void * ) 0xDEAD )

/* The base period used by the timer test tasks. */
#define mainTIMER_TEST_PERIOD			( 50 )

/* The frequency at which the check timer (described in the comments at the top
of this file) will call its callback function. */
#define mainCHECK_TIMER_PERIOD			( 5000UL / ( unsigned long ) portTICK_PERIOD_MS )

/* Misc. */
#define mainDONT_BLOCK					( 0 )
/*-----------------------------------------------------------*/

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

/* The definition of each message sent from tasks and interrupts to the LCD
task. */
typedef struct
{
	char cMessageID;				/* << States what the message is. */
	unsigned long ulMessageValue; 	/* << States the message value (can be an integer, string pointer, etc. depending on the value of cMessageID). */
} xQueueMessage;

/*-----------------------------------------------------------*/

/**
 *
 */
void vBlinkLEDTask(void *pvParameters);


void main( void )
{
	prvSetupHardware();

    // Create the LED blink task
    xTaskCreate(vBlinkLEDTask, "BlinkLED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();

	/* If all is well then this line will never be reached.  If it is reached
	then it is likely that there was insufficient (FreeRTOS) heap memory space
	to create the idle task.  This may have been trapped by the malloc() failed
	hook function, if one is configured. */	
	for( ;; );
}

/*-----------------------------------------------------------*/


static void prvSetupHardware( void )
{
	taskDISABLE_INTERRUPTS();
	
	/* Disable the watchdog. */
	WDTCTL = WDTPW + WDTHOLD;
  
	/* Tie unused ports */
	PAOUT  = 0;
	PADIR  = 0xFFFF;
	PBOUT  = 0;
	PBDIR  = 0xFFFF;
	PCOUT  = 0;
	PCDIR  = 0xFFFF;
	PJOUT  = 0;
	PJDIR  = 0xFF;

    /* Unlock the GPIO power-on default high-impedance mode */
    PMM_unlockLPM5();

    /* Configure LED1 */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configure LED2 */
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6); // Ensure the LED is off initially


    taskENABLE_INTERRUPTS();
}

/*-----------------------------------------------------------*/
/* Tasks                                                     */
/*-----------------------------------------------------------*/

void vBlinkLEDTask(void *pvParameters)
{
    const TickType_t xDelay = pdMS_TO_TICKS(500); // 500 ms delay

    for(;;)
    {
        // Toggle LED1
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

        // Delay for a period
        vTaskDelay(xDelay);
    }
}

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

/*-----------------------------------------------------------*/

