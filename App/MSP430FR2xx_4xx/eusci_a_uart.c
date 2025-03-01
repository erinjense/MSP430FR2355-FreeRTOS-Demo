//*****************************************************************************
//
// eusci_a_uart.c - Driver for the eusci_a_uart Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup eusci_a_uart_api eusci_a_uart
//! @{
//
//*****************************************************************************

#include <App/MSP430FR2xx_4xx/inc/hw_memmap.h>

#ifdef __MSP430_HAS_EUSCI_Ax__
#include <App/MSP430FR2xx_4xx/eusci_a_uart.h>

#include <assert.h>

bool EUSCI_A_UART_init(uint16_t baseAddress, EUSCI_A_UART_initParam *param)
{
    bool retVal = STATUS_SUCCESS;

    //Disable the USCI Module
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;

    //Clock source select
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCSSEL_3;
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= (uint16_t)param->selectClockSource;

    //MSB, LSB select
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCMSB;
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= param->msborLsbFirst;

    //UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCSPB;
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= param->numberofStopBits;

    //Parity
    switch (param->parity){
        case EUSCI_A_UART_NO_PARITY:
            //No Parity
            HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCPEN;
            break;
        case EUSCI_A_UART_ODD_PARITY:
            //Odd Parity
            HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCPEN;
            HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCPAR;
            break;
        case EUSCI_A_UART_EVEN_PARITY:
            //Even Parity
            HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCPEN;
            HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCPAR;
            break;
    }

    //BaudRate Control Register
    HWREG16(baseAddress + OFS_UCAxBRW ) = param->clockPrescalar;
    //Modulation Control Register
    HWREG16(baseAddress + OFS_UCAxMCTLW) = ((param->secondModReg <<8)
        + (param->firstModReg <<4) + param->overSampling );

    //Asynchronous mode & 8 bit character select & clear mode
    HWREG16(baseAddress + OFS_UCAxCTLW0) &=  ~(UCSYNC +
                                             UC7BIT +
                                             UCMODE_3
                                             );

    //Configure  UART mode.
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= param->uartMode ;

    //Reset UCRXIE, UCBRKIE, UCDORM, UCTXADDR, UCTXBRK
    HWREG16(baseAddress + OFS_UCAxCTLW0)  &= ~(UCRXEIE + UCBRKIE + UCDORM +
                                             UCTXADDR + UCTXBRK
                                             );
    return (retVal);
}

void EUSCI_A_UART_transmitData ( uint16_t baseAddress,
    uint8_t transmitData
    )
{
    //If interrupts are not used, poll for flags
    if (!(HWREG16(baseAddress + OFS_UCAxIE) & UCTXIE)){
        //Poll for transmit interrupt flag
        while (!(HWREG16(baseAddress + OFS_UCAxIFG) & UCTXIFG));
    }

    HWREG16(baseAddress + OFS_UCAxTXBUF) = transmitData;
}

uint8_t EUSCI_A_UART_receiveData (uint16_t baseAddress)
{
    //If interrupts are not used, poll for flags
    if (!(HWREG16(baseAddress + OFS_UCAxIE) & UCRXIE)){
        //Poll for receive interrupt flag
        while (!(HWREG16(baseAddress + OFS_UCAxIFG) & UCRXIFG));
    }

    return ( HWREG16(baseAddress + OFS_UCAxRXBUF)) ;
}

void EUSCI_A_UART_enableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    uint8_t locMask;

    locMask = (mask & (EUSCI_A_UART_RECEIVE_INTERRUPT
        | EUSCI_A_UART_TRANSMIT_INTERRUPT
        | EUSCI_A_UART_STARTBIT_INTERRUPT
        | EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT));

    HWREG16(baseAddress + OFS_UCAxIE) |= (uint16_t)locMask;

    locMask = (mask & (EUSCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT
        | EUSCI_A_UART_BREAKCHAR_INTERRUPT));
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= (uint16_t)locMask;
}

void EUSCI_A_UART_disableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    uint8_t locMask;

    locMask = (mask & (EUSCI_A_UART_RECEIVE_INTERRUPT
        | EUSCI_A_UART_TRANSMIT_INTERRUPT
        | EUSCI_A_UART_STARTBIT_INTERRUPT
        | EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT));
    HWREG16(baseAddress + OFS_UCAxIE) &= (uint16_t)~locMask;

    locMask = (mask & (EUSCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT
        | EUSCI_A_UART_BREAKCHAR_INTERRUPT));
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= (uint16_t)~locMask;
}

uint8_t EUSCI_A_UART_getInterruptStatus (uint16_t baseAddress,
    uint8_t mask)
{
    return ( HWREG16(baseAddress + OFS_UCAxIFG) & mask );
}

void EUSCI_A_UART_clearInterrupt (uint16_t baseAddress, uint16_t mask)
{
    //Clear the UART interrupt source.
    HWREG16(baseAddress + OFS_UCAxIFG) &= ~(mask);
}

void EUSCI_A_UART_enable (uint16_t baseAddress)
{
    //Reset the UCSWRST bit to enable the USCI Module
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCSWRST);
}

void EUSCI_A_UART_disable (uint16_t baseAddress)
{
    //Set the UCSWRST bit to disable the USCI Module
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;
}

uint8_t EUSCI_A_UART_queryStatusFlags (uint16_t baseAddress,
    uint8_t mask)
{
    return ( HWREG16(baseAddress + OFS_UCAxSTATW) & mask );
}

void EUSCI_A_UART_setDormant (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCDORM;
}

void EUSCI_A_UART_resetDormant (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCDORM;
}

void EUSCI_A_UART_transmitAddress (uint16_t baseAddress,
    uint8_t transmitAddress)
{
    //Set UCTXADDR bit
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCTXADDR;

    //Place next byte to be sent into the transmit buffer
    HWREG16(baseAddress + OFS_UCAxTXBUF) = transmitAddress;
}

void EUSCI_A_UART_transmitBreak (uint16_t baseAddress)
{
    //Set UCTXADDR bit
    HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCTXBRK;

    //If current mode is automatic baud-rate detection
    if (EUSCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE ==
        (HWREG16(baseAddress + OFS_UCAxCTLW0) &
         EUSCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE)){
        HWREG16(baseAddress + OFS_UCAxTXBUF) = EUSCI_A_UART_AUTOMATICBAUDRATE_SYNC;
    } else   {
        HWREG16(baseAddress + OFS_UCAxTXBUF) = DEFAULT_SYNC;
    }

    //If interrupts are not used, poll for flags
    if (!(HWREG16(baseAddress + OFS_UCAxIE) & UCTXIE)){
        //Poll for transmit interrupt flag
        while (!(HWREG16(baseAddress + OFS_UCAxIFG) & UCTXIFG));
    }
}

uint32_t EUSCI_A_UART_getReceiveBufferAddress (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxRXBUF );
}

uint32_t EUSCI_A_UART_getTransmitBufferAddress (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxTXBUF );
}

void EUSCI_A_UART_selectDeglitchTime(uint16_t baseAddress,
            uint16_t deglitchTime
            )
{
    HWREG16(baseAddress + OFS_UCAxCTLW1) &= ~(UCGLIT1 + UCGLIT0);

    HWREG16(baseAddress + OFS_UCAxCTLW1) |= deglitchTime;
}

void EUSCI_A_UART_remapPins (uint16_t baseAddress, uint8_t pinsSelect)
{
#ifdef USCIARMP
    HWREG16(SYS_BASE + OFS_SYSCFG3) &= ~USCIARMP;
    HWREG16(SYS_BASE + OFS_SYSCFG3) |= pinsSelect;
#elif defined(USCIA0RMP)
    HWREG16(SYS_BASE + OFS_SYSCFG3) &= ~USCIA0RMP;
    HWREG16(SYS_BASE + OFS_SYSCFG3) |= pinsSelect;
#endif
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for eusci_a_uart_api
//! @}
//
//*****************************************************************************
