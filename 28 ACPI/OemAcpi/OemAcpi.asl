
Scope(\)
{
    Name(\OFFO, 0x100000)
    Name(\SIZE, 0xfffff)
    
    OperationRegion(BSPV,SystemMemory,OFFO,SIZE)
    Field(BSPV,AnyAcc,NoLock,Preserve)
    {
    
    }
    
    OperationRegion(PHIL,SystemIO,0x80,1)
    Field(PHIL,ByteAcc,NoLock,Preserve)
    {
        P80L, 8
    }
    Method(AA,1){
        Store(0xAA, P80L) 
        Sleep(10000) 
        Return(0)
    }
}