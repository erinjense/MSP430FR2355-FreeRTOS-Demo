
extern "C"
{
#include <driverlib.h>
#include <FreeRTOS.h>
#include <task.h>
}

/*-----------------------------------------------------------*/
/* Hooks                                                     */
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
