
#ifndef INCLUDE_BLINKER_H_
#define INCLUDE_BLINKER_H_

extern "C" {
#include <driverlib.h>
#include <FreeRTOS.h>
#include <task.h>
}

class Blinker
{
public:
    Blinker(uint8_t port, uint8_t pin, TickType_t delay);
    static void blinkTask(void *pvParameters);

private:
    uint8_t port;
    uint8_t pin;
    TickType_t delay;
};

#endif /* INCLUDE_BLINKER_H_ */
