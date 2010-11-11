#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../myTypes.h"
#include "../global.h"
#include "prcTask.h"

prcTask::prcTask(){
    environment=new char[1];
    environment[0]=0;
    
    cmdline=new char[1];
    cmdline[0]=0;
}

prcTask::~prcTask(){
	delete cmdline;
	delete state;
	delete environment;
}

errType prcTask::set_prc_cmdline(char* incmdline){
		errType result=err_not_init;
		if (incmdline==0)  result=err_result_error;
		else {
		    int len=strlen(incmdline);
		    delete cmdline;
		    cmdline=new char[len+1];
		    strcpy(cmdline, incmdline);
		    result=err_result_ok;
		}
		return result;
}

errType prcTask::set_prc_pid(char* inpid){
		errType result=err_not_init;
		pid=atol(inpid);
		return result;
}

errType prcTask::set_prc_ppid(long inpid){
		errType result=err_not_init;
		ppid=inpid;
		return result;
}

errType prcTask::set_prc_euid(long inpid){
		errType result=err_not_init;
		euid=inpid;
		return result;
}

errType prcTask::set_prc_pid(long inpid){
		errType result=err_not_init;
		pid=inpid;
		return result;
}

errType prcTask::set_prc_cpu(char* incpu){
		errType result=err_not_init;
		cpu_usage=atoi(incpu);
		return result;
}

errType prcTask::set_prc_cpu(int incpu){
		errType result=err_not_init;
		cpu_usage=incpu;
		return result;
}

errType prcTask::set_prc_state(char* instate){
		errType result=err_not_init;
		state=new char[strlen(instate)+1];
		strcpy(state,instate);
		return result;
}

errType prcTask::set_prc_uptime(char* tottime){
		errType result=err_not_init;
		uptime=atol(tottime);
		return result;
}

errType prcTask::set_prc_uptime(QWORD tottime){
		errType result=err_not_init;
		uptime=tottime;
		return result;
}

errType prcTask::set_prc_vsz(long sz)
{
		errType result=err_not_init;
		vsz=sz;
		return result;

}

errType prcTask::set_prc_rss(long inrss)
{
    		errType result=err_not_init;
		rss=inrss;
		return result;
}

errType prcTask::set_prc_vsz(char* sz){
		errType result=err_not_init;
		vsz=atoi(sz);
		return result;
}
errType prcTask::set_prc_rss(char* inrss){
		errType result=err_not_init;
		rss=atoi(inrss);
		return result;
}
errType prcTask::set_prc_environment(char* argv){
		errType result=err_not_init;
		
		if (argv==0) result=err_result_error;
		else {
		    int len=strlen(argv);
		    delete environment;
		    environment=new char[len+1];
		    strcpy(environment,argv);
		    result=err_result_ok;
		}
		
		return result;
}

void prcTask::dbgPrint(){
		printf("\ncmdline: %s\n", cmdline);
		printf("\t ppid=%lu\n",ppid);
		printf("\t pid=%lu\n",pid);
		printf("\t euid=%lu\n",euid);
		printf("\t cpu usage=%d\n",cpu_usage);
		printf("\t state markers: %s\n", state);
		printf("\t uptime=%lu\n",uptime);
		printf("\t virt mem size + resident, KBytes = %lu\n", vsz);
		printf("\t resident size,KBytes = %lu\n", rss);
		printf("\t environment: %s\n", environment);
}

BYTE prcTask::get_prc_cpu(){  return cpu_usage; }
DWORD prcTask::get_prc_vsz(){  return vsz; }
DWORD prcTask::get_prc_rss(){  return rss; }
WORD prcTask::get_prc_pid(){  return pid; }
WORD prcTask::get_prc_euid(){  return euid; }
QWORD prcTask::get_prc_uptime(){  return uptime; }
char* prcTask::get_prc_state(){  return state; }
WORD prcTask::get_prc_ppid(){  return ppid; }
char* prcTask::get_cmdline(){return cmdline; }
char* prcTask::get_prc_environment(){return environment; }


DWORD prcTask::getSize()
{
    return sizeof(pid)+strlen(cmdline)+strlen(environment)+sizeof(cpu_usage)+strlen(state)+sizeof(uptime)+sizeof(vsz)+sizeof(rss);
}

errType prcTask::decode(BYTE* array, DWORD* size)
{
    errType result=err_not_init;
    DWORD offset=0;
    

    memcpy(array+offset, &pid, sizeof(pid)); 		     offset+=sizeof(pid);
    memcpy(array+offset, cmdline, strlen(cmdline)); 	     offset+=strlen(cmdline);
    memcpy(array+offset, environment, strlen(environment)); offset+=strlen(environment);
    memcpy(array+offset, &cpu_usage, sizeof(cpu_usage));     offset+=sizeof(cpu_usage);
    memcpy(array+offset, state, strlen(state)); 	     offset+=strlen(state);
    memcpy(array+offset, &uptime, sizeof(uptime)); 	     offset+=sizeof(uptime);
    memcpy(array+offset, &vsz, sizeof(vsz)); 		     offset+=sizeof(vsz);
    memcpy(array+offset, &rss, sizeof(rss)); 		     offset+=sizeof(rss);
    
    *size=offset;
    
    return result;
}
errType prcTask::encode(BYTE* array)
{
    errType result=err_not_init;
    
    return result;
}
