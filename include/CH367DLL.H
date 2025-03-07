// 2011.07.01, 2016.09.28, 2021.10.15
//*********************************************
//** Application Layer Interface Library for PCIe Bus Interface Chip CH36X **
//** Copyright(C) WCH 2021 **
//** http://wch.cn **
//********************************************
//** CH367DLL V1.2 **
//** DLL for PCIE interface chip **
//** Development Environment: C, VC5.0 **
//** Supported Chips: CH360/CH366/CH367/CH368 **
//** Operating Environment: Windows 2000/XP/7/8/8.1/10/11 **
//********************************************
//
/* !!Note:
     1. The CH367DLL interface is suitable for CH367 & CH368 chips. Functions starting with CH367 are applicable to both CH367 and CH368, while functions starting with CH368 are only applicable to CH368 chips and not to CH367.
     2. CH368 supports PC programs to read/write I/O ports or memory in four-byte (double word) units.
       For CH367mReadIoWord/CH367mWriteIoWord, when performing word operations, the starting address must be a word-aligned address within the base address range (multiple of 2);
       For CH367mReadIoDword/CH367mWriteIoDword/CH368ReadMemDword/CH368WriteMemDword, when performing four-byte access, the starting address must be a double word-aligned address within the base address range (multiple of 4).
*/

#ifndef _CH367_DLL_H
#define _CH367_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

#define mOFFSET(s, m) ((ULONG_PTR) & (((s *)0)->m)) // Macro to get the relative offset address of a structure member

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b)) // Maximum value
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b)) // Minimum value
#endif

#ifdef ExAllocatePool
#undef ExAllocatePool // Remove memory allocation with TAG
#endif

#ifndef NTSTATUS
typedef LONG NTSTATUS; // Return status
#endif

typedef struct _PCI_CONFIG { // Define PCI configuration space data structure
    USHORT mPcVendorId; // 00H Vendor ID
    USHORT mPcDeviceId; // 02H Device ID
    USHORT mPcCommandReg; // 04H Command register
    USHORT mPcStatusReg; // 06H Status register
    UCHAR mPcRevisionId; // 08H Revision ID
    UCHAR mPcProgramIf; // 09H Programming interface
    UCHAR mPcSubClass; // 0AH Subclass code
    UCHAR mPcBaseClass; // 0BH Base class code
    UCHAR mPcCacheLine; // 0CH Cache line size
    UCHAR mPcLatenTimer; // 0DH Latency timer
    UCHAR mPcHeaderType; // 0EH Header type
    UCHAR mPcBistReg; // 0FH Built-in self-test register
    ULONG mPcBaseAddr0; // 10H Base address register 0
    ULONG mPcBaseAddr1; // 14H Base address register 1
    ULONG mPcBaseAddr2; // 18H Base address register 2
    ULONG mPcBaseAddr3; // 1CH Base address register 3
    ULONG mPcBaseAddr4; // 20H Base address register 4
    ULONG mPcBaseAddr5; // 24H Base address register 5
    ULONG mPcCardCis; // 28H CardBus CIS pointer
    USHORT mPcSubSysVen; // 2CH Subsystem vendor ID
    USHORT mPcSubSysDev; // 2EH Subsystem device ID
    ULONG mPcExpansRom; // 30H Expansion ROM base address
    UCHAR mPcCapPtr; // 34H Capabilities pointer
    UCHAR mPcReserved1[3]; // 35H Reserved
    ULONG mPcReserved2; // 38H Reserved
    UCHAR mPcInterLine; // 3CH Interrupt line
    UCHAR mPcInterPin; // 3DH Interrupt pin
    UCHAR mPcMinGrant; // 3EH Minimum grant
    UCHAR mPcMaxLatency; // 3FH Maximum latency
} mPCI_CONFIG, *mPPCI_CONFIG;

typedef struct _PCI_EXP_ROM { // PCI expansion ROM data structure
    UCHAR mPerRomSig[4]; // 00H PCI expansion ROM signature string 'PCIR'
    USHORT mPerVendorId; // 04H Vendor ID
    USHORT mPerDeviceId; // 06H Device ID
    USHORT mPerVpdPtr; // 08H Vital product data pointer
    USHORT mPerStrucLen; // 0AH Length of PCI expansion ROM data structure
    UCHAR mPerRevision; // 0CH Revision of PCI expansion ROM data structure
    UCHAR mPerProgramIf; // 0DH Programming interface
    UCHAR mPerSubClass; // 0EH Subclass code
    UCHAR mPerBaseClass; // 0FH Base class code
    USHORT mPerImageLen; // 10H Image length
    USHORT mPerImageRev; // 12H Revision of code/data in image
    UCHAR mPerCodeType; // 14H Code type
    UCHAR mPerIndicator; // 15H Image indicator
    USHORT mPerReserved; // 16H Reserved
} mPCI_EXP_ROM, *mPPCI_EXP_ROM;

typedef struct _CH367_IO_REG { // CH367 chip I/O space registers
    UCHAR mCH367IoPort[0xE8]; // 00H-E7H, 232 bytes of standard I/O ports
    UCHAR mCH367GPOR; // E8H General-purpose output register
    UCHAR mCH367GPVR; // E9H General-purpose variable register
    UCHAR mCH367GPIR; // EAH General-purpose input register
    UCHAR mCH367IntCtr; // EBH Interrupt control register
    union {
        UCHAR mCH367IoBuf8; // ECH 8-bit passive parallel interface data buffer
        ULONG mCH367IoBuf32; // ECH 32-bit passive parallel interface data buffer
    };
    union {
        USHORT mCH368MemAddr; // F0H Memory interface: A15-A0 address setting register
        struct {
            UCHAR mCH368MemAddrL; // F0H Memory interface: A7-A0 address setting register
            union {
                UCHAR mCH368MemAddrH; // F1H Memory interface: A15-A8 address setting register
                UCHAR mCH367GPOR2; // F1H General-purpose output register 2
            };
        } ASR;
    };
    UCHAR mCH367IORESV2; // F2H Reserved
    UCHAR mCH368MemData; // F3H Memory interface: Memory data access register
    union {
        UCHAR mCH367Data8Sta; // F4H D7-D0 port status register
        ULONG mCH367SData32Sta; // F4H D31-D0 port status register
    };
    UCHAR mCH367Status; // F8H Miscellaneous control and status register
    UCHAR mCH367IO_RESV3; // F9H Reserved
    UCHAR mCH367Speed; // FAH Read/write speed control register
    UCHAR mCH367PDataCtrl; // FBH Passive parallel interface control register
    UCHAR mCH367IoTime; // FCH Hardware loop count register
    UCHAR mCH367SPICtrl; // FDH SPI control register
    UCHAR mCH367SPIData; // FEH SPI data register
    UCHAR mCH367IO_RESV4; // FFH Reserved
} mCH367_IO_REG, *mPCH367_IO_REG;

typedef struct _CH368_MEM_REG { // CH367 chip memory space
    UCHAR mCH368MemPort[0x8000]; // 0000H-7FFFH, 32768 bytes of standard memory units
} mCH368_MEM_REG, *mPCH368_MEM_REG;

typedef struct _WIN32_COMMAND { // Define WIN32 command interface structure
    union {
        ULONG mFunction; // Input: specifies the function code
        NTSTATUS mStatus; // Output: returns the operation status
    };
    ULONG mLength; // Access length, returns the length of subsequent data
    union {
        PVOID mAddress; // Start address, returns the address
        ULONGLONG mAddress64; // Start address, returns the address
    };
    UCHAR mBuffer[8]; // Data buffer, length from 0 to 32KB
} mWIN32_COMMAND, *mPWIN32_COMMAND;

// WIN32 application layer interface commands
#define IOCTL_CH367_COMMAND (FILE_DEVICE_UNKNOWN << 16 | FILE_ANY_ACCESS << 14 | 0x0f36 << 2 | METHOD_BUFFERED) // Dedicated interface

#define mWIN32_COMMAND_HEAD mOFFSET(mWIN32_COMMAND, mBuffer) // Length of WIN32 command interface header

#define mCH367_MAX_NUMBER 16 // Maximum number of CH367 devices connected simultaneously

#define mMAX_BUFFER_LENGTH max(sizeof(mCH367_IO_REG), sizeof(mCH368_MEM_REG)) // Maximum length of data buffer

#define mMAX_COMMAND_LENGTH (mWIN32_COMMAND_HEAD + mMAX_BUFFER_LENGTH) // Maximum data length plus the length of the command structure header

#define mCH368_MEM_BASE_AUTO ((PVOID)(ULONG_PTR)0xffffffff) // Automatically set CH368 memory base address

#define mCH367_INT_LINE_AUTO 0xffffffff // Automatically detect CH367 interrupt number

// Register bit definitions
// 0xE8
#define mBitI2cSdaOut 0x01 // Set SDA pin output value (1), 1*: high, 0: low
#define mBitI2cSclOut 0x02 // Set SCL pin output value (1)
#define mBitSpiScsOut 0x04 // Set SCS pin output value (1)
#define mBitBeWakenEnable 0x32 // Enable support for forced wake-up (0)
#define mBitSetSpiSdxDir 0x64 // Set SDX pin data direction (0)
// 0xEA
#define mBitSpiSdxOut 0x01 // Input SDA pin state (1)
#define mBitIntStatus 0x08 // Input INT# pin state (1)
#define mBitRDStatus 0x10 // Input RD# pin state (1)
#define mBitWRStatus 0x20 // Input WR# pin state (1)
#define mBitAddrStatus 0x40 // Input ADDR pin state (1)
#define mBitSdxStatus 0x80 // Input SDX pin state (1)
// 0xEB
#define mBitGlobalIntEnable 0x02 // Global interrupt enable (0)
#define mBitIntPolarity 0x04 // INT# pin interrupt input polarity (0), 0: low level/rising edge; 1: high level/falling edge
#define mBitIntInType 0x08 // INT# pin interrupt input type (0)
// 0xF8:
#define mBitAddr15Out 0x01 // Set A15 pin output value (1)
#define mBitIntAction 0x04 // Interrupt activation state (0)
#define mBitIntStatus 0x08 // Input INT# pin state (1)
#define mBitRST0Out 0x80 // Set RSTO pin output value (1)
// 0xFA
#define mBitSpeedInact 0xFA // Total width of read/write signals including setup and hold time (0111)
#define mBitDataAddrOutCreate 0x10 // Data and address output setup time (0), 0: 15nS, 1: 45nS
#define mBitDataAddrOutKeep 0x20 // Data and address output hold time (0), 0: 15nS, 1: 45nS
#define mBitLocalBusWidth 0x40 // Local data bus width (0), 0: 8-bit, 1: 32-bit
// 0xFD
#define mBitHWCountH4 0x0F // High 4 bits of hardware loop count (XXXX), combined with the hardware loop count register CNTR to form a total of 12 bits
#define mBitSpiTransportStatus 0x10 // SPI transfer in progress status (0), 0: idle, 1: transferring
#define mBitSpiClockFreq 0x20 // Select SPI serial clock frequency (0), 0: 31.3MHz, 1: 15.6MHz
#define mBitSelSpiInPin 0x40 // Select SPI data input pin (0), 0: SDK, 1: ADDR
#define mBitStartNewSPITrans 0x80 // Enable new transfer after reading SPIDR (0), 0: idle after read, 1: start after read

// Application layer interface function codes
#define mFuncNoOperation 0x00000000 // No operation
#define mFuncGetVersion 0x00000001 // Get driver version number
#define mFuncGetIoBaseAddr 0x00000004 // Read I/O port base address
#define mFuncSetIoBaseAddr 0x00000005 // Set I/O port base address
#define mFuncGetMemBaseAddr 0x00000006 // Read memory base address
#define mFuncSetMemBaseAddr 0x00000007 // Set memory base address, automatically set memory base address
#define mFuncGetInterLine 0x00000008 // Read interrupt number
#define mFuncSetInterLine 0x00000009 // Set interrupt number, automatically detect interrupt number
#define mFuncWaitInterrupt 0x0000000a // Wait for interrupt until CH367 generates an interrupt
#define mFuncAbortWaitInter 0x0000000b // Abort waiting for interrupt, terminate waiting
#define mFuncSetExclusive 0x0000000c // Set exclusive use
#define mFuncReadIoByte 0x00000010 // Read I/O port, continuously read at the same port address in byte units
#define mFuncReadIoWord 0x00000011 // Read I/O port, continuously read at the same port address in word units
#define mFuncReadIoDword 0x00000012 // Read I/O port, continuously read at the same port address in double word units
#define mFuncReadIoBlock 0x00000013 // Read I/O port, increment port address by one after each byte read
#define mFuncReadIo32Block 0x00000028 // Read I/O port, increment port address by four after each four-byte read, maximum length 232B (read from offset address 00H to E7H)
#define mFuncWriteIoByte 0x00000014 // Write I/O port, continuously write at the same port address in byte units
#define mFuncWriteIoWord 0x00000015 // Write I/O port, continuously write at the same port address in word units
#define mFuncWriteIoDword 0x00000016 // Write I/O port, continuously write at the same port address in double word units
#define mFuncWriteIoBlock 0x00000017 // Write I/O port, increment port address by one after each byte write
#define mFuncWriteIo32Block 0x00000029 // Write I/O port, increment port address by four after each four-byte write, maximum length 232B (write from offset address 00H to E7H)
#define mFuncReadMemByte 0x00000018 // Read memory/register
#define mFuncReadMemDword 0x0000001a // Read memory/register
#define mFuncWriteMemByte 0x0000001c // Write memory/register
#define mFuncWriteMemDword 0x0000001e // Write memory/register
#define mFuncReadConfig 0x00000020 // Read PCI configuration space
#define mFuncWriteConfig 0x00000021 // Write PCI configuration space
#define mFuncReadBuffer1 0x00000024 // Read interrupt command buffer 1
#define mFuncWriteBuffer1 0x00000025 // Write interrupt command buffer 1
#define mFuncReadBuffer2 0x00000026 // Read interrupt command buffer 2
#define mFuncWriteBuffer2 0x00000027 // Write interrupt command buffer 2

#define mFuncStreamSPI 0x0000002A // SPI data stream transfer function
#define mFuncSetStreamMode 0x0000002B // Set SPI to SPI3 or SPI4
#define mFuncSetInterMode 0x0000002C // Set interrupt polarity and input type

#define mFuncFlashLock 0x0000002D // Set write protection for Flash-ROM memory
#define mFuncFlashRead 0x0000002E // Read data block from Flash-ROM memory
#define mFuncFlashErase 0x0000002F // Erase specified area of Flash-ROM memory, 4096 bytes (4K) per sector, minimum erase unit is sector
#define mFuncFlashWrite 0x00000030 // Write data block to Flash-ROM memory
#define mFuncRestore 0x00000031 // Restore IO, MEM resources
typedef VOID (CALLBACK * mPCH367_INT_ROUTINE) (VOID); // Interrupt service routine

HANDLE WINAPI CH367OpenDevice( // Open CH367 device, return handle, invalid if error occurs
    BOOL iEnableMemory, // Whether to support memory, CH367 chip does not support, CH368 chip supports
    BOOL iEnableInterrupt, // Whether to support interrupt
    UCHAR iInterruptMode); // Bit 0: Interrupt input polarity 1: high level/falling edge; 0: low level/rising edge; Bit 1: Interrupt input type 1: edge, 0: level

VOID WINAPI CH367CloseDevice(); // Close CH367 device

ULONG WINAPI CH367GetVersion(); // Get DLL version number, return version number

ULONG WINAPI CH367DriverCommand( // Directly pass command to driver, return 0 if error occurs, otherwise return data length
    mPWIN32_COMMAND ioCommand); // Pointer to command structure
// This program returns the data length after calling and still returns the command structure. If it is a read operation, the data is returned in the command structure.
// The returned data length is 0 if the operation fails, and the length of the entire command structure if the operation succeeds. For example, reading one byte returns mWIN32_COMMAND_HEAD+1.
// Before calling, the command structure provides: command function code, start address (optional), data access length (optional).
// After calling, the command structure returns: operation status code, returned base address (optional), subsequent data length (optional).
// The operation status code is defined by WINDOWS, refer to NTSTATUS.H.
// The returned base address is only applicable to the following commands: get I/O base address, get memory base address, get interrupt number. For other commands, the start address remains as input.
// The subsequent data length refers to the data length returned by the read operation, and the data is stored in the subsequent buffer. For write operations, it is generally 0.

ULONG WINAPI CH367GetDrvVersion(); // Get driver version number, return version number, return 0 if error occurs

BOOL WINAPI CH367GetIoBaseAddr( // Get I/O port base address
    mPCH367_IO_REG *oIoBaseAddr); // Address of the unit to save the I/O port base address

BOOL WINAPI CH367SetIoBaseAddr( // Set I/O port base address
    mPCH367_IO_REG iIoBaseAddr); // Specify I/O port base address

BOOL WINAPI CH368GetMemBaseAddr( // Get memory base address
    mPCH368_MEM_REG *oMemBaseAddr); // Address of the unit to save the memory base address

BOOL WINAPI CH368SetMemBaseAddr( // Set memory base address
    mPCH368_MEM_REG iMemBaseAddr); // Specify memory base address, 0 to disable memory, -1 to automatically set

BOOL WINAPI CH367GetIntLine( // Get interrupt number
    PULONG oIntLine); // Address of the unit to save the interrupt number

BOOL WINAPI CH367SetIntLine( // Set interrupt number
    ULONG iIntLine, // Specify interrupt number, 0 to disable interrupt, -1 to automatically detect and set
    UCHAR iInterruptMode); // Bit 0: Interrupt input polarity 1: high level/falling edge; 0: low level/rising edge; Bit 1: Interrupt input type 1: edge, 0: level

BOOL WINAPI CH367WaitInterrupt(); // Wait for interrupt event

BOOL WINAPI CH367AbortWaitInt(); // Abort waiting for interrupt

BOOL WINAPI CH367SetIntRoutine( // Set interrupt service routine
    mPCH367_INT_ROUTINE iIntRoutine); // Specify interrupt service routine, NULL to cancel interrupt service, otherwise call this routine on interrupt

BOOL WINAPI CH367ReadIntCommand( // Read interrupt command buffer
    mPWIN32_COMMAND oCommand, // Point to a sufficiently large buffer to save the read command structure
    ULONG iCmdIndex); // Interrupt command index, 1 or 2

BOOL WINAPI CH367WriteIntCommand( // Write interrupt command buffer
    mPWIN32_COMMAND iCommand, // Point to the command structure as the interrupt command
    ULONG iCmdIndex); // Interrupt command index, 1 or 2

BOOL WINAPI CH367ReadIoByte( // Read one byte from I/O port
    PVOID iAddr, // Specify I/O port address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367ReadIoWord( // Read one word from I/O port
    PVOID iAddr, // Specify I/O port address
    PUSHORT oWord); // Point to a word unit to save the read word data

BOOL WINAPI CH367ReadIoDword( // Read one double word from I/O port
    PVOID iAddr, // Specify I/O port address
    PULONG oDword); // Point to a double word unit to save the read double word data

BOOL WINAPI CH367WriteIoByte( // Write one byte to I/O port
    PVOID iAddr, // Specify I/O port address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH367WriteIoWord( // Write one word to I/O port
    PVOID iAddr, // Specify I/O port address
    USHORT iWord); // Word data to be written

BOOL WINAPI CH367WriteIoDword( // Write one double word to I/O port
    PVOID iAddr, // Specify I/O port address
    ULONG iDword); // Double word data to be written

BOOL WINAPI CH368ReadMemByte( // Read one byte from memory
    PVOID iAddr, // Specify memory address, virtual address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH368ReadMemDword( // Read one double word from memory
    PVOID iAddr, // Specify memory address, virtual address
    PULONG oDword); // Point to a double word unit to save the read double word data

BOOL WINAPI CH368WriteMemByte( // Write one byte to memory
    PVOID iAddr, // Specify memory address, virtual address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH368WriteMemDword( // Write one double word to memory
    PVOID iAddr, // Specify memory address, virtual address
    ULONG iDword); // Double word data to be written

BOOL WINAPI CH367ReadConfig( // Read one byte data from configuration space
    PVOID iOffset, // Specify offset address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367WriteConfig( // Write one byte data to configuration space
    PVOID iOffset, // Specify offset address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH368SetA15_A8( // Set high address, i.e., set A15-A8 output
    UCHAR iA15_A8); // Bit 7-0 corresponds to A15-A8 output, 1: high level, 0: low level

BOOL WINAPI CH367ReadI2C( // Read one byte data from I2C interface
    UCHAR iDevice, // Lower 7 bits specify I2C device address
    UCHAR iAddr, // Specify data unit address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367WriteI2C( // Write one byte data to I2C interface
    UCHAR iDevice, // Lower 7 bits specify I2C device address
    UCHAR iAddr, // Specify data unit address
    UCHAR iByte); // Byte data to be written

HANDLE WINAPI CH367mOpenDevice( // Open CH367 device, return handle, invalid if error occurs
    ULONG iIndex, // Specify CH367 device index, 0 corresponds to the first device
    BOOL iEnableMemory, // Whether to support memory, CH367 chip does not support, CH368 chip supports
    BOOL iEnableInterrupt, // Whether to support interrupt
    UCHAR iInterruptMode); // Bit 0: Interrupt input polarity 1: high level/falling edge; 0: low level/rising edge; Bit 1: Interrupt input type 1: edge, 0: level

VOID WINAPI CH367mCloseDevice( // Close CH367 device
    ULONG iIndex); // Specify CH367 device index

ULONG WINAPI CH367mDriverCommand( // Directly pass command to driver, return 0 if error occurs, otherwise return data length
    ULONG iIndex, // Specify CH367 device index
    mPWIN32_COMMAND ioCommand); // Pointer to command structure
// This program returns the data length after calling and still returns the command structure. If it is a read operation, the data is returned in the command structure.
// The returned data length is 0 if the operation fails, and the length of the entire command structure if the operation succeeds. For example, reading one byte returns mWIN32_COMMAND_HEAD+1.
// Before calling, the command structure provides: command function code, start address (optional), data access length (optional).
// After calling, the command structure returns: operation status code, returned base address (optional), subsequent data length (optional).
// The operation status code is defined by WINDOWS, refer to NTSTATUS.H.
// The returned base address is only applicable to the following commands: get I/O base address, get memory base address, get interrupt number. For other commands, the start address remains as input.
// The subsequent data length refers to the data length returned by the read operation, and the data is stored in the subsequent buffer. For write operations, it is generally 0.

BOOL WINAPI CH367mGetIoBaseAddr( // Get I/O port base address
    ULONG iIndex, // Specify CH367 device index
    mPCH367_IO_REG *oIoBaseAddr); // Address of the unit to save the I/O port base address

BOOL WINAPI CH367mSetIoBaseAddr( // Set I/O port base address
    ULONG iIndex, // Specify CH367 device index
    mPCH367_IO_REG iIoBaseAddr); // Specify I/O port base address

BOOL WINAPI CH368mGetMemBaseAddr( // Get memory base address
    ULONG iIndex, // Specify CH367 device index
    mPCH368_MEM_REG *oMemBaseAddr); // Address of the unit to save the memory base address

BOOL WINAPI CH368mSetMemBaseAddr( // Set memory base address
    ULONG iIndex, // Specify CH367 device index
    mPCH368_MEM_REG iMemBaseAddr); // Specify memory base address, 0 to disable memory, -1 to automatically set

BOOL WINAPI CH367mGetIntLine( // Get interrupt number
    ULONG iIndex, // Specify CH367 device index
    PULONG oIntLine); // Address of the unit to save the interrupt number

BOOL WINAPI CH367mSetIntLine( // Set interrupt number
    ULONG iIndex, // Specify CH367 device index
    ULONG iIntLine, // Specify interrupt number, 0 to disable interrupt, -1 to automatically detect and set
    UCHAR iInterruptMode); // Bit 0: Interrupt input polarity 1: high level/falling edge; 0: low level/rising edge; Bit 1: Interrupt input type 1: edge, 0: level

BOOL WINAPI CH367mWaitInterrupt( // Wait for interrupt event
    ULONG iIndex); // Specify CH367 device index

BOOL WINAPI CH367mAbortWaitInt( // Abort waiting for interrupt
    ULONG iIndex); // Specify CH367 device index

BOOL WINAPI CH367mSetIntRoutine( // Set interrupt service routine
    ULONG iIndex, // Specify CH367 device index
    mPCH367_INT_ROUTINE iIntRoutine); // Specify interrupt service routine, NULL to cancel interrupt service, otherwise call this routine on interrupt

BOOL WINAPI CH367mReadIntCommand( // Read interrupt command buffer
    ULONG iIndex, // Specify CH367 device index
    mPWIN32_COMMAND oCommand, // Point to a sufficiently large buffer to save the read command structure
    ULONG iCmdIndex); // Interrupt command index, 1 or 2

BOOL WINAPI CH367mWriteIntCommand( // Write interrupt command buffer
    ULONG iIndex, // Specify CH367 device index
    mPWIN32_COMMAND iCommand, // Point to the command structure as the interrupt command
    ULONG iCmdIndex); // Interrupt command index, 1 or 2

BOOL WINAPI CH367mReadIoByte( // Read one byte from I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367mReadIoWord( // Read one word from I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    PUSHORT oWord); // Point to a word unit to save the read word data

BOOL WINAPI CH367mReadIoDword( // Read one double word from I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    PULONG oDword); // Point to a double word unit to save the read double word data

BOOL WINAPI CH367mWriteIoByte( // Write one byte to I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH367mWriteIoWord( // Write one word to I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    USHORT iWord); // Word data to be written

BOOL WINAPI CH367mWriteIoDword( // Write one double word to I/O port
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify I/O port address
    ULONG iDword); // Double word data to be written

BOOL WINAPI CH368mReadMemByte( // Read one byte from memory
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify memory address, virtual address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH368mReadMemDword( // Read one double word from memory
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify memory address, virtual address
    PULONG oDword); // Point to a double word unit to save the read double word data

BOOL WINAPI CH368mWriteMemByte( // Write one byte to memory
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify memory address, virtual address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH368mWriteMemDword( // Write one double word to memory
    ULONG iIndex, // Specify CH367 device index
    PVOID iAddr, // Specify memory address, virtual address
    ULONG iDword); // Double word data to be written

BOOL WINAPI CH367mReadConfig( // Read one byte data from configuration space
    ULONG iIndex, // Specify CH367 device index
    PVOID iOffset, // Specify offset address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367mWriteConfig( // Write one byte data to configuration space
    ULONG iIndex, // Specify CH367 device index
    PVOID iOffset, // Specify offset address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH368mSetA15_A8( // Set high address, i.e., set A15-A8 output
    ULONG iIndex, // Specify CH367 device index
    UCHAR iA15_A8); // Bit 7-0 corresponds to A15-A8 output, 1: high level, 0: low level

BOOL WINAPI CH367mReadI2C( // Read one byte data from I2C interface
    ULONG iIndex, // Specify CH367 device index
    UCHAR iDevice, // Lower 7 bits specify I2C device address
    UCHAR iAddr, // Specify data unit address
    PUCHAR oByte); // Point to a byte unit to save the read byte data

BOOL WINAPI CH367mWriteI2C( // Write one byte data to I2C interface
    ULONG iIndex, // Specify CH367 device index
    UCHAR iDevice, // Lower 7 bits specify I2C device address
    UCHAR iAddr, // Specify data unit address
    UCHAR iByte); // Byte data to be written

BOOL WINAPI CH367DelayUS( // Delay specified microseconds
    ULONG iDelay); // Specify the number of microseconds to delay, if less than 10, the error is large

BOOL WINAPI CH367mAccessBlock( // Read or write data block
    ULONG iIndex, // Specify CH367 device index
    ULONG iAccessMode, // Refer to the following description
    PVOID iAddr, // Specify I/O port or memory address
    PVOID ioBuffer, // Point to input or output buffer
    ULONG iLength); // Data block length, i.e., the number of bytes to read or write, length not exceeding 32KB

// iAccessMode definitions
// #define mFuncReadIoByte 0x00000010 // Read I/O port, continuously read at the same port address in byte units, maximum length 32KB
// #define mFuncReadIoWord 0x00000011 // Read I/O port, continuously read at the same port address in word units, maximum length 32KB
// #define mFuncReadIoDword 0x00000012 // Read I/O port, continuously read at the same port address in double word units, maximum length 32KB
// #define mFuncReadIoBlock 0x00000013 // Read I/O port, increment port address by one after each byte read, maximum length 232B (read from offset address 00H to E7H)
// #define mFuncReadIo32Block 0x0000001f // Read I/O port, increment port address by one/two/four after each four-byte read, maximum length 232B (read from offset address 00H to E7H)
// #define mFuncWriteIoByte 0x00000014 // Write I/O port, continuously write at the same port address in byte units, maximum length 32KB
// #define mFuncWriteIoWord 0x00000015 // Write I/O port, continuously write at the same port address in word units, maximum length 32KB
// #define mFuncWriteIoDword 0x00000016 // Write I/O port, continuously write at the same port address in double word units, maximum length 32KB
// #define mFuncWriteIoBlock 0x00000017 // Write I/O port, increment port address by one after each byte write, maximum length 232B (write from offset address 00H to E7H)
// #define mFuncWriteIo32Block 0x00000020 // Write I/O port, increment port address by one/two/four after each four-byte write, maximum length 232B (write from offset address 00H to E7H)
// #define mFuncReadMemByte 0x00000018 // Read memory/register, continuously read in byte units, increment address by one after each byte read, maximum length 32KB (read from offset address 0000H to 7FFFH)
// #define mFuncReadMemDword 0x0000001a // Read memory/register, continuously read in double word units, increment address by four after each byte read, maximum length 32KB (read from offset address 0000H to 7FFFH)
// #define mFuncWriteMemByte 0x0000001c // Write memory/register, continuously write in byte units, increment address by one after each byte write, maximum length 32KB (write from offset address 0000H to 7FFFH)
// #define mFuncWriteMemDword 0x0000001e // Write memory/register, continuously write in double word units, increment address by four after each double word write, maximum length 32KB (write from offset address 0000H to 7FFFH)
// Other modes are not supported, the above definitions are in the file CH367DLL.H

BOOL WINAPI CH367mSetExclusive( // Set exclusive use of current CH367 device
    ULONG iIndex, // Specify CH367 device index
    ULONG iExclusive); // 0: device can be shared, non-zero: exclusive use

BOOL WINAPI CH367SetStream( // Set serial stream mode
    ULONG iIndex, // Specify CH367 device index
    ULONG iMode); // Specify mode, see below
// Bit 0: SPI clock frequency, 0=31.3MHZ, 1=15.6MHZ
// Bit 1: SPI I/O number/IO pin, 0=SPI3(SCS/SCL/SDX), 1=SPI4(SCS/SCL/SDX/SDI)
// Others reserved, must be 0

BOOL WINAPI CH367StreamSPI( // Process SPI data stream, 3-wire interface, clock line is SCL pin, output and input pin is SDX, built-in pull-up resistor pin, can be connected to serial FlashROM chip 25FXX SDO/SDI pin; chip select line is SCS
                             // 4-wire interface, input pin is SDI, output pin is SDX, clock line is SCL pin, chip select line is SDA pin
                                 /* SPI timing: SCL pin is clock output, default is low level, output during low level before clock rising edge, input during high level before clock falling edge */
                ULONG iIndex, // Specify CH367 device index
                ULONG iWriteLength, // Write length, length to be written
                PVOID iWriteBuffer, // Point to a buffer, place the data to be written
                PULONG oReadLength, // Point to length unit, return the actual read length
                PVOID oReadBuffer); // Point to a sufficiently large buffer to save the read data

BOOL WINAPI CH364mLockFlash(
        ULONG iIndex, // Specify CH367 device index
        UCHAR WRLock); // Lock mode (00H to disable write protection, 8CH to allow write protection/read-only lock, 0FFH to read/write protection/full lock, can only be unlocked after reset)

BOOL WINAPI CH364mReadFlash(
        ULONG iIndex, // Specify CH367 device index
        ULONG iAddr, // Specify FLASH address
        PVOID oBuffer, // Point to a buffer to save the read byte data
        ULONG iLength); // Length of data to read

BOOL WINAPI CH364mEarseFlash(
        ULONG iIndex, // Specify CH367 device index
        ULONG iAddr, // Specify FLASH address
        ULONG iLength); // Specify erase length, 4096 bytes (4K) per sector, minimum erase unit is sector

BOOL WINAPI CH364mWriteFlash(
        ULONG iIndex, // Specify CH367 device index
        ULONG iAddr, // Specify I/O port address, this address can be IO offset address (<=0xE7) and absolute IO address
        PVOID iBuffer, // Point to a byte unit to save the read byte data
        ULONG iLength);

BOOL WINAPI CH367mRestoreIOMEM(
        ULONG iIndex); // Specify CH367 device index

#ifdef __cplusplus
}
#endif

#endif // _CH367_DLL_H