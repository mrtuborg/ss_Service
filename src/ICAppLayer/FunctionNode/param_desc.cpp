#include <stdio.h>
#include <string.h>
#include "../../../../rcsLib/ortsTypes/ortsTypes.h"
#include "param_desc.h"
param_desc::param_desc(OrtsType type, WORD param_len)
{
	//printf("type=%d, len=%d\n",(BYTE)type, param_len);
	param=new BYTE[param_len];
	_length=param_len;
	_type=type;
	_isVector=false;
}

param_desc::param_desc(OrtsType type)
{
	//printf("type=%d, len=%d\n",(BYTE)type, param_len);
	//param=new BYTE[param_len];
	_length=0;
	_type=type;
	_isVector=true;
}

param_desc::~param_desc()
{
	
	delete [] param_name;
	delete [] (BYTE*) param;
}

errType param_desc::resize(WORD new_size)
{
	errType result=err_not_init;
	if (_length!=new_size){
	    delete [] (BYTE*)param;
	    //if (!isVector) 
	    _length=new_size;
	    //else _length=new_size;
	    param=new BYTE[_length];
	    
	    if (param==0) result=err_mem_alloc;
	    else result=err_result_ok;
	} //else printf ("no resize needed\n");
	return result;
}

WORD param_desc::length()
{
    return _length;
}

void* param_desc::value()
{
    return param;
}

OrtsType param_desc::type()
{
    return _type;
}

bool param_desc::isVector()
{
    return (_type & 0xF0)!=0;
}

errType param_desc::setParam(void* param_val)
{
    errType result=err_not_init;
    	
    if (param_val==0) result=err_params_value;
    else {
	if (_isVector) resize(2+*(WORD*)param_val);
	memcpy(param, param_val,_length); // Need to copy for caller safety
	result=err_result_ok;
    }
    return result;
}

errType param_desc::setName(const char* name)
{
    errType result=err_not_init;
    param_name=new char[strlen(name)+1];
    strcpy(param_name,name);
    return result;
}

errType param_desc::printParam()
{
    errType result=err_not_init;
    //printf("ptr: %p, ",param_name);
    //printf("%s\n", param_name);

    switch (_type)
    {
	case type_ERRTYPE:
	    printf("\t%s: %s\n", param_name, strErrTypes[*(BYTE*)param]);
	    break;
	
	case type_BYTE:
	    printf("\t%s: %d\t(0x%.2X)\n", param_name, *(BYTE*)param, *(BYTE*)param);
	    break;
	
	case type_WORD:
	    printf("\t%s: %d\t(0x%.4X)\n", param_name, *(WORD*)param, *(WORD*)param);
	    break;
	
	case type_CHAR:
	    printf("\t%s: %c\n", param_name, *(char*)param);
	    break;

	case type_DWORD:
	    printf("\t%s: %d\t(0x%.8X)\n", param_name, *(DWORD*)param, *(DWORD*)param);
	    break;
	
	case type_QWORD:
	    printf("\t%s: %llu\n", param_name, *(QWORD*)param);
	    break;
	
	case type_FLOAT:
	    printf("\t%s: %f\n", param_name, *(float*)param);
	    break;
	
	case type_DOUBLE:
	    printf("\t%s: %f\n", param_name, *(double*)param);
	    break;
	    
	case type_CHARVECTOR:
	    printf("\t%s: ", param_name);
	    //printf("length1=%d\n", _length);
	    //printf("length2=%d\n", *(WORD*)param);
	    for (int i=0; i<*(WORD*)param; i++) printf("%c",*((char*)param+2+i));
	    printf("\n");
	    break;
    }
    
    return result;
}



void param_desc::dbgPrint()
{
    printf("param_desc ptr: %p\n",this);
    printf("param ptr: %p\n",param);
}
