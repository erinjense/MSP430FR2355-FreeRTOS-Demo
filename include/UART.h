#ifndef UART_H
#define UART_H



extern "C"
{
#include <msp430.h>
#include <driverlib.h>
#include "FreeRTOS.h"
#include "semphr.h"
}

class UART {
public:
    UART(EUSCI_A_UART_initParam &uartParams);
    void writeChar(char c);
    void writeString(const char *str);

private:
    static const uint32_t UART_QUEUE_LENGTH = 64;
    static const uint32_t UART_ITEM_SIZE = sizeof(char);

    void configureUART();
    static void uartTask(void * pvParameters);
    StaticSemaphore_t xMutexBuffer;
    SemaphoreHandle_t uartMutex;
    StaticQueue_t xQueueBuffer;
    uint8_t ucQueueStorage[UART_QUEUE_LENGTH];
    QueueHandle_t uartQueue;
    static void uartTaskWrapper(void * pvParameters);
};

#endif // UART_H
