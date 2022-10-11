//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>

extern EFI_SYSTEM_TABLE     *gST;
extern EFI_BOOT_SERVICES      *gBS;

VOID PressKeyToContinue()
{
    UINTN   size = 0;
    EFI_INPUT_KEY Key = {0};
    
	if((gST != NULL) && (gST->ConOut != NULL))
	    gST->ConOut->OutputString(gST->ConOut, L"Press any key to continue.");
	
	while(1){
        gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
        gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
        if((Key.UnicodeChar != 0) || (Key.ScanCode != 0))
            break;
	}
}

