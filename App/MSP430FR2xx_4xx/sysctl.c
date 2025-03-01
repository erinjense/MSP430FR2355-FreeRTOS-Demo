//*****************************************************************************
//
// sysctl.c - Driver for the sysctl Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup sysctl_api sysctl
//! @{
//
//*****************************************************************************

#include <App/MSP430FR2xx_4xx/inc/hw_memmap.h>

#ifdef __MSP430_HAS_SYS__
#include <App/MSP430FR2xx_4xx/sysctl.h>

#include <assert.h>

void SysCtl_enableDedicatedJTAGPins (void)
{
    HWREG8(SYS_BASE + OFS_SYSCTL_L) |= SYSJTAGPIN;
}

uint8_t SysCtl_getBSLEntryIndication (void)
{
    if ( HWREG8(SYS_BASE + OFS_SYSCTL_L) & SYSBSLIND){
        return (SYSCTL_BSLENTRY_INDICATED) ;
    } else   {
        return (SYSCTL_BSLENTRY_NOTINDICATED) ;
    }
}

void SysCtl_enablePMMAccessProtect (void)
{
    HWREG8(SYS_BASE + OFS_SYSCTL_L) |= SYSPMMPE;
}

void SysCtl_enableRAMBasedInterruptVectors (void)
{
    HWREG8(SYS_BASE + OFS_SYSCTL_L) |= SYSRIVECT;
}

void SysCtl_disableRAMBasedInterruptVectors (void)
{
    HWREG8(SYS_BASE + OFS_SYSCTL_L) &= ~(SYSRIVECT);
}

void SysCtl_enableBSLProtect (void)
{
    HWREG16(SYS_BASE + OFS_SYSBSLC) |= SYSBSLPE;
}

void SysCtl_disableBSLProtect (void)
{
    HWREG16(SYS_BASE + OFS_SYSBSLC) &= ~(SYSBSLPE);
}

void SysCtl_enableBSLMemory (void)
{
    HWREG16(SYS_BASE + OFS_SYSBSLC) &= ~(SYSBSLOFF);
}

void SysCtl_disableBSLMemory (void)
{
    HWREG16(SYS_BASE + OFS_SYSBSLC) |= SYSBSLOFF;
}

void SysCtl_setRAMAssignedToBSL (uint8_t BSLRAMAssignment)
{
    HWREG8(SYS_BASE + OFS_SYSBSLC_L) &= ~(SYSBSLR);
    HWREG8(SYS_BASE + OFS_SYSBSLC_L) |= BSLRAMAssignment;
}

void SysCtl_initJTAGMailbox (uint8_t mailboxSizeSelect,
    uint8_t autoClearInboxFlagSelect)
{
    HWREG8(SYS_BASE + OFS_SYSJMBC_L) &= ~(JMBCLR1OFF + JMBCLR0OFF + JMBMODE);
    HWREG8(SYS_BASE + OFS_SYSJMBC_L) |=
        mailboxSizeSelect + autoClearInboxFlagSelect;
}

uint8_t SysCtl_getJTAGMailboxFlagStatus (uint8_t mailboxFlagMask)
{
    return ( HWREG8(SYS_BASE + OFS_SYSJMBC_L) & mailboxFlagMask);
}

void SysCtl_clearJTAGMailboxFlagStatus (uint8_t mailboxFlagMask)
{
    HWREG8(SYS_BASE + OFS_SYSJMBC_L) &= ~(mailboxFlagMask);
}

uint16_t SysCtl_getJTAGInboxMessage16Bit (uint8_t inboxSelect)
{
    return ( HWREG16(SYS_BASE + OFS_SYSJMBI0 + inboxSelect) );
}

uint32_t SysCtl_getJTAGInboxMessage32Bit (void)
{
    uint32_t JTAGInboxMessageLow = HWREG16(SYS_BASE + OFS_SYSJMBI0);
    uint32_t JTAGInboxMessageHigh = HWREG16(SYS_BASE + OFS_SYSJMBI1);

    return ( (JTAGInboxMessageHigh << 16) + JTAGInboxMessageLow );
}

void SysCtl_setJTAGOutgoingMessage16Bit (uint8_t outboxSelect,
    uint16_t outgoingMessage)
{
    HWREG16(SYS_BASE + OFS_SYSJMBO0 + outboxSelect) = outgoingMessage;
}

void SysCtl_setJTAGOutgoingMessage32Bit (uint32_t outgoingMessage)
{
    HWREG16(SYS_BASE + OFS_SYSJMBO0) = (outgoingMessage);
    HWREG16(SYS_BASE + OFS_SYSJMBO1) = (outgoingMessage >> 16);
}

void SysCtl_protectFRAMWrite( uint8_t writeProtect){
    uint8_t state = HWREG8(SYS_BASE + OFS_SYSCFG0_L);

#ifndef DFWP
    if (writeProtect == SYSCTL_FRAMWRITEPROTECTION_DATA) {
        return;
    }
#endif

#ifdef FRWPPW
    HWREG16(SYS_BASE + OFS_SYSCFG0) = FWPW | state | writeProtect;
#else
    HWREG8(SYS_BASE + OFS_SYSCFG0_L) |= writeProtect;
#endif
}

void SysCtl_enableFRAMWrite( uint8_t writeEnable){
    uint8_t state = HWREG8(SYS_BASE + OFS_SYSCFG0_L);

#ifndef DFWP
    if (writeEnable == SYSCTL_FRAMWRITEPROTECTION_DATA) {
        return;
    }
#endif
    
#ifdef FRWPPW
    HWREG16(SYS_BASE + OFS_SYSCFG0) = FWPW | (state & ~writeEnable);
#else
    HWREG8(SYS_BASE + OFS_SYSCFG0_L) &= ~writeEnable;
#endif
}

void SysCtl_setInfraredConfig( uint8_t dataSource,uint8_t mode, uint8_t polarity)
{
    HWREG16(SYS_BASE + OFS_SYSCFG1) &= ~(IRDSSEL | IRMSEL | IRPSEL);

    HWREG16(SYS_BASE + OFS_SYSCFG1) |= dataSource | mode | polarity;
}

void SysCtl_enableInfrared(void)
{
    HWREG16(SYS_BASE + OFS_SYSCFG1) |= IREN;
}

void SysCtl_disableInfrared(void)
{
    HWREG16(SYS_BASE + OFS_SYSCFG1) &= ~IREN;
}

uint8_t SysCtl_getInfraredData(void)
{
    return ((HWREG16(SYS_BASE + OFS_SYSCFG1) & 0x10) >> 4);
}

void SysCtl_setFRWPOA( uint8_t offsetAddress){
    uint16_t state = HWREG16(SYS_BASE + OFS_SYSCFG0);
#ifdef FRWPOA
    HWREG16(SYS_BASE + OFS_SYSCFG0) = (FRWPPW | offsetAddress | (state & (DFWP | PFWP)));
#endif
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for sysctl_api
//! @}
//
//*****************************************************************************
