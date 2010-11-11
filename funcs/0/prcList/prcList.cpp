#include <stdio.h>
#include <string.h>
#include <list>
#include "../myTypes.h"
#include "prcTask.h"
#include "prcList.h"
#include "proc/readproc.h"



prcList::prcList()
{
    procs_size=0;
    //processes=new list<prcTask*>;
}

prcList::~prcList()
{
    //processes.clear();
    //delete processes;
}

errType prcList::addUserProcess(prcTask* prc)
{
    errType result=err_not_init;
    
    if (getUserProcess(prc->get_prc_pid())==0)
    {
        processes.push_back(prc);
        
    } else {
	printf("same process exists!\n");
	removeUserProcess(prc->get_prc_pid());
	processes.push_back(prc);
    }
    
    procs_size+=prc->getSize();
    
    return result;
}

errType prcList::removeUserProcess(long pid)
{
    errType result=err_not_init;
    list<prcTask*>::iterator iter;
    
    for(iter=processes.begin(); iter!= processes.end(); ++iter)
    {
	if ((*iter)->get_prc_pid()==pid) 
	{
	    procs_size-=(*iter)->getSize();	
	    processes.erase(iter);
	    break;
	}
    }
    
    return result;
}

errType prcList::scanUserProcesses()
{
    errType result=err_not_init;
    prcTask *prc;
    int iter=0;
    char tmpState[2];
    
    tmpState[0]=0;
    tmpState[1]=0;
    
    processes.clear();               
    

    proc_t **ptsk=readproctab(PROC_FILLMEM|PROC_FILLCOM|PROC_FILLENV|PROC_FILLSTAT|PROC_FILLSTATUS|PROC_FILLWCHAN|PROC_FILLARG);
    
    
    do {
	if ((ptsk[iter]->ppid!=0) && (ptsk[iter]->ppid!=2)){
    
	    prc = new prcTask;
	    
	    prc->set_prc_euid(ptsk[iter]->euid);
	    prc->set_prc_ppid(ptsk[iter]->ppid);
	    prc->set_prc_pid(ptsk[iter]->tid);
	    prc->set_prc_cpu(ptsk[iter]->pcpu);
	    
	    
	    tmpState[0]=ptsk[iter]->state;
	    prc->set_prc_state(tmpState);
	    prc->set_prc_uptime(ptsk[iter]->cutime+ptsk[iter]->cstime);
	    prc->set_prc_rss(ptsk[iter]->rss);
	    prc->set_prc_vsz(ptsk[iter]->vm_size);
    
	    if (ptsk[iter]->cmdline!=0) prc->set_prc_cmdline(*(ptsk[iter]->cmdline));
	    if (ptsk[iter]->environ!=0) prc->set_prc_environment(*(ptsk[iter]->environ));
	    
	    //prc->dbgPrint();
	    
	    addUserProcess(prc);
	
        }
    } while (ptsk[++iter]);
    

    return result;
}

prcTask* prcList::getUserProcess(long pid)
{
    prcTask* result=0;
    list<prcTask*>::iterator iter;
    
    
    for(iter=processes.begin(); iter!= processes.end(); ++iter)
    {
	if ((*iter)->get_prc_pid()==pid) result=(*iter);
    }
    
    
    return result;
}

prcTask* prcList::getUserProcessByIndex(WORD index)
{
    prcTask* result=0;
    list<prcTask*>::iterator iter;
    WORD step=0;
    bool terminate=false;
    
    iter=processes.begin(); 
    
    do {
        if ((step==index) || (iter==processes.end())) terminate=true;
	else {
	    iter++;
	    step++;
	}
    } while (!terminate);
    
    if ((step==index)&&(iter!=processes.end())) result=(*iter);
    
    return result;
}

WORD prcList::quantity()
{
    return processes.size();
}

void prcList::dbgPrint()
{
    list<prcTask*>::iterator iter;
    printf ("Processes list have these PIDs: \n");
    for(iter=processes.begin(); iter!= processes.end(); ++iter)
    {
	(*iter)->dbgPrint();
	//printf("%d ",(*iter)->get_prc_pid());
	//printf("cmdline: %s\n", (*iter)->get_cmdline());
	//printf("cmdline: %s\n", (*iter)->get_prc_environment());
    }
    printf("\n");
}

DWORD prcList::getPrcListSize()
{
    return procs_size;
}

errType prcList::decode (BYTE* dst_array)//, DWORD* dst_array_length)
{
    errType result=err_not_init;
    int offset=0;
    DWORD len;
    list<prcTask*>::iterator iter;

    for(iter=processes.begin(); iter!= processes.end(); ++iter)
    {
	(*iter)->decode(dst_array+offset, &len);
        offset+=len;
    }
    
    return result;
}

errType prcList::clear()
{
    processes.clear();
    return err_result_ok;
}

errType prcList::encode (BYTE* src_array, DWORD src_array_len)
{
    errType result=err_not_init;
    prcTask* prc;
    int offset=0;
    processes.clear();
    
    do {
	prc=(prcTask*)src_array+offset;
	procs_size+=prc->getSize();
	processes.push_back(prc);
	offset+=sizeof(prcTask);
    } while (offset<src_array_len);
    
    return result;

}

prcTask* prcList::searchUserProcessByCmdline(const char* cmdline, size_t size)
{
    prcTask* result=0;
    list<prcTask*>::iterator iter;
    WORD foundCount=0;
    bool founded=false;
    BYTE res=0;
    scanUserProcesses();
    
    if ((quantity()>0) && (cmdline!=0)) {
	iter=processes.begin(); 
	iter++;
	do {
		res=strncmp((*iter)->get_cmdline(), cmdline, size);
		//printf("cmd=%s, res=%d\n",(*iter)->get_cmdline(),res);
		if (res==0) {
		    result=(*iter);
		    founded=true;
		    foundCount++;
		}
		iter++;
	} while (iter!=processes.end());

	//if (founded) printf("foundCount=%d\n", foundCount);
    }
    
    return result;
}
