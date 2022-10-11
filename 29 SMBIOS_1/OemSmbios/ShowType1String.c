//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#include <AmiDxeLib.h>
#include <Include\Uefi\UefiInternalFormRepresentation.h>
#include <Include\Protocol\Smbios.h>
#include <Include\IndustryStandard\SmBios.h>
#include <Include\Uefi\UefiSpec.h>
#include <Include\Setup.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\DebugLib.h>
#include <Include\Library\MemoryAllocationLib.h>
#include <HddSecurity\SmmHddSecurity.h>
// read the continuous segment of characters
/*
CHAR8* GetString(
    OUT CHAR8 *Ptr,
    OUT CHAR8 *String
)
{
    String = (CHAR8 *)Malloc(sizeof(CHAR8) * 20);
    UINTN i = 0;
    while(i<20){
        if(*Ptr == 0x0){
            Ptr ++;
            String[i] = '\0';
            return String;
        }else{
            String[i] = *Ptr;
            i++;
            Ptr ++;
        }
    }
}
*/


EFI_STATUS
GetSmBiosInfoByIndex (
  IN   CHAR8                   *OptionalStrStart,
  IN   UINT8                   Index,
  OUT  CHAR8                   **String
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    *String = AllocateZeroPool (sizeof (CHAR8));
    return EFI_SUCCESS;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart);
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  *String = AllocatePool (StrSize);
  ASSERT (*String != NULL);
  if (*String != NULL) {
    CopyMem (*String, OptionalStrStart, StrSize);
  }
  return EFI_SUCCESS;
}


VOID ShowType1String(IN EFI_HII_HANDLE HiiHandle)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_SMBIOS_PROTOCOL         *gEfiSmbiosProtocol = NULL;
    EFI_SMBIOS_HANDLE           SmbiosHandle;
    EFI_SMBIOS_TYPE             Type = 1;
    EFI_SMBIOS_TABLE_HEADER     *Record = NULL;
    SMBIOS_TABLE_TYPE1          *Type1Record = NULL;
    UINT8                       Length = 0;
    UINT8                       *Ptr = NULL;
    CHAR8                       *String = NULL;
    UINT8                       StrIndex;

    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID **)&gEfiSmbiosProtocol);
    
    if(!EFI_ERROR(Status)){
        // Read type1 structure
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = gEfiSmbiosProtocol->GetNext (gEfiSmbiosProtocol, &SmbiosHandle, &Type, &Record, NULL);
        Type1Record = (SMBIOS_TABLE_TYPE1 *) Record;   //Data section address
        Length = Type1Record->Hdr.Length;
        Ptr = (UINT8 *)Type1Record + Length;
        
        /*GetString((CHAR8*)Ptr,String);
        DEBUG((-1,"Manufacturer:%s\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE1), L"%a%", String);*/
        
        StrIndex = Type1Record->Manufacturer;   //Manufacturer
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"Manufacturer:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE1), L"%a", String);
        FreePool (String);
        
        StrIndex = Type1Record->ProductName;    //ProductName
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"ProductName:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE2), L"%a", String);
        
        StrIndex = Type1Record->Version;        //Version
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"Version:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE3), L"%a", String);
        FreePool (String);

        StrIndex = Type1Record->SerialNumber;    //SerialNumber
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"SerialNumber:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE4), L"%a", String);
        FreePool (String);

        StrIndex = Type1Record->SKUNumber;      //SKUNumber
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"SKUNumber:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE5), L"%a", String);
        FreePool (String);

        StrIndex = Type1Record->Family;         //Family
        GetSmBiosInfoByIndex((CHAR8*)Ptr, StrIndex, &String);
        DEBUG((-1,"Family:%a\n",String));
        InitString(HiiHandle, STRING_TOKEN(STR_OEM_TEXT_VALUE6), L"%a", String);
        FreePool (String);
    }
}

