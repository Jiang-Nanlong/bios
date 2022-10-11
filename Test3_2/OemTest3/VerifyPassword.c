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

const UINTN TsePasswordVariableLength = 0x14;

#define TSE_STRUCT_OFFSET(type, field)  (UINTN)&(((type *)0)->field)

VOID GetAMITSEVariableLocal1(AMITSESETUP **mSysConf, UINT8 **setup, UINTN *VarSize)
{
    VarUpdateVariable(VARIABLE_ID_AMITSESETUP);
    *mSysConf = VarGetVariable( VARIABLE_ID_AMITSESETUP, VarSize );
    if ( ( *mSysConf == NULL ) || ( *VarSize != sizeof(AMITSESETUP) ) )
        *mSysConf = VarGetNvram( VARIABLE_ID_AMITSESETUP, VarSize );
}


UINT32 PasswordCheckInstalled( VOID )
{
    UINT32  Installed = AMI_PASSWORD_NONE;
    UINT8 *setup = NULL;
    AMITSESETUP *mSysConf = NULL;
    UINTN VarSize = 0;
    CHAR16 *TestPassword;
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
        
    
    TestPassword = EfiLibAllocateZeroPool((TsePasswordVariableLength + 1)*sizeof(CHAR16));

    GetAMITSEVariableLocal1(&mSysConf,&setup,&VarSize);

    if ( ( mSysConf != NULL ) && ( VarSize == sizeof(AMITSESETUP) ) )
    {
        MemSet( TestPassword, (TsePasswordVariableLength + 1)*sizeof(CHAR16), 0 );
        if ( ! EfiCompareMem( TestPassword, ((UINT8*)(mSysConf))+ TSE_STRUCT_OFFSET( AMITSESETUP, UserPassword ), TsePasswordVariableLength * sizeof(CHAR16) ) ||
            (SetupData.OemUserOenof == 0)
           )
            Installed |= AMI_PASSWORD_USER;
        if ( ! EfiCompareMem( TestPassword, ((UINT8*)(mSysConf))+ TSE_STRUCT_OFFSET( AMITSESETUP, AdminPassword ), TsePasswordVariableLength * sizeof(CHAR16) ) ||
            (SetupData.OemAdministratorOenof == 0)
           )
            Installed |= AMI_PASSWORD_ADMIN;
        Installed ^= AMI_PASSWORD_ANY;
    }

    if(setup)
        MemFreePointer( (VOID **)&setup );
    else
        MemFreePointer( (VOID **)&mSysConf );

    StringZeroFreeMemory( (VOID **)&TestPassword);

    return Installed;
}
