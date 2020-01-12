#include "xboxata.h"

CXboxATA::CXboxATA()
{
}

CXboxATA::~CXboxATA()
{
}

bool CXboxATA::SendATACommand(UCHAR DeviceNum, LPATA_COMMAND_OBJ ATACommandObj, UCHAR ReadWrite)
{
	bool bRetVal = false;

	ANSI_STRING tmp;
	if (DeviceNum == XBOX_DEVICE_HDD)
		RtlInitAnsiString(&tmp, "\\Device\\Harddisk0\\Partition0");
	else if (DeviceNum == XBOX_DEVICE_DVDROM)
		RtlInitAnsiString(&tmp, "\\Device\\Cdrom0");
	else
		return false;

	HANDLE Device;
	IO_STATUS_BLOCK iosb;
	OBJECT_ATTRIBUTES oa;
	NTSTATUS status;

	ZeroMemory(&oa, sizeof(OBJECT_ATTRIBUTES));
	oa.ObjectName = &tmp;
	oa.Attributes = 0x40; // Case insensitive

	status = NtOpenFile(&Device, SYNCHRONIZE | GENERIC_READ | GENERIC_WRITE, &oa, &iosb, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SYNCHRONOUS_IO_ALERT);

	if(status < 0) // Check for success
		return false;

	unsigned int Size = sizeof(ATA_PASS_THROUGH);

	LPATA_PASS_THROUGH pAPT = (LPATA_PASS_THROUGH) VirtualAlloc(NULL, Size, MEM_COMMIT, PAGE_READWRITE);
	ZeroMemory (pAPT, Size);

	if(pAPT)
	{
		memset(pAPT, 0, Size);
		pAPT->DataBufferSize          = ATACommandObj->DATA_BUFFSIZE;
		pAPT->IdeReg.bFeaturesReg     = ATACommandObj->IPReg.bFeaturesReg;
		pAPT->IdeReg.bSectorCountReg  = ATACommandObj->IPReg.bSectorCountReg;
		pAPT->IdeReg.bSectorNumberReg = ATACommandObj->IPReg.bSectorNumberReg;
		pAPT->IdeReg.bCylLowReg       = ATACommandObj->IPReg.bCylLowReg;
		pAPT->IdeReg.bCylHighReg      = ATACommandObj->IPReg.bCylHighReg;
		pAPT->IdeReg.bDriveHeadReg    = ATACommandObj->IPReg.bDriveHeadReg;
		pAPT->IdeReg.bCommandReg      = ATACommandObj->IPReg.bCommandReg;

		pAPT->DataBuffer = (BYTE *) &ATACommandObj->DATA_BUFFER;
		pAPT->IdeReg.bHostSendsData = ReadWrite == IDE_COMMAND_WRITE ? TRUE : FALSE;

		DWORD BytesReturned = 0;

		// Send the ATA/IDE Pass through command..
		BOOL Status = DeviceIoControl(Device, IOCTL_IDE_PASS_THROUGH, pAPT, Size, pAPT, Size, &BytesReturned, FALSE);
		
		// Get the Error and Status registers of IDE command return
		ATACommandObj->OPReg.bErrorReg =  pAPT->IdeReg.bFeaturesReg;
		ATACommandObj->OPReg.bStatusReg = pAPT->IdeReg.bCommandReg;

		// If the command was successful, copy the ATA structure's data into the ouptut object
		if(Status)
			bRetVal = true;
	}
	else
		bRetVal = false;

#if 0
	// On the Xbox, if this is the DVD-ROM device, make sure we poll the DVD-ROM device for readiness
	// after the command is sent.
	if (DeviceNum == XBOX_DEVICE_DVDROM)
	{
		for (int errors = 0; errors < 5; errors++)
		{
			if (DeviceIoControl(Device, IOCTL_CDROM_CHECK_VERIFY, 0, 0, 0, 0, 0, 0))
				break;
	
			Sleep(200);
		}
	}
#endif

	VirtualFree(pAPT, Size, MEM_RELEASE);
	CloseHandle(Device);

	return bRetVal;
}

void CXboxATA::GetIDEModel(UCHAR* IDEData, LPSTR ModelString)
{
	CleanATAData((UCHAR*)ModelString, IDEData+HDD_MODEL_OFFSET, HDD_MODEL_LENGTH);
}

void CXboxATA::CleanATAData(unsigned char *dst, unsigned char *src, int len, BOOL bClean)
{
	// Byte swap the Data that comes back from ATA command..  and clean out blanks etc.
	unsigned char tmp;
	int i;

	for(i=0; i < len; i+=2) 
	{
		tmp = src[i];		
		dst[i] = src[i+1];
		dst[i+1] = tmp;
	}
	
	for(i=len-1; i>0; --i) 
		if(dst[i] != ' ') break;

	if(bClean)
		dst[i + 1] = 0;
}