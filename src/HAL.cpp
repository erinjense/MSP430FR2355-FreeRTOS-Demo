
#include "UART.h"
#include <cstdio>

/*-----------------------------------------------------------*/
/* Standard I/O                                              */
/*-----------------------------------------------------------*/

#if 0
/**
 *
 */
static EUSCI_A_UART_initParam stdOutUartParams = {
                                .selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
                                .clockPrescalar = 13,
                                .firstModReg = 0,
                                .secondModReg = 37,
                                .parity = EUSCI_A_UART_NO_PARITY,
                                .msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
                                .numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
                                .uartMode = EUSCI_A_UART_MODE,
                                .overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
};

/**
 *
 */
static UART stdOutUart(stdOutUartParams);

/**
 *
 */
extern "C" int __io__putchar(int ch)
{
    stdOutUart.writeChar(static_cast<char>(ch));
    return (ch);
}

/**
 *
 */
extern "C" int _write(int file, char const * const data, int len)
{
    stdOutUart.writeString(data);
    return (len);
}
#endif
