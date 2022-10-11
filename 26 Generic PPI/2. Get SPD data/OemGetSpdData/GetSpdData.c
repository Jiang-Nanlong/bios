//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiPeiLib.h>
#include <Include\Ppi\Smbus2.h>
#include <Include\IndustryStandard\SmBus.h>
#include <Include\Library\PeiServicesLib.h>

EFI_STATUS GetSpdDataEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    UINT8                           SlaveAddr = 0xA0;
	UINTN                           Length = 1;
	EFI_PEI_SMBUS2_PPI              *SmbusPpi = NULL;
	EFI_SMBUS_DEVICE_ADDRESS        SlaveAddress;
	EFI_SMBUS_DEVICE_COMMAND        Command = 0x00;
	UINT8                           Data;
	
    Status = PeiServicesLocatePpi (&gEfiPeiSmbus2PpiGuid,\
                                 0,\
                                 NULL,\
                                 (VOID **) &SmbusPpi);
    if(!EFI_ERROR(Status)){
        DEBUG((-1,"PeiServicesLocatePpi.\n"));
        SlaveAddress.SmbusDeviceAddress = (UINTN)RShiftU64(SlaveAddr, 1);
        DEBUG((-1,"Data :\n"));
        for(Command = 0;Command <= 0xFF;Command ++){
            Status = SmbusPpi->Execute (SmbusPpi,\
                                      SlaveAddress,\
                                      Command,\
                                      EfiSmbusReadByte,\
                                      FALSE,\
                                      &Length,\
                                      &Data);
            DEBUG((-1,"%x \t",Data));
            if(Command % 16 ==15 )  //16 per line
                DEBUG((-1,"\n"));
        }
    }
    return Status;
}

