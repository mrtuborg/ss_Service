#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../myTypes.h"
#include "../global.h"
#include "fltr.h"

fltr::fltr(){
    cmdline=new char[1];
    cmdline[0]=0;
}

fltr::~fltr(){
	delete cmdline;
}

errType fltr::set_filter_cmdline(char* incmdline, size_t size){
		errType result=err_not_init;
		if (incmdline==0)  result=err_result_error;
		else {
		    delete cmdline;
		    cmdline=new char[size+1];
		    memcpy(cmdline, incmdline,size);
		    cmdline[size]=0;
		    result=err_result_ok;
		}
		return result;
}


errType fltr::set_filter_id(WORD inpid){
		errType result=err_not_init;
		id=inpid;
		return result;
}

errType fltr::set_filter_cpu(int incpu){
		errType result=err_not_init;
		cpu_usage=incpu;
		return result;
}

errType fltr::set_filter_mem(DWORD sz)
{
		errType result=err_not_init;
		mem_usage=sz;
		return result;

}


void fltr::dbgPrint(){
		printf("\ncmdline: %s\n", cmdline);
		printf("\t id=%lu\n",id);
		printf("\t cpu usage=%d\n",cpu_usage);
		printf("\t virt mem size + resident, KBytes = %lu\n", mem_usage);
}

BYTE fltr::get_filter_cpu(){  return cpu_usage; }
DWORD fltr::get_filter_mem(){  return mem_usage; }
WORD fltr::get_filter_id(){  return id; }
char* fltr::get_cmdline(){return cmdline; }

DWORD fltr::getSize()
{
    return sizeof(id)+strlen(cmdline)+sizeof(cpu_usage)+sizeof(mem_usage);
}

errType fltr::decode(BYTE* array, DWORD* size)
{
    errType result=err_not_init;
    DWORD offset=0;
    

    memcpy(array+offset, &id, sizeof(id)); 			offset+=sizeof(id);
    memcpy(array+offset, cmdline, strlen(cmdline)); 		offset+=strlen(cmdline);
    memcpy(array+offset, &cpu_usage, sizeof(cpu_usage));	offset+=sizeof(cpu_usage);
    memcpy(array+offset, &mem_usage, sizeof(mem_usage)); 	offset+=sizeof(mem_usage);
    
    *size=offset;
    
    return result;
}
errType fltr::encode(BYTE* array)
{
    errType result=err_not_init;
    
    return result;
}

BYTE fltr::action()
{
    return _action;
}

errType fltr::set_filter_action(BYTE act)
{
    _action=act;
    return err_result_ok;
}
