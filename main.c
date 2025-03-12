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
//	CH368mWriteMemDword(0, memReg + 0xFB, 0b10101010101010101010101010101010);  
	mPCH367_IO_REG ioAddr;
	CH367mGetIoBaseAddr(0, &ioAddr);
	printf("io base address: %lu\n", (unsigned long)ioAddr);
	UCHAR b = 0xfe;
	for(int i = 0; i < 100; i++)
	{
		CH367mWriteIoByte(0, /*&ioAddr +*/ 0, b);
//		CH367mWriteIoByte(0, &ioAddr->mCH367GPOR, 0xff);
		b = (b << 1) | (b >> 7);
		CH367DelayUS(100000);
	}

	CH367mCloseDevice(0);
	return 0;
}