//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>
#include <Include\Protocol\AmiSio.h>
#include <Include\GenericSio.h>
#include <Include\Uefi\UefiSpec.h>


VOID                    *gSmpDxeAmiSioReg = NULL;
VOID
EFIAPI
SmpDxePrintAmiSio(
    IN EFI_EVENT    Event,
    IN VOID         *Context
){
    EFI_STATUS              Status = EFI_SUCCESS;
    UINTN                   BufferSize = sizeof(EFI_HANDLE);
    EFI_HANDLE              Handle;
    AMI_SIO_PROTOCOL        *AmiSio = NULL;
    SPIO_DEV               *SpioDev = NULL;
    UINT8                   SioData = 0;
    UINT8                   i = 0;
    Status = pBS->LocateHandle(ByRegisterNotify,\
                              NULL,\
                              gSmpDxeAmiSioReg,\
                              &BufferSize,\
                              &Handle);
    if(!EFI_ERROR(Status)){
        //Queries a handle to determine if it supports a specified protocol.
        Status = pBS->HandleProtocol(Handle,\
                                    &gEfiAmiSioProtocolGuid,\
                                    &AmiSio);
        if(!EFI_ERROR(Status)){
            SpioDev = (SPIO_DEV *)AmiSio;
            Status = AmiSio->Access(AmiSio,\
                                    FALSE,\
                                    //TRUE write,FALSE read
                                    FALSE,\
                                    //whether to ExitCfgMode
                                    0X07,\
                                    &SioData); //0x07's value
            if(EFI_ERROR(Status)){
                DEBUG((-1,"Logical device information reading failed.\n"));
                return;
            }
            DEBUG((-1,"Logical device information read successfully.\n"));
            DEBUG((-1,"SPIO_DEV:\n"));
            DEBUG((-1,"Device EFI_EISAID.HID:%x\n",SpioDev->EisaId.HID));
            DEBUG((-1,"Device EFI_EISAID.UID:%x\n",SpioDev->EisaId.UID));
            DEBUG((-1,"AMI_SDL_LOGICAL_DEV_INFO:\n"));
            DEBUG((-1,"Device Ldn:%x\n",SpioDev->DeviceInfo->LDN));
            DEBUG((-1,"Device PnpID:%x\n",SpioDev->DeviceInfo->PnpId));
            DEBUG((-1,"Device AslName:"));
            for(i = 0;i < 5;i ++){
                DEBUG((-1,"%c",SpioDev->DeviceInfo->AslName[i]));
            }
            DEBUG((-1,"\n"));
            DEBUG((-1,"AmiSio->Access[LDN:%x]:Rx07 = 0x%x.\n",SpioDev->DeviceInfo->LDN,SioData));
            return;
        }
        DEBUG((-1,"HandlePtorocol failed.\n"));
        return;
    }
    DEBUG((-1,"LocateHandle failed.\n"));
    return;
}

EFI_STATUS GetDeviceSourcesEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_EVENT                   Event;
	InitAmiLib(ImageHandle,SystemTable);
	
	Status = RegisterProtocolCallback(&gEfiAmiSioProtocolGuid,\
                                    SmpDxePrintAmiSio,\
                                    NULL,\
                                    &Event,\
                                    &gSmpDxeAmiSioReg);
	if(EFI_ERROR(Status)){
	    DEBUG((-1,"RegisterProtocolCallback failed.\n"));
	}
	return Status;
}

