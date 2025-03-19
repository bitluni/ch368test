#include <Windows.h>
#include "include/CH367DLL.H"
#include <stdio.h>

int main()
{
	printf("henlo!\n");
	HANDLE h = CH367mOpenDevice(0, FALSE, FALSE, 0); 
	if(h == INVALID_HANDLE_VALUE)
	{
		printf("can't find device\n");
		return -1;
	}
	printf("device found\n");
	unsigned long ver = CH367GetDrvVersion();
	printf("driver ver %lu\n", ver);
	mPCH368_MEM_REG memReg;
	CH368mGetMemBaseAddr(0, &memReg);
	printf("mem base address: %lu\n", (unsigned long)memReg); 
	mPCH367_IO_REG ioAddr;
	CH367mGetIoBaseAddr(0, &ioAddr);
	printf("io base address: %lu\n", (unsigned long)ioAddr);
	ULONG32 d = 1;
	CH367mWriteIoByte(0, 0xFA, 0b1000000);	//setting IO to 32bit (and fastest hold rate 40ns)
	for(int i = 0; i < 256*256*256; i++)
	{
		CH367mWriteIoDword(0, 0x0, 0xffffffff);
		CH367mWriteIoDword(0, 0x0, 0x0);
		CH367mWriteIoDword(0, 0x0, 0xffff0000);
		CH367mWriteIoDword(0, 0x0, 0x0);
	}

	CH367mCloseDevice(0);
	return 0;
}