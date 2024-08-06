#include "UART.h"

UART::UART(EUSCI_A_UART_initParam &uartParams)
{
    uartMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    if (uartMutex == NULL)
    {
        for (;;);
    }

    uartQueue = xQueueCreateStatic(UART_QUEUE_LENGTH,
                                   UART_ITEM_SIZE,
                                   ucQueueStorage, &xQueueBuffer);
    xTaskCreate(uartTask, "UART Task",
                configMINIMAL_STACK_SIZE, this, tskIDLE_PRIORITY + 2, NULL);

    // Configure UCA1TXD and UCA1RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, &uartParams)) {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);
}

void UART::writeChar(char c)
{
    if (xQueueSend(uartQueue, &c, portMAX_DELAY) != pdPASS)
    {
        // Handle error if queue send fails
    }
}

void UART::writeString(const std::string &s) {
    std::string::const_iterator it = s.begin();

    while (it != s.end())
    {
       writeChar(*it++);
    }
    writeChar('\r');
    writeChar('\n');
}

void UART::uartTask(void * pvParameters)
{
    UART *uart = static_cast<UART *>(pvParameters);
    char c;
    while (1)
    {
        if (xQueueReceive(uart->uartQueue, &c, portMAX_DELAY) == pdPASS)
        {
            while (EUSCI_A_UART_queryStatusFlags(EUSCI_A1_BASE, EUSCI_A_UART_BUSY));
            EUSCI_A_UART_transmitData(EUSCI_A1_BASE, c);
        }
    }
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


// UART interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            // Echo received character
            while (!(UCA1IFG & UCTXIFG)); // Wait until ready
            EUSCI_A_UART_transmitData(EUSCI_A1_BASE, EUSCI_A_UART_receiveData(EUSCI_A1_BASE));
            break;
        case USCI_UART_UCTXIFG: break;
        default: break;
    }
}
