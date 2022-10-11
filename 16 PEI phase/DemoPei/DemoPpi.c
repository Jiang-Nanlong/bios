//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiPeiLib.h>
#include <ppi.h>
#include <Library/IoLib.h>
#include <Ppi/Stall.h>

//GUID Definition(s)
//EFI_GUID gWriteAndReadCmosGuid = WRITE_AND_READ_COMS_PPI_GUID;

UINT8 ReadCmos(IN UINT8 Address){
    UINT8 value;
    IoWrite8(0x70, Address);
    value = IoRead8(0x71);
    return value;
}
UINT8 WriteCmos(IN UINT8 Address,IN UINT8 Parameter){
    IoWrite8(0x70, Address);
    IoWrite8(0x71, Parameter);
    return ReadCmos(Address);
}
//PPI Definition(s)
static WRITE_AND_READ_COMS_PPI mWriteAndReadCmosPpi = {
        WriteCmos,
        ReadCmos
};

//PPI that are installed
static EFI_PEI_PPI_DESCRIPTOR mWriteAndReadPpi[] = {
        {(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),\
        &gWriteAndReadCmosGuid, &mWriteAndReadCmosPpi}
};
EFI_STATUS DemoPeiEntryPoint(
        IN EFI_PEI_FILE_HANDLE FileHandle,
        IN CONST EFI_PEI_SERVICES **PeiServices
)
{
	EFI_STATUS Status;  //install the clock generator PPI
	Status = (*PeiServices)->InstallPpi(PeiServices,&mWriteAndReadPpi[0]);
	ASSERT_PEI_ERROR(PeiServices,Status);
		
    return EFI_SUCCESS;
}

