//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Include\Protocol\SmmSwDispatch2.h>
#include <Include\Uefi\UefiSpec.h>


#define SMG_SW_SMM_CALLBACK     0x11
EFI_STATUS SmgSwSmiCallback(
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer OPTIONAL,
    IN OUT UINTN        *CommBufferSize OPTIONAL
)
{
    DEBUG((-1,"SMI program triggered.\n"));
    return EFI_SUCCESS;
}

EFI_STATUS InSmmFunction(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_HANDLE                      SwHandle = NULL;
    EFI_SMM_SW_DISPATCH2_PROTOCOL   *SwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
    
    InitAmiLib(ImageHandle,SystemTable);

    Status = InitAmiSmmLib(ImageHandle,SystemTable);
    if(EFI_ERROR(Status)) return Status;
    Status = pSmst->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid,\
                                    NULL,\
                                    &SwDispatch);
    if(!EFI_ERROR(Status)){
        SwContext.SwSmiInputValue = SMG_SW_SMM_CALLBACK;
        Status = SwDispatch->Register(SwDispatch,\
                                    SmgSwSmiCallback,\
                                    &SwContext,\
                                    &SwHandle);
    }
    return Status;
    
}

EFI_STATUS SwSmiEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	InitAmiLib(ImageHandle,SystemTable);
    DEBUG((-1,"SwSmiEntryPoint\n"));
	return InitSmmHandler(ImageHandle,SystemTable,InSmmFunction,NULL);
}
