//*****************************************************************************
//
// sysctl.h - Driver for the SYSCTL Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_SYSCTL_H__
#define __MSP430WARE_SYSCTL_H__

#include <App/MSP430FR2xx_4xx/inc/hw_memmap.h>

#ifdef __MSP430_HAS_SYS__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// The following are values that can be passed to the BSLRAMAssignment
// parameter for functions: SysCtl_setRAMAssignedToBSL().
//
//*****************************************************************************
#define SYSCTL_BSLRAMASSIGN_NORAM                                  (!(SYSBSLR))
#define SYSCTL_BSLRAMASSIGN_LOWEST16BYTES                             (SYSBSLR)

//*****************************************************************************
//
// The following are values that can be passed to the mailboxSizeSelect
// parameter for functions: SysCtl_initJTAGMailbox().
//
//*****************************************************************************
#define SYSCTL_JTAGMBSIZE_16BIT                                    (!(JMBMODE))
#define SYSCTL_JTAGMBSIZE_32BIT                                       (JMBMODE)

//*****************************************************************************
//
// The following are values that can be passed to the autoClearInboxFlagSelect
// parameter for functions: SysCtl_initJTAGMailbox().
//
//*****************************************************************************
#define SYSCTL_JTAGINBOX0AUTO_JTAGINBOX1AUTO       (!(JMBCLR0OFF + JMBCLR1OFF))
#define SYSCTL_JTAGINBOX0AUTO_JTAGINBOX1SW                         (JMBCLR1OFF)
#define SYSCTL_JTAGINBOX0SW_JTAGINBOX1AUTO                         (JMBCLR0OFF)
#define SYSCTL_JTAGINBOX0SW_JTAGINBOX1SW              (JMBCLR0OFF + JMBCLR1OFF)

//*****************************************************************************
//
// The following are values that can be passed to the mailboxFlagMask parameter
// for functions: SysCtl_getJTAGMailboxFlagStatus(), and
// SysCtl_clearJTAGMailboxFlagStatus().
//
//*****************************************************************************
#define SYSCTL_JTAGOUTBOX_FLAG0                                     (JMBOUT0FG)
#define SYSCTL_JTAGOUTBOX_FLAG1                                     (JMBOUT1FG)
#define SYSCTL_JTAGINBOX_FLAG0                                       (JMBIN0FG)
#define SYSCTL_JTAGINBOX_FLAG1                                       (JMBIN1FG)

//*****************************************************************************
//
// The following are values that can be passed to the inboxSelect parameter for
// functions: SysCtl_getJTAGInboxMessage16Bit().
//
//*****************************************************************************
#define SYSCTL_JTAGINBOX_0                                                (0x0)
#define SYSCTL_JTAGINBOX_1                                                (0x2)

//*****************************************************************************
//
// The following are values that can be passed to the outboxSelect parameter
// for functions: SysCtl_setJTAGOutgoingMessage16Bit().
//
//*****************************************************************************
#define SYSCTL_JTAGOUTBOX_0                                               (0x0)
#define SYSCTL_JTAGOUTBOX_1                                               (0x2)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the SysCtl_getBSLEntryIndication() function.
//
//*****************************************************************************
#define SYSCTL_BSLENTRY_INDICATED                                         (0x1)
#define SYSCTL_BSLENTRY_NOTINDICATED                                      (0x0)

//*****************************************************************************
//
// The following are values that can be passed to the writeEnable parameter for
// functions: SysCtl_enableFRAMWrite(); the writeProtect parameter for
// functions: SysCtl_protectFRAMWrite().
//
//*****************************************************************************
#define SYSCTL_FRAMWRITEPROTECTION_DATA                                   (0x2)
#define SYSCTL_FRAMWRITEPROTECTION_PROGRAM                                (0x1)

//*****************************************************************************
//
// The following are values that can be passed to the dataSource parameter for
// functions: SysCtl_setInfraredConfig().
//
//*****************************************************************************
#define SYSCTL_INFRAREDDATASOURCE_CONFIG                                  (0x0)
#define SYSCTL_INFRAREDDATASOURCE_IRDATA                              (IRDSSEL)

//*****************************************************************************
//
// The following are values that can be passed to the mode parameter for
// functions: SysCtl_setInfraredConfig().
//
//*****************************************************************************
#define SYSCTL_INFRAREDMODE_ASK                                           (0x0)
#define SYSCTL_INFRAREDMODE_FSK                                        (IRMSEL)

//*****************************************************************************
//
// The following are values that can be passed to the polarity parameter for
// functions: SysCtl_setInfraredConfig().
//
//*****************************************************************************
#define SYSCTL_INFRAREDPOLARITY_NORMAL                                    (0x0)
#define SYSCTL_INFRAREDPOLARITY_INVERTED                               (IRPSEL)

//*****************************************************************************
//
// The following are values that can be passed to the offsetAddress parameter
// for functions: SysCtl_setFRWPOA().
//
//*****************************************************************************
#define SYSCTL_FRWPOA0                                                  FRWPOA0
#define SYSCTL_FRWPOA1                                                  FRWPOA1
#define SYSCTL_FRWPOA2                                                  FRWPOA2
#define SYSCTL_FRWPOA3                                                  FRWPOA3
#define SYSCTL_FRWPOA4                                                  FRWPOA4
#define SYSCTL_FRWPOA5                                                  FRWPOA5

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Sets the JTAG pins to be exclusively for JTAG until a BOR occurs.
//!
//! This function sets the JTAG pins to be exclusively used for the JTAG, and
//! not to be shared with the GPIO pins. This setting can only be cleared when
//! a BOR occurs.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableDedicatedJTAGPins(void);

//*****************************************************************************
//
//! \brief Returns the indication of a BSL entry sequence from the Spy-Bi-Wire.
//!
//! This function returns the indication of a BSL entry sequence from the Spy-
//! Bi-Wire.
//!
//!
//! \return One of the following:
//!         - \b SYSCTL_BSLENTRY_INDICATED
//!         - \b SYSCTL_BSLENTRY_NOTINDICATED
//!         \n indicating if a BSL entry sequence was detected
//
//*****************************************************************************
extern uint8_t SysCtl_getBSLEntryIndication(void);

//*****************************************************************************
//
//! \brief Enables PMM Access Protection.
//!
//! This function enables the PMM Access Protection, which will lock any
//! changes on the PMM control registers until a BOR occurs.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enablePMMAccessProtect(void);

//*****************************************************************************
//
//! \brief Enables RAM-based Interrupt Vectors.
//!
//! This function enables RAM-base Interrupt Vectors, which means that
//! interrupt vectors are generated with the end address at the top of RAM,
//! instead of the top of the lower 64kB of flash.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableRAMBasedInterruptVectors(void);

//*****************************************************************************
//
//! \brief Disables RAM-based Interrupt Vectors.
//!
//! This function disables the interrupt vectors from being generated at the
//! top of the RAM.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_disableRAMBasedInterruptVectors(void);

//*****************************************************************************
//
//! \brief Enables BSL memory protection.
//!
//! This function enables protection on the BSL memory, which prevents any
//! reading, programming, or erasing of the BSL memory.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableBSLProtect(void);

//*****************************************************************************
//
//! \brief Disables BSL memory protection.
//!
//! This function disables protection on the BSL memory.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_disableBSLProtect(void);

//*****************************************************************************
//
//! \brief Enables BSL memory.
//!
//! This function enables BSL memory, which allows BSL memory to be addressed
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableBSLMemory(void);

//*****************************************************************************
//
//! \brief Disables BSL memory.
//!
//! This function disables BSL memory, which makes BSL memory act like vacant
//! memory.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_disableBSLMemory(void);

//*****************************************************************************
//
//! \brief Sets RAM assignment to BSL area.
//!
//! This function allows RAM to be assigned to BSL, based on the selection of
//! the BSLRAMAssignment parameter.
//!
//! \param BSLRAMAssignment is the selection of if the BSL should be placed in
//!        RAM or not.
//!        Valid values are:
//!        - \b SYSCTL_BSLRAMASSIGN_NORAM [Default]
//!        - \b SYSCTL_BSLRAMASSIGN_LOWEST16BYTES
//!        \n Modified bits are \b SYSBSLR of \b SYSBSLC register.
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_setRAMAssignedToBSL(uint8_t BSLRAMAssignment);

//*****************************************************************************
//
//! \brief Initializes JTAG Mailbox with selected properties.
//!
//! This function sets the specified settings for the JTAG Mailbox system. The
//! settings that can be set are the size of the JTAG messages, and the auto-
//! clearing of the inbox flags. If the inbox flags are set to auto-clear, then
//! the inbox flags will be cleared upon reading of the inbox message buffer,
//! otherwise they will have to be reset by software using the
//! SYS_clearJTAGMailboxFlagStatus() function.
//!
//! \param mailboxSizeSelect is the size of the JTAG Mailboxes, whether 16- or
//!        32-bits.
//!        Valid values are:
//!        - \b SYSCTL_JTAGMBSIZE_16BIT [Default] - the JTAG messages will take
//!           up only one JTAG mailbox (i. e. an outgoing message will take up
//!           only 1 outbox of the JTAG mailboxes)
//!        - \b SYSCTL_JTAGMBSIZE_32BIT - the JTAG messages will be contained
//!           within both JTAG mailboxes (i. e. an outgoing message will take
//!           up both Outboxes of the JTAG mailboxes)
//!        \n Modified bits are \b JMBMODE of \b SYSJMBC register.
//! \param autoClearInboxFlagSelect decides how the JTAG inbox flags should be
//!        cleared, whether automatically after the corresponding outbox has
//!        been written to, or manually by software.
//!        Valid values are:
//!        - \b SYSCTL_JTAGINBOX0AUTO_JTAGINBOX1AUTO [Default] - both JTAG
//!           inbox flags will be reset automatically when the corresponding
//!           inbox is read from.
//!        - \b SYSCTL_JTAGINBOX0AUTO_JTAGINBOX1SW - only JTAG inbox 0 flag is
//!           reset automatically, while JTAG inbox 1 is reset with the
//!        - \b SYSCTL_JTAGINBOX0SW_JTAGINBOX1AUTO - only JTAG inbox 1 flag is
//!           reset automatically, while JTAG inbox 0 is reset with the
//!        - \b SYSCTL_JTAGINBOX0SW_JTAGINBOX1SW - both JTAG inbox flags will
//!           need to be reset manually by the
//!        \n Modified bits are \b JMBCLR0OFF and \b JMBCLR1OFF of \b SYSJMBC
//!        register.
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_initJTAGMailbox(uint8_t mailboxSizeSelect,
                                   uint8_t autoClearInboxFlagSelect);

//*****************************************************************************
//
//! \brief Returns the status of the selected JTAG Mailbox flags.
//!
//! This function will return the status of the selected JTAG Mailbox flags in
//! bit mask format matching that passed into the mailboxFlagMask parameter.
//!
//! \param mailboxFlagMask is the bit mask of JTAG mailbox flags that the
//!        status of should be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b SYSCTL_JTAGOUTBOX_FLAG0 - flag for JTAG outbox 0
//!        - \b SYSCTL_JTAGOUTBOX_FLAG1 - flag for JTAG outbox 1
//!        - \b SYSCTL_JTAGINBOX_FLAG0 - flag for JTAG inbox 0
//!        - \b SYSCTL_JTAGINBOX_FLAG1 - flag for JTAG inbox 1
//!
//! \return A bit mask of the status of the selected mailbox flags.
//
//*****************************************************************************
extern uint8_t SysCtl_getJTAGMailboxFlagStatus(uint8_t mailboxFlagMask);

//*****************************************************************************
//
//! \brief Clears the status of the selected JTAG Mailbox flags.
//!
//! This function clears the selected JTAG Mailbox flags.
//!
//! \param mailboxFlagMask is the bit mask of JTAG mailbox flags that the
//!        status of should be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b SYSCTL_JTAGOUTBOX_FLAG0 - flag for JTAG outbox 0
//!        - \b SYSCTL_JTAGOUTBOX_FLAG1 - flag for JTAG outbox 1
//!        - \b SYSCTL_JTAGINBOX_FLAG0 - flag for JTAG inbox 0
//!        - \b SYSCTL_JTAGINBOX_FLAG1 - flag for JTAG inbox 1
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_clearJTAGMailboxFlagStatus(uint8_t mailboxFlagMask);

//*****************************************************************************
//
//! \brief Returns the contents of the selected JTAG Inbox in a 16 bit format.
//!
//! This function returns the message contents of the selected JTAG inbox. If
//! the auto clear settings for the Inbox flags were set, then using this
//! function will automatically clear the corresponding JTAG inbox flag.
//!
//! \param inboxSelect is the chosen JTAG inbox that the contents of should be
//!        returned
//!        Valid values are:
//!        - \b SYSCTL_JTAGINBOX_0 - return contents of JTAG inbox 0
//!        - \b SYSCTL_JTAGINBOX_1 - return contents of JTAG inbox 1
//!
//! \return The contents of the selected JTAG inbox in a 16 bit format.
//
//*****************************************************************************
extern uint16_t SysCtl_getJTAGInboxMessage16Bit(uint8_t inboxSelect);

//*****************************************************************************
//
//! \brief Returns the contents of JTAG Inboxes in a 32 bit format.
//!
//! This function returns the message contents of both JTAG inboxes in a 32 bit
//! format. This function should be used if 32-bit messaging has been set in
//! the SYS_initJTAGMailbox() function. If the auto clear settings for the
//! Inbox flags were set, then using this function will automatically clear
//! both JTAG inbox flags.
//!
//!
//! \return The contents of both JTAG messages in a 32 bit format.
//
//*****************************************************************************
extern uint32_t SysCtl_getJTAGInboxMessage32Bit(void);

//*****************************************************************************
//
//! \brief Sets a 16 bit outgoing message in to the selected JTAG Outbox.
//!
//! This function sets the outgoing message in the selected JTAG outbox. The
//! corresponding JTAG outbox flag is cleared after this function, and set
//! after the JTAG has read the message.
//!
//! \param outboxSelect is the chosen JTAG outbox that the message should be
//!        set it.
//!        Valid values are:
//!        - \b SYSCTL_JTAGOUTBOX_0 - set the contents of JTAG outbox 0
//!        - \b SYSCTL_JTAGOUTBOX_1 - set the contents of JTAG outbox 1
//! \param outgoingMessage is the message to send to the JTAG.
//!        \n Modified bits are \b MSGHI and \b MSGLO of \b SYSJMBOx register.
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_setJTAGOutgoingMessage16Bit(uint8_t outboxSelect,
                                               uint16_t outgoingMessage);

//*****************************************************************************
//
//! \brief Sets a 32 bit message in to both JTAG Outboxes.
//!
//! This function sets the 32-bit outgoing message in both JTAG outboxes. The
//! JTAG outbox flags are cleared after this function, and set after the JTAG
//! has read the message.
//!
//! \param outgoingMessage is the message to send to the JTAG.
//!        \n Modified bits are \b MSGHI and \b MSGLO of \b SYSJMBOx register.
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_setJTAGOutgoingMessage32Bit(uint32_t outgoingMessage);

//*****************************************************************************
//
//! \brief Sets write protected for data FRAM and program FRAM.
//!
//! \param writeProtect is the value setting data FRAM and program write
//!        protection.
//!        Mask value is the logical OR of any of the following:
//!        - \b SYSCTL_FRAMWRITEPROTECTION_DATA - data FRAM write protected
//!        - \b SYSCTL_FRAMWRITEPROTECTION_PROGRAM - program FRAM write
//!           protected
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_protectFRAMWrite(uint8_t writeProtect);

//*****************************************************************************
//
//! \brief Sets write enable for data FRAM and program FRAM.
//!
//! \param writeEnable is the value setting data FRAM and program write
//!        enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b SYSCTL_FRAMWRITEPROTECTION_DATA - data FRAM write protected
//!        - \b SYSCTL_FRAMWRITEPROTECTION_PROGRAM - program FRAM write
//!           protected
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableFRAMWrite(uint8_t writeEnable);

//*****************************************************************************
//
//! \brief Sets infrared configuration bits.
//!
//! \param dataSource is the value setting infrared data source.
//!        Valid values are:
//!        - \b SYSCTL_INFRAREDDATASOURCE_CONFIG - infrared data from hardware
//!           peripherals upon device configuration
//!        - \b SYSCTL_INFRAREDDATASOURCE_IRDATA - infrared data from IRDATA
//!           bit
//! \param mode is the value setting infrared mode.
//!        Valid values are:
//!        - \b SYSCTL_INFRAREDMODE_ASK - infrared ASK mode
//!        - \b SYSCTL_INFRAREDMODE_FSK - infrared FSK mode
//! \param polarity is the value setting infrared polarity.
//!        Valid values are:
//!        - \b SYSCTL_INFRAREDPOLARITY_NORMAL - infrared normal polarity
//!        - \b SYSCTL_INFRAREDPOLARITY_INVERTED - infrared inverted polarity
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_setInfraredConfig(uint8_t dataSource,
                                     uint8_t mode,
                                     uint8_t polarity);

//*****************************************************************************
//
//! \brief Enables infrared function.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_enableInfrared(void);

//*****************************************************************************
//
//! \brief Disables infrared function.
//!
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_disableInfrared(void);

//*****************************************************************************
//
//! \brief This function returns the infrared data if the infrared data source
//! is configured as from IRDATA bit.
//!
//!
//! \return the infrared logic data '0' or '1'
//
//*****************************************************************************
extern uint8_t SysCtl_getInfraredData(void);

//*****************************************************************************
//
//! \brief This function sets the Program FRAM write protection offset address
//! from the beginning of Program FRAM. The offset increases by 1 kB
//! resolution.
//!
//! \param offsetAddress is the Program FRAM write protection offset address
//!        from the beginning of Program FRAM, with offset increases of 1KB
//!        resolution.
//!        Mask value is the logical OR of any of the following:
//!        - \b SYSCTL_FRWPOA0
//!        - \b SYSCTL_FRWPOA1
//!        - \b SYSCTL_FRWPOA2
//!        - \b SYSCTL_FRWPOA3
//!        - \b SYSCTL_FRWPOA4
//!        - \b SYSCTL_FRWPOA5
//!
//! \return None
//
//*****************************************************************************
extern void SysCtl_setFRWPOA(uint8_t offsetAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_SYSCTL_H__
