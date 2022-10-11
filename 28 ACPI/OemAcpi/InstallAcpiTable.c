//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *

#include <AmiDxeLib.h>
#include <Include\Acpi.h>
#include <Include\Uefi\UefiBaseType.h>
#include <Include\Protocol\AcpiTable.h>
#include <Include\Uefi\UefiSpec.h>
#include <Include\AcpiRes.h>
#include <Include\AmiProtocol.h>
#include <Include\Uefi\UefiMultiPhase.h>

static ACPI_HDR                 SampleAcpiTable;

EFI_STATUS  GetAslNameObject(
    IN  ACPI_HDR  *PDsdt,
    IN  UINT8     *ObjName,
    OUT UINT64    *Value)
{
    ASL_OBJ_INFO    Obj;
    EFI_STATUS      Status;

    if (PDsdt->Signature != DSDT_SIG){
        return EFI_INVALID_PARAMETER;
    }

    Status = GetAslObj((UINT8*)(PDsdt+1), PDsdt->Length-sizeof(ACPI_HDR)-1,
        ObjName, otName, &Obj);

    if (!EFI_ERROR(Status)){
        *Value = *((UINT64*)((UINT8*)Obj.DataStart + 1));
    }

    return Status;
}

EFI_STATUS SmpDxeAcpiTable(
    IN VOID                         *Context
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_ACPI_TABLE_PROTOCOL         *AcpiTable = NULL;
    UINTN                           TableKey;
    
    Status = pBS->LocateProtocol(&gEfiAcpiTableProtocolGuid,\
                                NULL,\
                                &AcpiTable);
    if(!EFI_ERROR(Status)){
        Status = AcpiTable->InstallAcpiTable(AcpiTable,\
                                            Context,\
                                            ((ACPI_HDR *)Context)->Length,\
                                            &TableKey);
        if(!EFI_ERROR(Status)){
            DEBUG((-1,"AcpiTable installed step 2.\n"));
            DEBUG((-1,"TableKey: %x.\n",TableKey));
            return Status;
        }
    }
    return Status;
}

VOID SmpDxeUpdateAcpiTable(
    IN OUT ACPI_HDR                 *AcpiTable,
    IN UINT32                       Signature
)
{
    AcpiTable->Signature = Signature;
    AcpiTable->Length = sizeof(ACPI_HDR);
    AcpiTable->Revision = 1;
    AcpiTable->Checksum = 0;
    AcpiTable->OemId[0] = 'A';
    AcpiTable->OemId[1] = 'M';
    AcpiTable->OemId[2] = 'I';
    AcpiTable->OemId[3] = 'D';
    AcpiTable->OemId[4] = 'X';
    AcpiTable->OemId[5] = 'E';
    AcpiTable->OemTblId[0] = '0';
    AcpiTable->OemTblId[1] = '1';
    AcpiTable->OemTblId[2] = '2';
    AcpiTable->OemTblId[3] = '3';
    AcpiTable->OemTblId[4] = '4';
    AcpiTable->OemTblId[5] = '5';
    AcpiTable->OemTblId[6] = '6';
    AcpiTable->OemTblId[7] = '7';
    AcpiTable->OemRev = 0x12345678;
    AcpiTable->CreatorId = 0x12345678;
    AcpiTable->CreatorRev = 0x12345678;
    
    DEBUG((-1,"AcpiTable initialization step 1.\n"));
}

EFI_STATUS SetAcpiTableEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    VOID                        *gSmpDxeAcpiTableReg = NULL;
    UINT64                      OemVariableSize = 0;
    VOID                        *OemBuffer = NULL;
    EFI_PHYSICAL_ADDRESS        OemDsdtAddr = 0;
    ACPI_HDR                    *OemDsdt = NULL;
    UINT64                      CurrentValue = 0;
    
	InitAmiLib(ImageHandle,SystemTable);
	
	SmpDxeUpdateAcpiTable(&SampleAcpiTable,'AAAA');

	Status = SmpDxeAcpiTable(&SampleAcpiTable);
	if(!EFI_ERROR(Status)){
	    DEBUG((-1,"AcpiTable installtion setp3.\n"));
	}
	
	//allocate 1K bytes of memory with Type EfiACPIMemoryNVS
	OemVariableSize = 1024*sizeof(ACPI_HDR);
	Status = pBS->AllocatePool(EfiACPIMemoryNVS,\
	                            OemVariableSize,\
	                            &OemBuffer);
	if(EFI_ERROR(Status)){
	    DEBUG((-1,"ACPI buffer allocation failed.\n"));
	    return Status;
	}
	
    // Get a pointer to the DSDT.
	Status=LibGetDsdt(&OemDsdtAddr, EFI_ACPI_TABLE_VERSION_ALL);
	DEBUG((-1,"LibGetDsdt Status:%d\n",Status));
	
	if(!EFI_ERROR(Status)){
	    DEBUG((-1,"LibGetDsdt successful.\n"));
	    OemDsdt = (ACPI_HDR*)OemDsdtAddr;
	    
	    Status = GetAslNameObject(OemDsdt,"OFFO",&CurrentValue);
	    if(!EFI_ERROR(Status)){
	        DEBUG((-1,"OFFO CurrentValue :%x\n",CurrentValue));
	    }
        Status = UpdateAslNameObject(OemDsdt,"OFFO",(UINT64)OemBuffer);
        if (EFI_ERROR(Status)){
            DEBUG((-1, "failed to update \\OFFO\n"));
            return Status;
        }else{
            DEBUG((-1, "success to update \\OFFO\n"));
            Status = GetAslNameObject(OemDsdt,"OFFO",&CurrentValue);
            if(!EFI_ERROR(Status))
                DEBUG((-1,"OFFO CurrentValue :%x\n",CurrentValue));
        }
        
        CurrentValue = 0;
        Status = GetAslNameObject(OemDsdt,"SIZE",&CurrentValue);
        if(!EFI_ERROR(Status)){
            DEBUG((-1,"SIZE CurrentValue :%x\n",CurrentValue));
        }

        Status = UpdateAslNameObject(OemDsdt,"SIZE",(UINT64)OemVariableSize);
        if (EFI_ERROR(Status)){
            DEBUG((-1, "failed to update \\SIZE\n"));
            return Status;
        }else{
            DEBUG((-1, "success to update \\SIZE\n"));
            Status = GetAslNameObject(OemDsdt,"SIZE",&CurrentValue);
            if(!EFI_ERROR(Status)){
                DEBUG((-1,"SIZE CurrentValue :%x\n",CurrentValue));
                DEBUG((-1,"%x\n",OemVariableSize));
            }
        }
	}
	pBS->FreePool(OemBuffer);
	
	return Status;
}

