//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Library/PciLib.h>
#include <Library/UefiLib.h>
#include <Include/AmiLib.h>
#include <Include/Protocol/SimpleTextIn.h>
#include <Include/Protocol/SimpleTextOut.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

EFI_STATUS GetPciResourcesEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINTN                           bus,dev,func,count = 0;
    UINT32                          dwData0,dwData1,dwData2;
    UINT32                          ExpansionRom,ClassCode;
    UINT16                          VID,DID,RID,SVID,SID;
    EFI_INPUT_KEY                   Key = {0};
    UINTN                           size = 0;

    gST->ConOut->ClearScreen (gST->ConOut);
    gST->ConOut->OutputString(gST->ConOut, L"PCI configuration space resources:\n");
    
    for(bus=0;bus<256;bus++)
    {
        for(dev=0;dev<32;dev++)
        {
            for(func=0;func<8;func++)
            {
                dwData0 = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,0));
                if(dwData0!=0xFFFFFFFF)
                {
                    CHAR16                          buffer[256] = {0};
                    VID = dwData0 & 0xFFFF;
                    DID = dwData0 & 0xFFFF0000;
                    dwData1 = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,0x08));
                    ClassCode = dwData1 & 0xFFFFFF00;
                    RID = dwData1 & 0xFF;
                    dwData2 = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,0x2C));
                    SID = dwData2 & 0xFFFF0000;
                    SVID = dwData2 & 0xFFFF;
                    ExpansionRom = PciRead32(PCI_LIB_ADDRESS(bus,dev,func,0x30));
                    
                    Print(L"PCI device NO.%d\n\r"
                            "Device ID:%x           Vendor ID:%x\n\r"
                            "Class Code:%x    Revision ID:%x\n\r"
                            "SubSystem ID:%x        SubSystem Vendor ID:%x\n\r"
                            "Expansion ROM Base Address:%x\n\r"
                            "--------------------------------------\n\r",\
                              ++ count,\
                              DID,\
                              VID,\
                              ClassCode,\
                              RID,\
                              SID,\
                              SVID,\
                              ExpansionRom);
                }
            }
        }
    }
    
    gST->ConOut->OutputString(gST->ConOut, L"Press any key to exit...\n");      //Press any key to exit
    gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    if((Key.UnicodeChar != 0) || (Key.ScanCode != 0)) return EFI_SUCCESS;

	return EFI_SUCCESS;
}

