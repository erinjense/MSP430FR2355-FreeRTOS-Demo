#include "UART.h"
#include <string.h>

UART::UART()
{
    uartMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    if (uartMutex == NULL)
    {
        for (;;);
    }

    uartQueue = xQueueCreateStatic(UART_QUEUE_LENGTH,
                                   UART_ITEM_SIZE,
                                   ucQueueStorage, &xQueueBuffer);
    init();
}

void UART::init() {
    configureUART();
    xTaskCreate(uartTaskWrapper, "UART Task",
                configMINIMAL_STACK_SIZE, this, tskIDLE_PRIORITY + 2, NULL);
}

void UART::configureUART() {
    // Configure UCA1TXD and UCA1RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configure UART
    // ClockSource = SMCLK = 24MHz, Baudrate = 115200bps
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 13;
    param.firstModReg = 0;
    param.secondModReg = 37;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, &param)) {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
}

void UART::writeChar(char c)
{
    if (xQueueSend(uartQueue, &c, portMAX_DELAY) != pdPASS)
    {
        // Handle error if queue send fails
    }
}

void UART::writeString(const char *str) {
    while (*str)
    {
       writeChar(*str++);
    }
    writeChar('\r');
    writeChar('\n');
}

void UART::uartTask()
{
    char c;
    while (1)
    {
        if (xQueueReceive(uartQueue, &c, portMAX_DELAY) == pdPASS)
        {
            while (EUSCI_A_UART_queryStatusFlags(EUSCI_A1_BASE, EUSCI_A_UART_BUSY));
            EUSCI_A_UART_transmitData(EUSCI_A1_BASE, c);
        }
    }
}

void UART::uartTaskWrapper(void *pvParameters)
{
    UART *uart = static_cast<UART * >(pvParameters);
    uart->uartTask();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Clear P2.3 IFG
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN3);
    __bic_SR_register_on_exit(LPM3_bits);
}
