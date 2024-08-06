#ifndef INCLUDE_LED_TEMPLATE_H_
#define INCLUDE_LED_TEMPLATE_H_

/*-----------------------------------------------------------*/
/* Includes                                                  */
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
/* LED Template Class                                        */
/*-----------------------------------------------------------*/

/*
 * led_template - A template class for controlling an LED.
 *
 * This class provides an interface for initializing and controlling an LED
 * connected to a specified GPIO port and pin. The class uses template parameters
 * to allow flexibility in specifying the port and pin types.
 *
 * Template Parameters:
 *   port_type - The type representing the GPIO port (e.g., uint8_t).
 *   pin_type  - The type representing the GPIO pin (e.g., uint8_t).
 *
 * Member Functions:
 *   led_template(port_type port, pin_type pin)
 *     Constructor that initializes the specified port and pin as an output and
 *     sets the pin to low.
 *
 *   void toggle() const
 *     Toggles the state of the LED.
 *
 *   void on() const
 *     Turns the LED on (sets the pin to high).
 *
 *   void off() const
 *     Turns the LED off (sets the pin to low).
 *
 * Member Variables:
 *   port_type port - The GPIO port to which the LED is connected.
 *   pin_type pin  - The GPIO pin to which the LED is connected.
 */

template<typename port_type, typename pin_type>
class led_template
{
public:
    led_template(port_type port, pin_type pin) : port(port), pin(pin)
    {
        GPIO_setAsOutputPin(port, pin);
        GPIO_setOutputLowOnPin(port, pin);
    }

    void toggle() const
    {
        GPIO_toggleOutputOnPin(port, pin);
    }

    void on() const
    {
        GPIO_setOutputHighOnPin(port, pin);
    }

    void off() const
    {
        GPIO_setOutputLowOnPin(port, pin);
    }

private:
    port_type port;
    pin_type pin;
};

#endif /* INCLUDE_LED_TEMPLATE_H_ */
