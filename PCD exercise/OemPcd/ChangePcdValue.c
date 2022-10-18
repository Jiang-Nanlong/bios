//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Library\PcdLib.h>

//The PCD type can be set to both PcdsDynamic and PcdsPatchableInModule. 
//If you set the type to PcdsPatchableInModule, you can see the corresponding value through AMIBCP.exe
EFI_STATUS ChangePcdValueEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINT64                  Temp = 0;
    EFI_STATUS              Status = EFI_SUCCESS;
	InitAmiLib(ImageHandle,SystemTable);
	
	Temp = PcdGet64 (PcdUartDefaultBaudRate);
	DEBUG((-1,"PcdUartDefaultBaudRate:%x\n",Temp));
	
	DEBUG((-1,"before change Pcd value:%x\n",PcdGet64 (OemPcdName)));
	Status = PcdSet64(OemPcdName,Temp);
	
	DEBUG((-1,"after change Pcd value:%x\n",PcdGet64 (OemPcdName)));
	
	return Status;
}

