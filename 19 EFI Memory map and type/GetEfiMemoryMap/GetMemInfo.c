//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <AmiDxeLib.h>

extern EFI_BOOT_SERVICES      *gBS;

EFI_STATUS GetMemInfo(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINTN MemoryMapSize = 1;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    EFI_MEMORY_DESCRIPTOR *TempMemoryMap = NULL;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    EFI_STATUS Status;
    UINTN Index;
    UINT64 Size;
    
	InitAmiLib(ImageHandle,SystemTable);
	
	//Get the System MemoryMapSize and judge whether the size is appropriate
	do{
        Status = gBS->GetMemoryMap (
                     &MemoryMapSize,
                     MemoryMap,
                     &MapKey,
                     &DescriptorSize,
                     &DescriptorVersion
                 );
        if(Status == EFI_BUFFER_TOO_SMALL)
            MemoryMapSize += EFI_PAGE_SIZE;
        else
            break;
	}while(1);
	
	//allocate memory pool
	Status = gBS->AllocatePool (
                 EfiBootServicesData,
                 MemoryMapSize,
                 (VOID**)&MemoryMap
             );
    ASSERT_EFI_ERROR (Status);
    if(EFI_ERROR(Status)){
        return Status;
    }
    
    //get System MemoryMap
    Status = gBS->GetMemoryMap (
                 &MemoryMapSize,
                 MemoryMap,
                 &MapKey,
                 &DescriptorSize,
                 &DescriptorVersion
             );
    ASSERT_EFI_ERROR (Status);
    if(EFI_ERROR(Status)){
        gBS->FreePool (MemoryMap);
        return Status;
    }

    TempMemoryMap = MemoryMap;
	CHAR16 *Types[] = {L"EfiReservedMemoryType",L"EfiLoaderCode",L"EfiLoaderData",L"EfiBootServicesCode",\
	                L"EfiBootServicesData",L"EfiRuntimeServicesCode",L"EfiRuntimeServicesData",\
	                L"EfiConventionalMemory",L"EfiUnusableMemory",L"EfiACPIReclaimMemory",L"EfiACPIMemoryNVS",\
	                L"EfiMemoryMappedIO",L"EfiMemoryMappedIOPortSpace",L"EfiPalCode",L"EfiPersistentMemory",\
	                L"EfiMaxMemoryType"};
	
	//traversal MemoryMap
	for (Index = 0; Index < (MemoryMapSize / DescriptorSize); Index++){
        Size = MemoryMap->NumberOfPages <<  EFI_PAGE_SHIFT;
        DEBUG((-1,"Memory Type: %s;\t Address Start : %x;\t Address End : %x;\t Size : %x\n",\
                 Types[MemoryMap->Type],MemoryMap->PhysicalStart, MemoryMap->PhysicalStart+Size-1, Size));
	    MemoryMap = (EFI_MEMORY_DESCRIPTOR *)((UINTN)MemoryMap + DescriptorSize); //Adjust the direction of the pointer
	}
	gBS->FreePool (TempMemoryMap);
	return EFI_SUCCESS;
}

