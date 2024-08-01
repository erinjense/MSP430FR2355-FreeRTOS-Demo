
#include <Blinker.h>

Blinker::Blinker(uint8_t port, uint8_t pin, TickType_t delay)
               : port(port), pin(pin), delay(delay)
{
    GPIO_setAsOutputPin(port, pin);
    GPIO_setOutputLowOnPin(port, pin);
}

void Blinker::blinkTask(void *pvParameters)
{
    Blinker *blinker = static_cast<Blinker *>(pvParameters);

    for (;;)
    {
        GPIO_toggleOutputOnPin(blinker->port, blinker->pin);
        vTaskDelay(blinker->delay);
    }
}
