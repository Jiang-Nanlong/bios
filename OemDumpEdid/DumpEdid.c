//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <AmiDxeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Include\Protocol\DevicePath.h>
#include <Include\Uefi\UefiSpec.h>
#include <Include\Uefi\UefiBaseType.h>
#include <Include\Protocol\SimpleTextIn.h>
#include <Include\Protocol\DevicePathToText.h>
#include <Include\Library\BaseMemoryLib.h>
#include <OemGuidList.h>
#include <Include\Protocol\EdidActive.h>
#include <CSM\CsmVideo\VesaBiosExtensions.h>
#include <Include\Protocol\EdidDiscovered.h>
#include <Include\Protocol\EdidOverride.h>
#include <Include\Base.h>
#include <Include\Library\UefiBootServicesTableLib.h>

EFI_STATUS DumpEdidEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                              Status = EFI_SUCCESS;
    EFI_EDID_DISCOVERED_PROTOCOL            *OemDiscoveredEdid = NULL;
    EFI_EDID_OVERRIDE_PROTOCOL              *OemOverrideEdid = NULL;
    EFI_EDID_ACTIVE_PROTOCOL                *OemActiveEdid = NULL;
    UINT8                                   *Ptr = NULL;
    VESA_BIOS_EXTENSIONS_EDID_DATA_BLOCK    *EdidDataBlock = NULL;
    UINT8                                   Name3,Name2,Name1;
    UINT8                                   i = 0;
    UINT32                                  HorizontalActivePixels = 0;
    UINT32                                  VerticalActivePixels = 0;
    UINTN                                   size = 0;
    EFI_INPUT_KEY                           Key = {0};
    EFI_HANDLE                              *HandleBuffer = NULL;
    UINTN                                   BufferSize = 0;
    UINT8                                   Index = 0;
    //UINT32                                  EdidAttributes;
    //UINTN                                   EdidOverrideDataSize;
    //UINT8                                   *EdidOverrideDataBlock;
    UINTN                                   EdidActiveDataSize = 0;
    UINT8                                   *EdidActiveDataBlock = NULL;
    UINT8                                   byte1 = 0,byte2 = 0;
    UINT32                                  ManufactureName = 0;
    
    Print(L"Key[1] get EDID raw information\n");
    Print(L"Key[2] get EDID override information");
    gST->ConOut->ClearScreen( gST->ConOut);
    gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
    do{
        gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
        if((UINT8)Key.UnicodeChar == 49 || (UINT8)Key.UnicodeChar == 50) //1 or 2
            break ;
    }while(1);
    
    if((UINT8)Key.UnicodeChar == 50){
        Status = gBS->LocateHandleBuffer(
                                ByProtocol,   
                                &gEfiDevicePathProtocolGuid,
                                NULL,
                                &BufferSize, 
                                &HandleBuffer);
        if(!EFI_ERROR(Status)){
            for(Index = 0;Index<BufferSize;Index++){
                Status = gBS->OpenProtocol (
                                  HandleBuffer[Index],
                                  &gEfiEdidActiveProtocolGuid,
                                  (VOID **) &OemActiveEdid,
                                  HandleBuffer[Index],
                                  NULL,
                                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                  );
                if(!EFI_ERROR(Status)) break;
            }
        }
        EdidActiveDataSize = OemActiveEdid->SizeOfEdid;
        EdidActiveDataBlock = OemActiveEdid->Edid;
        /*Status = gBS->LocateHandle(
                            ByProtocol,
                            &gEfiEdidActiveProtocolGuid,
                            NULL,
                            &BufferSize,
                            HandleBuffer);
        if(!EFI_ERROR(Status)){
            for ( Index = 0; Index < BufferSize / sizeof(EFI_HANDLE); Index ++) {
                  //search for Handle Regular Expression Protoco 
                Status = gBS->HandleProtocol (
                              HandleBuffer[Index],
                              &gEfiEdidActiveProtocolGuid,
                              (VOID**)&OemActiveEdid
                             );
                if(!EFI_ERROR(Status)) break;
            }
        }
        EdidActiveDataSize = OemActiveEdid->SizeOfEdid;
        EdidActiveDataBlock = OemActiveEdid->Edid;*/
        /*Status = gBS->LocateHandle(
                            ByProtocol,
                            &gEfiEdidOverrideProtocolGuid,
                            NULL,
                            &BufferSize,
                            HandleBuffer);
        if(!EFI_ERROR(Status)){
            for ( Index = 0; Index < BufferSize / sizeof(EFI_HANDLE); Index ++) {
                  //search for Handle Regular Expression Protoco 
                Status = gBS->HandleProtocol (
                              HandleBuffer[Index],
                              &gEfiEdidOverrideProtocolGuid,
                              (VOID**)&OemOverrideEdid
                             );
                if(!EFI_ERROR(Status)) break;
            }
            Status = OemOverrideEdid->GetEdid(OemOverrideEdid,
                                        NULL,
                                        &EdidAttributes,
                                        &EdidOverrideDataSize,
                                        (UINT8 **) &EdidOverrideDataBlock
                                        );
            if (!EFI_ERROR (Status) && EdidOverrideDataSize != 0){
                EdidActiveDataSize = EdidOverrideDataSize;
                EdidActiveDataBlock = EdidOverrideDataBlock;
            }else{
                Status = gBS->LocateHandle(
                                        ByProtocol,
                                        &gEfiEdidActiveProtocolGuid,
                                        NULL,
                                        &BufferSize,
                                        HandleBuffer);
                if(!EFI_ERROR(Status)){
                    for ( Index = 0; Index < BufferSize / sizeof(EFI_HANDLE); Index ++) {
                        Status = gBS->HandleProtocol (
                                      HandleBuffer[Index],
                                      &gEfiEdidActiveProtocolGuid,
                                      (VOID**)&OemActiveEdid
                                     );
                        if(!EFI_ERROR(Status)) break;
                    }
                    EdidActiveDataSize = OemActiveEdid->SizeOfEdid;
                    EdidActiveDataBlock = OemActiveEdid->Edid;
                }
            }
        }*/
    }else{
        Status = gBS->LocateHandleBuffer(
                                ByProtocol,   
                                &gEfiDevicePathProtocolGuid,
                                NULL,
                                &BufferSize, 
                                &HandleBuffer);
        if(!EFI_ERROR(Status)){
            for(Index = 0;Index<BufferSize;Index++){
                Status = gBS->OpenProtocol (
                                  HandleBuffer[Index],
                                  &gEfiEdidDiscoveredProtocolGuid,
                                  (VOID **) &OemDiscoveredEdid,
                                  HandleBuffer[Index],
                                  NULL,
                                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                  );
                if(!EFI_ERROR(Status)) break;
            }
        }
        EdidActiveDataSize = OemDiscoveredEdid->SizeOfEdid;
        EdidActiveDataBlock = OemDiscoveredEdid->Edid;
        /*Status = gBS->LocateHandle(
                                ByProtocol,
                                &gEfiEdidDiscoveredProtocolGuid,
                                NULL,
                                &BufferSize,
                                HandleBuffer);
        if(!EFI_ERROR(Status)){
            for ( Index = 0; Index < BufferSize / sizeof(EFI_HANDLE); Index ++) {
                Status = gBS->HandleProtocol (
                              HandleBuffer[Index],
                              &gEfiEdidDiscoveredProtocolGuid,
                              (VOID**)&OemDiscoveredEdid
                             );
                if(!EFI_ERROR(Status)) break;
            }
            EdidActiveDataSize = OemDiscoveredEdid->SizeOfEdid;
            EdidActiveDataBlock = OemDiscoveredEdid->Edid;
        }*/
    }
    if(!EFI_ERROR(Status)){
        Ptr = (UINT8 *)EdidActiveDataBlock;
        Print(L"UEFI EDID DATA:\n");
        for(int i = 1;i <= EdidActiveDataSize;i++){
            Print(L"%02x ",*(Ptr ++));
            if(i % 16 == 0)
                Print(L"\n");
        }
        
        EdidDataBlock = (VESA_BIOS_EXTENSIONS_EDID_DATA_BLOCK *)EdidActiveDataBlock;
        //header
        Print(L"Header:");
        for(i = 0;i< sizeof(EdidDataBlock->Header)/sizeof(EdidDataBlock->Header[0]);i++)
            Print(L"%x ",EdidDataBlock->Header[i]);
        //manufacturename
        byte1 = EdidDataBlock->ManufactureName & 0xFF;
        byte2 = EdidDataBlock->ManufactureName & 0xFF00;
        ManufactureName = byte2;
        ManufactureName = (ManufactureName << 8)|byte1;
        Name1 = ManufactureName & 0x7C00;
        Name2 = ManufactureName & 0x03E0;
        Name3 = ManufactureName & 0x001F;
        Print(L"\nManufactureName:%c%c%c\n",Name1+64,Name2+64,Name3+64);
        
        Print(L"Product Code:%x\n",EdidDataBlock->ProductCode);
        Print(L"SerialNumber:%x\n",EdidDataBlock->SerialNumber);
        Print(L"WeekOfManufacture:%d\n",EdidDataBlock->WeekOfManufacture);
        Print(L"YearOfManufacture:%d\n",1990 + EdidDataBlock->YearOfManufacture);
        Print(L"EdidVersion:%x\n",EdidDataBlock->EdidVersion);
        Print(L"EdidRevision:%x\n",EdidDataBlock->EdidRevision);
        Print(L"VideoInputDefinition:%x\n",EdidDataBlock->VideoInputDefinition);
        //byte 14
        if(EdidDataBlock->VideoInputDefinition & 0x80){
            Print(L"Input is a Digital Video Signal Interface:\n");
            Print(L"    Color Bit Depth:");
            switch(EdidDataBlock->VideoInputDefinition & 0X70){
                case 0:
                    Print(L"Color Bit Depth is undefined\n");
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    Print(L"%d Bits per Primary Color\n",4+2*(EdidDataBlock->VideoInputDefinition & 0X70));
                    break;
                default:
                    Print(L"Reserved(Do Not Use)\n");
                    break;
            }
            switch(EdidDataBlock->VideoInputDefinition & 0X0F){
                case 0:
                    Print(L"Digital Interface is not defined\n");
                    break;
                case 1:
                    Print(L"DVI is supported\n");
                    break;
                case 2:
                    Print(L"HDMI-a is supported\n");
                    break;
                case 3:
                    Print(L"HDMI-b is supported\n");
                    break;
                case 4:
                    Print(L"MDDI is supported\n");
                    break;
                case 5:
                    Print(L"DisplayPort is supported\n");
                    break;
                default:
                    Print(L"Reserved(Do Not Use)\n");
                    break;
            }
        } else{
            Print(L"Input is an Analog Video Signal Interface:\n");
            Print(L"    Signal Level Standard:");
            switch(EdidDataBlock->VideoInputDefinition & 0x60){
                case 0:
                    Print(L"0.700,0.300(1.000Vp-p)\n");
                    break;
                case 1:
                    Print(L"0.714,0.286(1.000Vp-p)\n");
                    break;
                case 2:
                    Print(L"1.000,0.400(1.400Vp-p)\n");
                    break;
                default:
                    Print(L"0.700,0.000(0.700Vp-p)\n");
                    break;
            }
            Print(L"    Video Setup:\n");
            if(EdidDataBlock->VideoInputDefinition & 0x10)
                Print(L"    Video Setup: Blank-to-Black setup or pedestal\n");
            else
                Print(L"    Video Setup: Blank Level = Black Level\n");
            Print(L"    Synchronization Types:\n");
            if(EdidDataBlock->VideoInputDefinition & 0x08)
                Print(L"    Separate Sync H & V Signals are supported\n");
            else
                Print(L"    Separate Sync H & V Signals are not supported\n");
            if(EdidDataBlock->VideoInputDefinition & 0x04)
                Print(L"    Composite Sync Signal on Horizontal is supported\n");
            else
                Print(L"    Composite Sync Signal on Horizontal is not supported\n");
            if(EdidDataBlock->VideoInputDefinition & 0x02)
                Print(L"    Composite Sync Signal on Green Video is supported\n");
            else
                Print(L"    Composite Sync Signal on Green Video is not supported\n");
            Print(L"    Serrations:");
            if(EdidDataBlock->VideoInputDefinition & 0x01)
                Print(L"Serration on the Vertical Sync is supported\n");
            else
                Print(L"Serration on the Vertical Sync is not supported\n");
        }
        //byte 15 and 16
        if(EdidDataBlock->MaxVerticalImageSize || EdidDataBlock->MaxHorizontalImageSize){
            if(EdidDataBlock->MaxVerticalImageSize)
                Print(L"    Horizontal Screen Size:%dcm\n",EdidDataBlock->MaxHorizontalImageSize);
            /*else
                Print(L"    Aspect Ratio (Landscape):%0.2f:1AR\n",1+EdidDataBlock->MaxHorizontalImageSize * 0.01);*/
            if(EdidDataBlock->MaxHorizontalImageSize)
                Print(L"    Vertical Screen Size:%dcm\n",EdidDataBlock->MaxVerticalImageSize);
            /*else
                Print(L"    Aspect Ratio (Portrait):%0.2f:1AR\n",0.28+EdidDataBlock->MaxVerticalImageSize /3.59);*/
                
        }else{
            Print(L"    the screen size or aspect ratio are unknown or undefined\n");
        }
        //byte17
        if(EdidDataBlock->DisplayTransferCharacteristic == 0xFF)
            Print(L"    the GAMMA value is not defined here and the GAMMA data shall be stored in an extension block (e.g., DI-EXT)\n");
        /*else
            Print(L"    Display Transfer Characteristic (GAMMA):%0.f\n",1.00+0.01*EdidDataBlock->DisplayTransferCharacteristic);*/
        
        //byte18
        if(EdidDataBlock->FeatureSupport & 0x80)
            Print(L"    Standby Mode is supported\n");
        else
            Print(L"    Standby Mode is not supported\n");
        if(EdidDataBlock->FeatureSupport & 0x40)
            Print(L"    Suspend Mode is supported\n");
        else
            Print(L"    Suspend Mode is not supported\n");
        if(EdidDataBlock->FeatureSupport & 0x20)
            Print(L"    Active Off = Very Low Power is supported\n");
        else
            Print(L"    Active Off = Very Low Power is not supported\n");
        if(EdidDataBlock->VideoInputDefinition & 0x80){
            Print(L"    Supported Color Encoding Format/s:");
            switch(EdidDataBlock->FeatureSupport & 0x18){
                case 0:
                    Print(L"RGB 4:4:4\n");
                    break;
                case 1:
                    Print(L"RGB 4:4:4 & YCrCb 4:4:4\n");
                    break;
                case 2:
                    Print(L"RGB 4:4:4 & YCrCb 4:2:2\n");
                default:
                    Print(L"RGB 4:4:4 & YCrCb 4:4:4 & YCrCb 4:2:2\n");
                    break;
            }
        }else{
            Print(L"    Display Color Type:");
            switch(EdidDataBlock->FeatureSupport & 0x18){
                case 0:
                    Print(L"Monochrome or Grayscale display\n");
                    break;
                case 1:
                    Print(L"RGB color display\n");
                    break;
                case 2:
                    Print(L"Non-RGB color display\n");
                default:
                    Print(L"Display Color Type is Undefined\n");
                    break;
            }
        }
        Print(L"    Other Feature Support Flags:\n");
        if(EdidDataBlock->FeatureSupport & 0x04)
            Print(L"    sRGB Standard is the default color space\n");
        else
            Print(L"    sRGB Standard is not the default color space\n");
        if(EdidDataBlock->FeatureSupport & 0x02)
            Print(L"    Preferred Timing Mode includes the native pixel format and preferred refresh rate of the display device\n");
        else
            Print(L"    Preferred Timing Mode does not include the native pixel format and preferred refresh rate of the display device\n");
        if(EdidDataBlock->FeatureSupport & 0x01)
            Print(L"    Display is continuous frequency\n");
        else
            Print(L"    Display is non-continuous frequency (multi-mode)\n");
        
        Print(L"    Color Characteristics:\n");
        //byte 19
        Print(L"    RedGreenLowBits:%d\n",EdidDataBlock->RedGreenLowBits);
        //byte 1A
        Print(L"    BlueWhiteLowBits:%d\n",EdidDataBlock->BlueWhiteLowBits);
        //byte 1B
        Print(L"    Red_x:%d\n",EdidDataBlock->RedX);
        //byte 1C
        Print(L"    Red_y:%d\n",EdidDataBlock->RedY);
        //byte 1D
        Print(L"    Green_x:%d\n",EdidDataBlock->GreenX);
        //byte 1E
        Print(L"    Green_y:%d\n",EdidDataBlock->GreenY);
        //byte 1F
        Print(L"    Blue_x:%d\n",EdidDataBlock->BlueX);
        //byte 20
        Print(L"    Blue_y:%d\n",EdidDataBlock->BlueY);
        //byte 21
        Print(L"    White_x:%d\n",EdidDataBlock->WhiteX);
        //byte 22
        Print(L"    White_y:%d\n",EdidDataBlock->WhiteY);
        
        //byte 23
        Print(L"Established Timings:\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x80)
            Print(L"    720 x 400 @ 70Hz  (IBM,VGA)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x40)
            Print(L"    720 x 400 @ 88Hz  (IBM,XGA2)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x20)
            Print(L"    640 x 480 @ 60Hz  (IBM,VGA)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x10)
            Print(L"    640 x 480 @ 67Hz  (Apple,Mac II)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x08)
            Print(L"    640 x 480 @ 72Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x04)
            Print(L"    640 x 480 @ 75Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x02)
            Print(L"    800 x 600 @ 56Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[0] & 0x01)
            Print(L"    800 x 600 @ 60Hz  (VESA)\n");
        
        //byte 24
        if(EdidDataBlock->EstablishedTimings[1] & 0x80)
            Print(L"    800 x 600 @ 72Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x40)
            Print(L"    800 x 600 @ 75Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x20)
            Print(L"    832 x 624 @ 75Hz  (Apple,Mac II)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x10)
            Print(L"    1024 x 768 @ 87Hz  (IBM)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x08)
            Print(L"    1024 x 768 @ 60Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x04)
            Print(L"    1024 x 768 @ 70Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x02)
            Print(L"    1024 x 768 @ 75Hz  (VESA)\n");
        if(EdidDataBlock->EstablishedTimings[1] & 0x01)
            Print(L"    1280 x 1024 @ 75Hz  (VESA)\n");
        
        //byte 25
        if(EdidDataBlock->EstablishedTimings[2] & 0x80)
            Print(L"    1152 x 870 @ 75Hz  (Apple,Mac II)\n");
        
        //byte 26
        Print(L"Standard Timing Identification:\n");
        i = 0;
        while(i<16){
            if(EdidDataBlock->StandardTimingIdentification[i]){
                HorizontalActivePixels = (31+EdidDataBlock->StandardTimingIdentification[i])*8;
                Print(L"    Horizontal active pixels: %d\n",HorizontalActivePixels);
                switch(EdidDataBlock->StandardTimingIdentification[i+1] & 0xC0){
                    case 0:
                        VerticalActivePixels = HorizontalActivePixels * 10/16;
                        break;
                    case 1:
                        VerticalActivePixels = HorizontalActivePixels * 3/4;
                        break;
                    case 2:
                        VerticalActivePixels = HorizontalActivePixels * 4/5;
                        break;
                    default:
                        VerticalActivePixels = HorizontalActivePixels * 9/16;
                        break;
                }
                Print(L"    Vertical active pixels:  %d\n",VerticalActivePixels);
                Print(L"    Refresh Rate(Hz):        %d\n",60+EdidDataBlock->StandardTimingIdentification[i+1] & 0x3F);
            }
            i+=2;
        }
        Print(L"    EDID Detailed timing:    %dx%d\n",HorizontalActivePixels,VerticalActivePixels);
        Print(L"ManufactureName:%x",EdidDataBlock->ManufactureName);
    }
    return EFI_SUCCESS;
}
