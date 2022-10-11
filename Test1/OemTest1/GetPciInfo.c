//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiDxeLib.h>
#include <Library\IoLib.h>
#include <Library\DebugLib.h>
#include <Library\PciLib.h>
#include <Include\Uefi\UefiBaseType.h>

UINT8 ReadCmos(IN UINT8 Address){       //read a byte in cmos
    UINT8 value;
    IoWrite8(0x70, Address);
    value = IoRead8(0x71);
    return value;
}
UINT8 WriteCmos(IN UINT8 Address,IN UINT8 Parameter){   //write a byte into cmos
    IoWrite8(0x70, Address);
    IoWrite8(0x71, Parameter);
    return ReadCmos(Address);
}

EFI_STATUS GetPciInfoEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINT8                   bus = 0;
    UINT8                   dev = 0x17;
    UINT8                   func = 0;
    UINT8                   count = 0x2c;
    UINT32                  dwData0 = 0;    //get PCI device 0,17,0,0x2c-0x2f
    UINT8                   dwData1 = 0;    //get CMOS info
    UINT8                   offset = 0x51;
    UINT8                   i = 0;
    
	InitAmiLib(ImageHandle,SystemTable);
    
	dwData0 = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,count));   //read PCI device 0,17,0,0x2c-0x2f	
    DEBUG((-1,"dwData0: %x\n",dwData0));

    DEBUG((-1,"original CMOS 0x51-0x54 data: "));
    for(i = 0;i<4;i++){
        dwData1 = ReadCmos(offset+i);
        DEBUG((-1,"%x ",dwData1));
    }
    
    DEBUG((-1,"\nCMOS 0x51-0x54 data after modify:"));
    for(i = 0;i<4;i++){
        dwData1 = WriteCmos(offset+i, (dwData0 >> (i*8)) & 0xFF);
        DEBUG((-1,"%x ", dwData1));
    }
    DEBUG((-1,"\n"));
	return EFI_SUCCESS;
}

