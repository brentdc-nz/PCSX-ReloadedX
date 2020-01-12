#ifndef CXBOXATA_H
#define CXBOXATA_H

#include <xtl.h>
#include "undocumented.h"

#define XBOX_DEVICE_HDD    0
#define XBOX_DEVICE_DVDROM 1

// Our SendATACommand needs this to figure our if
// we should read or write data to IDE registers
#define IDE_COMMAND_READ          0x00
#define IDE_COMMAND_WRITE         0x01
#define IDE_COMMAND_NONDATA       0x02

// Important ATA/ATAPI Commands As per ATA Spec
#define ATA_IDENTIFY_PACKET_DEVICE    0xA1
#define ATA_IDENTIFY_DEVICE           0xEC
#define ATA_READ_SECTORS              0x20
#define ATA_READ_BUFFER               0xE4
#define ATA_WRITE_SECTORS             0x30
#define ATA_WRITE_BUFFER              0xE8
#define ATA_EXECUTE_DEVICE_DIAGNOSTIC 0x90
#define ATA_COMMAND_POWERMODE1        0xE5
#define ATA_COMMAND_STANDBY           0xE0
#define ATA_COMMAND_ACTIVE            0xE1
#define ATA_COMMAND_SET_FEATURES      0xEF

// ATA IDENTIFY Structure offsets..
// As per ATA Spec
#define HDD_CONFIG                  0x000 // 0
#define HDD_NUM_OF_CYLS_OFFSET      0x002 // 2
#define HDD_NUM_OF_HEADS_OFFSET     0x006 // 6
#define HDD_BYTE_PERTRACK_OFFSET    0x008 // 8
#define HDD_BYTE_PERSECTOR_OFFSET   0x00a // 10
#define HDD_SECTOR_PERTRACK_OFFSET  0x00c // 12
#define HDD_VENDOR_SPEC_1_OFFSET    0x00e // 14
#define HDD_SERIAL_OFFSET           0x014 // 20  ->ASCII
#define HDD_BUFFER_TYPE_OFFSET      0x028 // 40
#define HDD_BUFFER_SIZE_OFFSET      0x02A // 42
#define HDD_NUM_OF_ECC_BYTE_OFFSET  0x02c // 44
#define IDE_FIRMWARE_OFFSET         0x02e // 46 ->ASCII
#define HDD_MODEL_OFFSET            0x036 // 54 ->ASCII
#define HDD_MUL_SEC_PER_INT_OFFSET  0x05E // 94
#define HDD_DWIO_OFFSET             0x060 // 96
#define HDD_LBADMA_OFFSET           0x062 // 98
#define HDD_PIO_TIMING_MODE_OFFSET  0x066 // 102
#define HDD_DMA_TIMING_MODE_OFFSET  0x068 // 104
#define HDD_AP_NUM_OF_CYLS_OFFSET   0x06C // 108
#define HDD_AP_NUM_OF_HEADS_OFFSET  0x06E // 110
#define HDD_AP_SEC_PER_TRACK_OFFSET 0x070 // 112
#define HDD_CAPACITY_OFFSET         0x072 // 114
#define HDD_NUM_SEC_PER_INT_OFFSET  0x076 // 118
#define HDD_LBA_SECTORS_OFFSET      0x078 // 120
#define HDD_SIN_DMA_MODES_OFFSET    0x07C // 124
#define HDD_MUL_DMA_MODES_OFFSET    0x07E // 126
#define HDD_CMD_SUPPORT_OFFSET      0x0A4 // 164
#define HDD_CMD_ENABLED_OFFSET      0x0AA // 170
#define HDD_SECURITY_STATUS_OFFSET  0x100 // 256
#define HDD_RESERVED_5_OFFSET       0x140 // 320

#define HDD_FIRMWARE_LENGTH 0x08
#define HDD_SERIAL_LENGTH   0x14
#define HDD_MODEL_LENGTH    0x28

class CXboxATA
{
public:
	CXboxATA();
	~CXboxATA();

	//IDE ATA Input Registers Structure
	typedef struct IP_IDE_REG
	{
		BYTE bFeaturesReg;      // Used for specifying SMART "commands".
		BYTE bSectorCountReg;   // IDE sector count register
		BYTE bSectorNumberReg;  // IDE sector number register
		BYTE bCylLowReg;        // IDE low order cylinder value
		BYTE bCylHighReg;       // IDE high order cylinder value
		BYTE bDriveHeadReg;     // IDE drive/head register
		BYTE bCommandReg;       // Actual IDE command.
		BYTE bReserved;         // reserved for future use.  Must be zero.
	};
	typedef IP_IDE_REG* LPIP_IDE_REG;

	typedef struct _IDEREG
	{
		BYTE bFeaturesReg;    // +0
		BYTE bSectorCountReg; // +1
		BYTE bSectorNumberReg;// +2
		BYTE bCylLowReg;      // +3
		BYTE bCylHighReg;     // +4
		BYTE bDriveHeadReg;   // +5
		BYTE bCommandReg;     // +6
		BYTE bHostSendsData;  // +7
	} IDEREG;

	//IDE ATA Output Registers Structure
	typedef struct OP_IDE_REG
	{
		BYTE bErrorReg;
		BYTE bSectorCountReg;
		BYTE bSectorNumberReg;
		BYTE bCylLowReg;
		BYTE bCylHighReg;
		BYTE bDriveHeadReg;
		BYTE bStatusReg;
	};
	typedef OP_IDE_REG* LPOP_IDE_REG;

	typedef struct ATA_PASS_THROUGH
	{
		IDEREG IdeReg;
		ULONG  DataBufferSize;// +8
		BYTE * DataBuffer;    // +0c
	};
	typedef ATA_PASS_THROUGH* LPATA_PASS_THROUGH;

	// Our own object for issuing commands..
	// Includes in/ou register objects and 1 Sector of HDD Data
	typedef struct ATA_COMMAND_OBJ
	{
		IP_IDE_REG	IPReg;
		OP_IDE_REG	OPReg;
		BYTE		DATA_BUFFER[512];
		ULONG		DATA_BUFFSIZE;
	};
	typedef ATA_COMMAND_OBJ* LPATA_COMMAND_OBJ;

	static bool SendATACommand(UCHAR DeviceNum, LPATA_COMMAND_OBJ ATACommandObj, UCHAR ReadWrite);
	static void	GetIDEModel(UCHAR* IDEData, LPSTR ModelString);

private:
	static void CleanATAData(unsigned char *dst, unsigned char *src, int len, BOOL bClean = true);
};

#endif // CXBOXATA_H