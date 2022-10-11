//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiDxeLib.h>
#include <DemoHob.h>
#include <Include/Guid/HobList.h>

EFI_STATUS UseHobEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS              Status =   EFI_SUCCESS;
    DEMO_EFI_HOB_STRING     *DemoEfiHobPointer = NULL;
    EFI_GUID                HobListGuid = HOB_LIST_GUID;

	InitAmiLib(ImageHandle,SystemTable);
	
	DemoEfiHobPointer = (DEMO_EFI_HOB_STRING*)GetEfiConfigurationTable(pST, &HobListGuid);
	if (DemoEfiHobPointer != NULL) {
        Status = FindNextHobByGuid(&gCreateAndUseHobGuid, &DemoEfiHobPointer);
        if (EFI_ERROR(Status)) {
            DemoEfiHobPointer = NULL;
        }
        else {
            DEBUG((-1,"the result is: %x\n",DemoEfiHobPointer->Data));
            DEBUG((-1,"the memory size is: %u MB\n",DemoEfiHobPointer->MemorySize));
        }
    }
	return Status;
}

