//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiPeiLib.h>
#include <DemoHob.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PeimEntryPoint.h>
#include <Include/Pi/PiHob.h>
#include <Include/Pi/PiPeiCis.h>
#include <SystemAgent/Include/MemInfoHob.h>
#include <Library/HobLib.h>

extern EFI_GUID gSiMemoryInfoDataGuid;

EFI_STATUS CreateHobEntryPoint(
    IN EFI_PEI_FILE_HANDLE FileHandle,
    IN CONST EFI_PEI_SERVICES **PeiServices
)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    DEMO_EFI_HOB_STRING     *DemoEfiHobPointer = NULL;
    VOID                    *HobList = NULL;
    EFI_HOB_GUID_TYPE       *MemoryPointer = NULL;
    VOID                    *HobData = NULL;
    MEMORY_INFO_DATA_HOB    *mMemInfo = NULL;
    
    DEBUG((-1,"HOB create start.\n"));
    Status = (*PeiServices)->CreateHob(PeiServices,\
                            EFI_HOB_TYPE_GUID_EXTENSION,\
                            sizeof(DEMO_EFI_HOB_STRING),\
                            (VOID **)&DemoEfiHobPointer);
    
    if( !EFI_ERROR(Status) ){
        DemoEfiHobPointer->EfiHobGuidTypes.Name = gCreateAndUseHobGuid;
        DemoEfiHobPointer->Data = 0xab;
        DEBUG((-1,"HOB created.\n"));
    }
    // Get the Memory HOB list.
    HobList = GetHobList();
    ASSERT (HobList != NULL);
    
    MemoryPointer = GetNextGuidHob (&gSiMemoryInfoDataGuid, HobList);
    if(MemoryPointer != NULL){
        HobData = GET_GUID_HOB_DATA (MemoryPointer);
        mMemInfo = (MEMORY_INFO_DATA_HOB *) HobData;
        DemoEfiHobPointer->MemorySize = mMemInfo->TotalPhysicalMemorySize;  //Get the Memory Size
        DEBUG((-1,"memorysize got.\n"));
    }
	return Status;
}

