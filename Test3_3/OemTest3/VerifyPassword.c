//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Core\em\AMITSE\Inc\PwdLib.h>
#include <Include\Uefi\UefiBaseType.h>
#include <Include\Setup.h>
#include <Include\Library\UefiRuntimeServicesTableLib.h>
#include <Include\Uefi\UefiSpec.h>
#include <Include\AmiLib.h>
#include <Core\em\AMITSE\Inc\TseCommon.h>
#include <Core\em\AMITSE\Inc\mem.h>
#include <Core\em\AMITSE\Inc\variable.h>

const UINTN TsePasswordVariableLength = SETUP_PASSWORD_LENGTH;

UINT32 PasswordCheckInstalledLocal(VOID);
UINT32 PasswordCheckInstalled( VOID )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    EFI_GUID                SetupGuid = SETUP_GUID;
    SETUP_DATA              SetupData;
    UINT32                  Attributes = 0;
    UINTN                   VariableSize = sizeof(SETUP_DATA);
        
    Status = gRT->GetVariable(L"Setup",\
                            &SetupGuid,\
                            &Attributes,\
                            &VariableSize,\
                            &SetupData );
    
    UINT32  Installed = AMI_PASSWORD_NONE;
    Installed = PasswordCheckInstalledLocal();
    
    if(Installed == AMI_PASSWORD_NONE)
        return Installed;
    if(Installed == AMI_PASSWORD_USER && SetupData.OemUserOenof == 1){
        return Installed;
    }else if(Installed == AMI_PASSWORD_USER && SetupData.OemUserOenof == 0){
        Installed = AMI_PASSWORD_NONE;
    }
    
    if(Installed == AMI_PASSWORD_ADMIN && SetupData.OemAdministratorOenof == 1){
        return Installed;
    }else if(Installed == AMI_PASSWORD_ADMIN && SetupData.OemAdministratorOenof == 0){
        Installed = AMI_PASSWORD_NONE;
    }
    
    if(Installed == 3){
        if(SetupData.OemUserOenof && SetupData.OemAdministratorOenof)
            return Installed;
        if(!SetupData.OemUserOenof && SetupData.OemAdministratorOenof){
            Installed = AMI_PASSWORD_ADMIN;
        }
        if(SetupData.OemUserOenof && !SetupData.OemAdministratorOenof){
            Installed = AMI_PASSWORD_USER;
        }
        if(!SetupData.OemUserOenof && !SetupData.OemAdministratorOenof){
            Installed = AMI_PASSWORD_NONE;
        }
    }
    return Installed;
    
}
