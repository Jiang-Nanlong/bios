//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiDxeLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>

#define PCI_CONFIG_ADDRESS 0xcf8    //PCI address port
#define PCI_CONFIG_DATA 0xcfc       //PCI data port
/*
 * There are up to 256 PCI buses in the system,
 * each bus has a maximum of 32 devices, 
 * each device has up to 8 functions, 
 * and each function has a configuration space of 256 bytes,
 * so the total configuration space is 16M.
 * */

EFI_STATUS VerifyOpRomAvailableEntryPoint(
        IN EFI_HANDLE       ImageHandle,
            IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINT16 bus,dev,func,count = 0;
    InitAmiLib(ImageHandle, SystemTable);
    for(bus=0;bus<256;bus++)
    {
        for(dev=0;dev<32;dev++)
        {
            for(func=0;func<8;func++)
            {
                UINT32 dwData=0;
                UINT32 ExpansionRomBaseAddress=0;
                dwData = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,0x30));
                if(dwData)
                {
                    count++;
                    //ExpansionRomBaseAddress = (dwData>>11)&0x1FFFFF;
                    //get expansion ROM Base Address of the PCI configuration space
                    DEBUG((-1,"Expansion ROM Base Address:%x\n",dwData));
                }
                else
                {
                    DEBUG((-1,"The device's expansion ROM address space is disabled."));
                    DEBUG((-1,"%x\t%x\t%x\n",bus,dev,func));
                }
            }
        }
    }
    DEBUG((-1,"Find PCI Device %d",count));
    return EFI_SUCCESS;
}
