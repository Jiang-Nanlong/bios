//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiDxeLib.h>
#include <Setup.h>
#include <AMIVfr.h>

EFI_STATUS SetLogoStatusEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                      Status=EFI_SUCCESS;
    EFI_GUID                        gAmiTseSetupGuid = AMITSESETUP_GUID;
    AMITSESETUP                     AmiTseData;
    UINT32                          Attributes= 0;
    UINTN                           VariableSize;
    VariableSize                    = sizeof(AMITSESETUP);
    
	InitAmiLib(ImageHandle,SystemTable);

	Status = pRS->GetVariable(L"AMITSESetup",\
                            &gAmiTseSetupGuid,\
                            &Attributes,\
                            &VariableSize,\
                            &AmiTseData );
	
	if(!EFI_ERROR(Status))
	{
        DEBUG((-1,"Logo Status:%x\n",AmiTseData.AMISilentBoot));
        
        AmiTseData.AMISilentBoot = !AmiTseData.AMISilentBoot;//Reverse
        
        Status = pRS->SetVariable(L"AMITSESetup",\
                                &gAmiTseSetupGuid,\
                                Attributes,\
                                VariableSize,\
                                &AmiTseData );
        if(EFI_ERROR(Status))
        {
            DEBUG((-1,"Logo status modify failure\n"));
        }else{
            DEBUG((-1,"Logo status modify success\n"));
        }
	}
	return Status;
}

