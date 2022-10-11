//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef __DEMOPROTOCOL__H__
#define __DEMOPROTOCOL__H__
#define WRITE_AND_READ_CMOS_PROTOCOL {0xb5a2e3d1, 0xc8a6, 0x440f, {0xa3, 0x25, 0x36, 0xd, 0xc9, 0x3f, 0xfb, 0xc2} }
// {b5a2e3d1-c8a6-440f-a325-360dc93ffbc2} 

extern EFI_GUID gRwCmosProtocolGuid;

typedef
UINT8
(EFIAPI *AMI_DXE_READ_COMS_PROTOCOL)(
    IN UINT8 Address
);

typedef
UINT8
(EFIAPI *AMI_DXE_WRITE_CMOS_PROTOCOL)(
    IN UINT8 Address,
    IN UINT8 Parameter
); 

typedef struct _READ_AND_WRITE_CMOS_PROTOCOL{
    AMI_DXE_READ_COMS_PROTOCOL Read;
    AMI_DXE_WRITE_CMOS_PROTOCOL Write;
}READ_AND_WRITE_CMOS_PROTOCOL;

#ifdef __cplusplus
extern "C" {
#endif

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
