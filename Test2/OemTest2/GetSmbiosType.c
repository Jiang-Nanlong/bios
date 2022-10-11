//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Include\Protocol\Smbios.h>
#include <Include\Uefi\UefiSpec.h>
#include <Include\IndustryStandard\SmBios.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\UefiLib.h>
#include <Library\UefiRuntimeServicesTableLib.h>

EFI_STATUS
GetSmBiosInfo (                         //read a type
    IN UINT8              *Ptr
)
{
    UINT8       i = 0;
    UINT8       Length = *(Ptr + 1);
    for(i=1;i <= Length;i++){
        Print(L"%2x  ",*Ptr);
        Ptr ++;
        if(i % 16 == 0)
            Print(L"\n");
    }
    
    while(1){
        if(*Ptr == 0 && *(Ptr+1) == 0){
            Print(L"00  ");
            i ++;
            if(i % 16 == 0)
                Print(L"\n");
            Print(L"00\n");
            break;
        }else{
            Print(L"%2x  ",*Ptr);
            Ptr ++;
            if(i % 16 == 0)
                Print(L"\n");
            i ++;
        }
    }
    Print(L"------------------------------\n");

  return EFI_SUCCESS;
}


EFI_STATUS GetSmbiosTypeEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_SMBIOS_TYPE             Type;       //shell scan
    EFI_SMBIOS_TABLE_HEADER     *Record = NULL;
    EFI_SMBIOS_PROTOCOL         *gEfiSmbiosProtocol = NULL;
    EFI_SMBIOS_HANDLE           SmbiosHandle;
    UINT8                       Length = 0;
    UINT8                       *Ptr = NULL;
    UINT8                       number = 0;
    UINTN                       size = 0;
    EFI_INPUT_KEY               Key = {0};
    UINT8                       sum = 0;
    gST->ConOut->ClearScreen (gST->ConOut);         //clear screen
    gST->ConOut->OutputString(gST->ConOut, L"enter a number of type(F1 exit):\n\r");

    while(1){
        gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
        gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
        if(Key.ScanCode != SCAN_F1){
            Print(L"%d",(UINT8)(Key.UnicodeChar - 48));
            sum = sum * 10 + (UINT8)(Key.UnicodeChar - 48);             //unicodechar into int
        }else{
            break;
        }
    }
    Print(L"\n");
    Type = sum;
    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID **)&gEfiSmbiosProtocol);
    if(!EFI_ERROR(Status)){
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        do{
            Status = gEfiSmbiosProtocol->GetNext (gEfiSmbiosProtocol, &SmbiosHandle, &Type, &Record, NULL);
            if(Status != EFI_NOT_FOUND){
                Ptr = (UINT8 *)Record;
                number ++;
                Print(L"SMBIOS type %d     number: %d\n",Type, number);
                GetSmBiosInfo(Ptr);
            }else {
                if(Ptr == NULL){                //not found
                    Print(L"This type is not supported.\n");
                }
                break;
            }
        }while(1);
        
    }
	return EFI_SUCCESS;
}

