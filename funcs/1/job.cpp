#include <stdio.h>
#include "../myTypes.h"
#include "cmd.h"
#include "job.h"


job::job()
{

}

job::~job()
{

}

void job::encode(BYTE *data)
{

    this->time_ms=((job*) data)->time_ms;
    this->service_id=((job*) data)->service_id;
    this->action.encode(data+5);
}

void job::decode(BYTE *data)
{
/*
    func_id=((task*) data)->action.get_func_id();
    this->action.func_paramsLength=((task*) data)->action.get_func_paramsLength();
    this->action.func_params=new BYTE [this->action.func_paramsLength];
    memcpy(this->action.func_params, ((task*) data)->action.get_func_paramsPtr(), this->action.get_func_paramsLength());
*/    
}


WORD job::getLength()
{
    WORD cmd_len=0;
    WORD task_len=0;
    
    cmd_len=action.get_func_paramsLength()+3;
    task_len=cmd_len+sizeof(time_ms)+sizeof(service_id);
    
    return task_len;
}

void job::dbgPrint()
{
    printf("start time = 0x%.8X\n", time_ms);
    printf("calling service = %d\n", service_id);
    this->action.dbgPrint();
}

DWORD job::get_dwTime()
{
    return time_ms;
}

BYTE job::get_btServId()
{
    return service_id;
}

BYTE job::get_btFuncId()
{
    return action.get_func_id();
}

WORD job::get_paramsLen()
{
    return action.get_func_paramsLength();
}

BYTE* job::get_paramsPtr()
{
    return action.get_func_paramsPtr();
}
/*
BYTE* get_strTime()
{
    double day=(double)time_ms / 86400000); // 86400000 ms in 1 day
    double hh = day
}
*/
