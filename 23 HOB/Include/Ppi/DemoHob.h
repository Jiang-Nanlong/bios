//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef __DEMOHOB__H__
#define __DEMOHOB__H__

#define CREATE_AND_USE_HOB_GUID {0xc59f6a4a, 0x21b2, 0x4244, {0x9a, 0xbf, 0x3f, 0xd7, 0xee, 0x85, 0x73, 0xa5}}

extern EFI_GUID gCreateAndUseHobGuid;

typedef struct {
    EFI_HOB_GUID_TYPE EfiHobGuidTypes;
    UINT16 Data;
    UINT16 MemorySize;
}DEMO_EFI_HOB_STRING;

#ifdef __cplusplus
extern "C" {
#endif

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
