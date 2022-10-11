//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef __PPI__H__
#define __PPI__H__

#define WRITE_AND_READ_COMS_PPI_GUID {0xcf5959d4, 0x8752, 0x4a5b, {0xbe, 0x24, 0x83, 0x9e, 0xd2, 0xa, 0x94, 0x34}}
// {cf5959d4-8752-4a5b-be24-839ed20a9434} 
typedef struct _WRITE_AND_READ_COMS_PPI WRITE_AND_READ_COMS_PPI;

extern EFI_GUID gWriteAndReadCmosGuid;
typedef
UINT8
(EFIAPI *AMI_PEI_CRBINFO_READ_COMS)(
    IN UINT8 Address
);

typedef
UINT8
(EFIAPI *AMI_PEI_CRBINFO_WRITE_CMOS)(
    IN UINT8 Address,
    IN UINT8 Parameter
); 

struct _WRITE_AND_READ_COMS_PPI{
    AMI_PEI_CRBINFO_WRITE_CMOS WriteCmos;
    AMI_PEI_CRBINFO_READ_COMS ReadCmos;
};
#ifdef __cplusplus
extern "C" {
#endif

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
