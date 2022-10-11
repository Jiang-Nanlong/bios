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

extern EFI_BOOT_SERVICES    *pBS;
UINT8 ReadCmosProtocol(IN UINT8 Address){
    UINT8 value;
    IoWrite8(0x70, Address);
    value = IoRead8(0x71);
    return value;
} 
UINT8 WriteCmosProtocol(IN UINT8 Address,IN UINT8 Parameter){
    IoWrite8(0x70, Address);
    IoWrite8(0x71, Parameter);
    return ReadCmosProtocol(Address);
}

static READ_AND_WRITE_CMOS_PROTOCOL WriteAndReadCmosProtocol = {
    ReadCmosProtocol,
    WriteCmosProtocol
};

EFI_STATUS InstallProtocolsEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS Status;
    InitAmiLib(ImageHandle,SystemTable);
    Status = pBS->InstallProtocolInterface(&ImageHandle,&gRwCmosProtocolGuid,0,&WriteAndReadCmosProtocol);
    ASSERT_EFI_ERROR (Status);
	
	return EFI_SUCCESS;
}

