#ifndef UART_H
#define UART_H

extern "C" {
    #include <msp430.h>
    #include "FreeRTOS.h"
    #include "semphr.h"
    #include <driverlib.h>
}

#define UART_QUEUE_LENGTH 880
#define UART_ITEM_SIZE sizeof(char)

class UART {
public:
    UART();
    void init();
    void writeChar(char c);
    void writeString(const char *str);

private:
    void configureUART();
    void uartTask();
    StaticSemaphore_t xMutexBuffer;
    SemaphoreHandle_t uartMutex;
    StaticQueue_t xQueueBuffer;
    uint8_t ucQueueStorage[UART_QUEUE_LENGTH];
    QueueHandle_t uartQueue;

    static void uartTaskWrapper(void * pvParameters);
};

#endif // UART_H
