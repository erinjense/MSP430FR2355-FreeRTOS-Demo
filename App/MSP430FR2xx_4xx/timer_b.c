//*****************************************************************************
//
// timer_b.c - Driver for the timer_b Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup timer_b_api timer_b
//! @{
//
//*****************************************************************************

#include <App/MSP430FR2xx_4xx/inc/hw_memmap.h>

#ifdef __MSP430_HAS_TxB7__
#include <App/MSP430FR2xx_4xx/timer_b.h>

#include <assert.h>

void Timer_B_startCounter ( uint16_t baseAddress,
    uint16_t timerMode
    )
{
    HWREG16(baseAddress + OFS_TBxCTL) |= timerMode;
}

void Timer_B_initContinuousMode(uint16_t baseAddress,
    Timer_B_initContinuousModeParam *param)
{

    HWREG16(baseAddress +
        OFS_TBxCTL) &= ~(TIMER_B_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
                         TIMER_B_UPDOWN_MODE +
                         TIMER_B_DO_CLEAR +
                         TIMER_B_TBIE_INTERRUPT_ENABLE +
                         CNTL_3 +
                         ID__8
                         );
    HWREG16(baseAddress + OFS_TBxEX0) &= ~TBIDEX_7;

    HWREG16(baseAddress + OFS_TBxEX0) |= param->clockSourceDivider&0x7;

    HWREG16(baseAddress + OFS_TBxCTL) |= (param->clockSource +
                                          param->timerClear +
                                          param->timerInterruptEnable_TBIE +
                                          ((param->clockSourceDivider>>3)<<6));

    if(param->startTimer) {
        HWREG16(baseAddress + OFS_TBxCTL) |= TIMER_B_CONTINUOUS_MODE;
    }
}

void Timer_B_initUpMode (uint16_t baseAddress,
    Timer_B_initUpModeParam *param)
{

    HWREG16(baseAddress + OFS_TBxCTL) &=
        ~(TIMER_B_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
          TIMER_B_UPDOWN_MODE +
          TIMER_B_DO_CLEAR +
          TIMER_B_TBIE_INTERRUPT_ENABLE +
          CNTL_3
          );
    HWREG16(baseAddress + OFS_TBxEX0) &= ~TBIDEX_7;

    HWREG16(baseAddress + OFS_TBxEX0) |= param->clockSourceDivider&0x7;

    HWREG16(baseAddress + OFS_TBxCTL) |= (param->clockSource +
                                          param->timerClear +
                                          param->timerInterruptEnable_TBIE +
                                          ((param->clockSourceDivider>>3)<<6));

    if (param->startTimer) {
        HWREG16(baseAddress + OFS_TBxCTL) |= TIMER_B_UP_MODE;
    }

    if (TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE ==
        param->captureCompareInterruptEnable_CCR0_CCIE){
        HWREG16(baseAddress + OFS_TBxCCTL0)  |= TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    } else   {
        HWREG16(baseAddress + OFS_TBxCCTL0)  &= ~TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    }

    HWREG16(baseAddress + OFS_TBxCCR0)  = param->timerPeriod;
}

void Timer_B_initUpDownMode(uint16_t baseAddress,
    Timer_B_initUpDownModeParam *param)
{
    HWREG16(baseAddress + OFS_TBxCTL) &=
        ~(TIMER_B_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
          TIMER_B_UPDOWN_MODE +
          TIMER_B_DO_CLEAR +
          TIMER_B_TBIE_INTERRUPT_ENABLE +
          CNTL_3
          );
    HWREG16(baseAddress + OFS_TBxEX0) &= ~TBIDEX_7;

    HWREG16(baseAddress + OFS_TBxEX0) |= param->clockSourceDivider&0x7;

    HWREG16(baseAddress + OFS_TBxCTL) |= (param->clockSource +
                                          TIMER_B_STOP_MODE +
                                          param->timerClear +
                                          param->timerInterruptEnable_TBIE +
                                          ((param->clockSourceDivider>>3)<<6));

    if (param->startTimer) {
        HWREG16(baseAddress + OFS_TBxCTL) |= TIMER_B_UPDOWN_MODE;
    }

    if (TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE ==
        param->captureCompareInterruptEnable_CCR0_CCIE){
        HWREG16(baseAddress + OFS_TBxCCTL0)  |= TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    } else   {
        HWREG16(baseAddress + OFS_TBxCCTL0)  &= ~TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    }

    HWREG16(baseAddress + OFS_TBxCCR0)  = param->timerPeriod;
}

void Timer_B_initCaptureMode(uint16_t baseAddress,
    Timer_B_initCaptureModeParam *param)
{

    HWREG16(baseAddress + param->captureRegister ) |= CAP;

    HWREG16(baseAddress + param->captureRegister) &=
        ~(TIMER_B_CAPTUREMODE_RISING_AND_FALLING_EDGE +
          TIMER_B_CAPTURE_INPUTSELECT_Vcc +
          TIMER_B_CAPTURE_SYNCHRONOUS +
          TIMER_B_DO_CLEAR +
          TIMER_B_TBIE_INTERRUPT_ENABLE +
          CM_3
          );

    HWREG16(baseAddress + param->captureRegister) |= (param->captureMode +
                                              param->captureInputSelect +
                                              param->synchronizeCaptureSource +
                                              param->captureInterruptEnable +
                                              param->captureOutputMode
                                              );
}

void Timer_B_initCompareMode(uint16_t baseAddress,
    Timer_B_initCompareModeParam *param)
{

    HWREG16(baseAddress + param->compareRegister ) &= ~CAP;

    HWREG16(baseAddress + param->compareRegister) &=
        ~(TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE +
          TIMER_B_OUTPUTMODE_RESET_SET
          );

    HWREG16(baseAddress + param->compareRegister) |= (param->compareInterruptEnable +
                                               param->compareOutputMode
                                               );

    HWREG16(baseAddress + param->compareRegister + OFS_TBxR) = param->compareValue;
}

void Timer_B_enableInterrupt (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TBxCTL) |= TBIE;
}

void Timer_B_disableInterrupt (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TBxCTL) &= ~TBIE;
}

uint32_t Timer_B_getInterruptStatus (uint16_t baseAddress)
{
    return ( HWREG16(baseAddress + OFS_TBxCTL) & TBIFG );
}

void Timer_B_enableCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG16(baseAddress + captureCompareRegister) |= CCIE;
}

void Timer_B_disableCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG16(baseAddress + captureCompareRegister) &= ~CCIE;
}

uint32_t Timer_B_getCaptureCompareInterruptStatus (uint16_t baseAddress,
		 uint16_t captureCompareRegister,
		 uint16_t mask
		 )
{
    return ( HWREG16(baseAddress + captureCompareRegister) & mask );
}

void Timer_B_clear (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TBxCTL) |= TBCLR;
}

uint8_t Timer_B_getSynchronizedCaptureCompareInput
    (uint16_t baseAddress,
    uint16_t captureCompareRegister,
    uint16_t synchronized
    )
{
    if (HWREG16(baseAddress + captureCompareRegister) & synchronized){
        return ( TIMER_B_CAPTURECOMPARE_INPUT_HIGH) ;
    } else   {
        return ( TIMER_B_CAPTURECOMPARE_INPUT_LOW) ;
    }
}

uint8_t Timer_B_getOutputForOutputModeOutBitValue
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{

    if (HWREG16(baseAddress + captureCompareRegister) & OUT){
        return ( TIMER_B_OUTPUTMODE_OUTBITVALUE_HIGH) ;
    } else   {
        return ( TIMER_B_OUTPUTMODE_OUTBITVALUE_LOW) ;
    }
}

uint16_t Timer_B_getCaptureCompareCount
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    return  (HWREG16(baseAddress + OFS_TBxR + captureCompareRegister));
}

void Timer_B_setOutputForOutputModeOutBitValue
    (uint16_t baseAddress,
    uint16_t captureCompareRegister,
    uint16_t outputModeOutBitValue
    )
{
    HWREG16(baseAddress + captureCompareRegister) &= ~OUT;
    HWREG16(baseAddress + captureCompareRegister) |= outputModeOutBitValue;
}

void Timer_B_outputPWM(uint16_t baseAddress, Timer_B_outputPWMParam *param)
{

    HWREG16(baseAddress + OFS_TBxCTL)  &=
        ~( TIMER_B_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
           TIMER_B_UPDOWN_MODE + TIMER_B_DO_CLEAR +
           TIMER_B_TBIE_INTERRUPT_ENABLE
           );
    HWREG16(baseAddress + OFS_TBxEX0) &= ~TBIDEX_7;

    HWREG16(baseAddress + OFS_TBxEX0) |= param->clockSourceDivider&0x7;

    HWREG16(baseAddress + OFS_TBxCTL) |= (param->clockSource +
                                          TIMER_B_UP_MODE +
                                          TIMER_B_DO_CLEAR +
                                          ((param->clockSourceDivider>>3)<<6));

    HWREG16(baseAddress + OFS_TBxCCR0) = param->timerPeriod;

    HWREG16(baseAddress + OFS_TBxCCTL0)  &=
        ~(TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE +
          TIMER_B_OUTPUTMODE_RESET_SET
          );

    HWREG16(baseAddress + param->compareRegister)  |= param->compareOutputMode;

    HWREG16(baseAddress + param->compareRegister + OFS_TBxR) = param->dutyCycle;
}

void Timer_B_stop ( uint16_t baseAddress )
{
    HWREG16(baseAddress + OFS_TBxCTL)  &= ~MC_3;
}


void Timer_B_setCompareValue (  uint16_t baseAddress,
    uint16_t compareRegister,
    uint16_t compareValue
    )
{
    HWREG16(baseAddress + compareRegister + OFS_TBxR) = compareValue;
}

void Timer_B_clearTimerInterrupt (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TBxCTL) &= ~TBIFG;
}

void Timer_B_clearCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG16(baseAddress + captureCompareRegister)  &= ~CCIFG;
}

void Timer_B_selectCounterLength (uint16_t  baseAddress,
		uint16_t counterLength
		)
{
	HWREG16(baseAddress + OFS_TBxCTL) &= ~CNTL_3;
	HWREG16(baseAddress + OFS_TBxCTL) |= counterLength;
}

void Timer_B_selectLatchingGroup(uint16_t  baseAddress,
		uint16_t  groupLatch)
{
	HWREG16(baseAddress + OFS_TBxCTL) &= ~TBCLGRP_3;
	HWREG16(baseAddress + OFS_TBxCTL) |= groupLatch;
}

void Timer_B_initCompareLatchLoadEvent(uint16_t  baseAddress,
		uint16_t  compareRegister,
		uint16_t  compareLatchLoadEvent
		)
{
	HWREG16(baseAddress + compareRegister)  &= ~CLLD_3;
	HWREG16(baseAddress + compareRegister)  |= compareLatchLoadEvent;
}

uint16_t Timer_B_getCounterValue (uint16_t baseAddress)
{
	uint16_t voteOne, voteTwo, res;

    voteTwo = HWREG16(baseAddress + OFS_TBxR);

	do
    {
        voteOne = voteTwo;
        voteTwo = HWREG16(baseAddress + OFS_TBxR);

		if(voteTwo > voteOne) {
			res = voteTwo - voteOne;
		} else if(voteOne > voteTwo) {
			res = voteOne - voteTwo;
		} else{
			res = 0;
		}

    } while ( res > TIMER_B_THRESHOLD);

    return voteTwo;
}

void Timer_B_setOutputMode(uint16_t baseAddress,
                             uint16_t compareRegister,
                             uint16_t compareOutputMode)
{
    uint16_t temp = HWREG16(baseAddress + compareRegister);
    HWREG16(baseAddress + compareRegister) = temp & ~(OUTMOD_7) | compareOutputMode;
}

void Timer_B_selectOutputHighImpedanceTrigger(uint16_t baseAddress,
                                                uint8_t triggerSelect)
{
#ifdef TB0TRGSEL
    if (baseAddress == TIMER_B0_BASE) {
        HWREG16(SYS_BASE + OFS_SYSCFG2) &= ~TB0TRGSEL;
        HWREG16(SYS_BASE + OFS_SYSCFG2) |= triggerSelect<<15;
    }
#endif
#ifdef TB1TRGSEL
    if (baseAddress == TIMER_B1_BASE) {
        HWREG16(SYS_BASE + OFS_SYSCFG2) &= ~TB1TRGSEL;
        HWREG16(SYS_BASE + OFS_SYSCFG2) |= triggerSelect<<14;
    }
#endif    
}

void Timer_B_remapPins(uint16_t baseAddress, uint8_t pinsSelect)
{
#ifdef TBRMP
    HWREG16(SYS_BASE + OFS_SYSCFG3) &= ~TBRMP;
    HWREG16(SYS_BASE + OFS_SYSCFG3) |= pinsSelect<<1;
#endif
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for timer_b_api
//! @}
//
//*****************************************************************************
