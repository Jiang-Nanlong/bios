//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Include/Library/OemGetTimeLib.h>
#include <Include/Uefi/UefiBaseType.h>
#include <Include/Protocol/SimpleTextIn.h>
#include <Include/Protocol/SimpleTextOut.h>
#include <Include/Library/UefiBootServicesTableLib.h>
#include <Include/Library/UefiRuntimeServicesTableLib.h>

VOID EFIAPI GetSystemTime()
{
    EFI_TIME                OemTime;
    CHAR16                  buffer[100];
    UINTN                   size = 0;
    EFI_INPUT_KEY           Key = {0};
    
    gST->ConOut->OutputString(gST->ConOut, L"system time:\n");
    gRT->GetTime(&OemTime,NULL);
    Swprintf(buffer, L"%d/%d/%d   %d:%d:%d\n \r",\
                                OemTime.Year,\
                                OemTime.Month,\
                                OemTime.Day,\
                                OemTime.Hour,\
                                OemTime.Minute,\
                                OemTime.Second);
    gST->ConOut->OutputString(gST->ConOut, buffer);     //Output time to screen
    
    gST->ConOut->OutputString(gST->ConOut, L"Press any key to exit...\n");      //Press any key to exit
    gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    if((Key.UnicodeChar != 0) || (Key.ScanCode != 0)) return;
}
