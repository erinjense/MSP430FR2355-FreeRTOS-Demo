
#include <Clock.h>

Clock::Clock()
{
    // Configure two FRAM waitstate as required by the device datasheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    /* TODO: take in external oscillator pins as constructor */
    /* Setup the external oscillator pins */
    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins

    // Initialize the crystal oscillator
    // Set the external crystal frequency (XT1)
    CS_setExternalClockSource(32768);
    // Turn on XT1 low-frequency crystal oscillator with the minimum drive strength.
    CS_turnOnXT1LF(CS_XT1_DRIVE_0);

    do
    {
        CS_clearFaultFlag(CS_XT1OFFG);
        CS_clearFaultFlag(CS_DCOFFG);
        SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    /* TODO: accept oscillator frequency from constructor */
    CS_initFLLSettle(24000, 731);  // Set MCLK to 24MHz with DCO multiplier 731 (N + 1)

    // Select the clock sources for MCLK, SMCLK, and ACLK

    // MCLK = DCOCLKDIV / 1
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);

    // SMCLK = DCOCLKDIV / 1
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);

    // ACLK = XT1CLK / 1
    CS_initClockSignal(CS_ACLK, CS_XT1CLK_SELECT, CS_CLOCK_DIVIDER_1);

    P3DIR |= BIT4;
    P3SEL0 |= BIT4;
    P3SEL1 &= ~BIT4;
}
