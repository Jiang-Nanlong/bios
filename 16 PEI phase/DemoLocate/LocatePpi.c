//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiPeiLib.h>
#include <Ppi/ppi.h>
#include <Library/IoLib.h>
#include <Ppi/Stall.h>
#include <Library/DebugLib.h>

EFI_STATUS LocatePpiEntryPoint(
        IN EFI_PEI_FILE_HANDLE FileHandle,
        IN CONST EFI_PEI_SERVICES **PeiServices
)
{
    UINT8 value;
	EFI_STATUS Status;
	WRITE_AND_READ_COMS_PPI *WriteAndRead;
	Status = (*PeiServices)->LocatePpi(PeiServices,&gWriteAndReadCmosGuid,0,NULL,&WriteAndRead);
	if(Status != EFI_SUCCESS)
	    return Status;
	value = WriteAndRead->ReadCmos(0x40);
	DEBUG((-1,"The content before modification is as follows:%x\n",value));
	WriteAndRead->WriteCmos(0x40,0xAA);
	value = WriteAndRead->ReadCmos(0x40);
	DEBUG((-1,"The content after modification is as follows:%x\n",value));
	
	return EFI_SUCCESS;
}

