
#ifndef INCLUDE_BLINKER_H_
#define INCLUDE_BLINKER_H_

extern "C" {
#include <driverlib.h>
#include <FreeRTOS.h>
#include <task.h>
}

template<typename port_type,
         typename pin_type,
         typename tick_type,
         const port_type port,
         const pin_type pin,
         const tick_type delay>
class blinker_template
{
public:
    blinker_template()
    {
        GPIO_setAsOutputPin(port, pin);
        GPIO_setOutputLowOnPin(port, pin);
        xTaskCreate(blinkTask, "Blink Task",
                    configMINIMAL_STACK_SIZE, this, tskIDLE_PRIORITY, NULL);
    }

    static void toggle()
    {
        GPIO_toggleOutputOnPin(port, pin);
    }

    static void blinkTask(void *pvParameters)
    {
        blinker_template *blinker = static_cast<blinker_template *>(pvParameters);

        for (;;)
        {
            blinker->toggle();
            vTaskDelay(delay);
        }
    }
};

#endif /* INCLUDE_BLINKER_H_ */
