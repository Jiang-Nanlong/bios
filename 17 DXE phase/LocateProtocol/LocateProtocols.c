//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <DemoProtocol.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
EFI_STATUS LocateProtocolsEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINT8 value;
    EFI_STATUS Status;
    InitAmiLib(ImageHandle,SystemTable);
    READ_AND_WRITE_CMOS_PROTOCOL *WriteAndReadProtocol;
    Status = pBS->LocateProtocol(&gRwCmosProtocolGuid,NULL,&WriteAndReadProtocol);
    if(Status != EFI_SUCCESS)
        return Status;
    value = WriteAndReadProtocol->Read(0x41);
    DEBUG((-1,"The content before modification is as follows:%x\n",value));
    WriteAndReadProtocol->Write(0x41,0xAA);
    value = WriteAndReadProtocol->Read(0x41);
    DEBUG((-1,"The content after modification is as follows:%x\n",value));
    
	return EFI_SUCCESS;
}

