#include <stdio.h>
#include <string.h>
#include <list>
#include <signal.h>
#include <extra/ortsTypes/ortsTypes.h>
#include "../prcList/prcTask.h"
#include "../prcList/prcList.h"
#include "fltr.h"
#include "fltrList.h"




fltrList::fltrList()
{
    filters_size=0;
}

fltrList::~fltrList()
{
}

errType fltrList::addProcFilter(fltr* filter)
{
    errType result=err_not_init;
    
    if (getProcFilter(filter->get_filter_id())==0)
    {
        procFilters.push_back(filter);
        
    } else {
	printf("same filter exists!\n");
	removeProcFilter(filter->get_filter_id());
	procFilters.push_back(filter);
    }
    
    filters_size+=filter->getSize();
    return result;
}

errType fltrList::removeProcFilter(WORD id)
{
    errType result=err_not_init;
    list<fltr*>::iterator iter;
    
    for(iter=procFilters.begin(); iter!=procFilters.end(); ++iter)
    {
	if ((*iter)->get_filter_id()==id) 
	{
	    filters_size-=(*iter)->getSize();	
	    procFilters.erase(iter);
	    break;
	}
    }
    
    return result;
}

errType fltrList::applyFilters(prcList* processes) // Need to do in scanprocesses
{
    errType result=err_not_found;
    char *cmdline;
    fltr* filter;
    prcTask* prc;
    int res=0;
    BYTE sig=SIGKILL;
    bool act=false;
    
    if ((processes->quantity()==0)||(quantity()==0)) return result;
    
    //printf("filters=%d\n", quantity());
    //printf("processes=%p\n", processes);
    
    for (int i=0; i< quantity(); i++)
    {
	//printf("i=%d\n", i);
	filter=getProcFilterByIndex(i);
	cmdline=filter->get_cmdline();
	//printf("filter_cmd_line=%s\n", cmdline);
	prc=processes->searchUserProcessByCmdline(cmdline, strlen(cmdline));
	//printf("comparing...\n");
	//printf("prc=%p", prc);
	
	if (prc) 
	{
	    //printf("prc_cpu=%d\n",prc->get_prc_cpu());
	    //printf("prc_mem=%lu\n",prc->get_prc_rss());
	    //printf("filter_cpu=%d\n",filter->get_filter_cpu());
	    //printf("filter_mem=%lu\n",filter->get_filter_mem());
	
	    if (prc->get_prc_cpu()>=filter->get_filter_cpu()) act=true;
	    if (prc->get_prc_rss()>=filter->get_filter_mem()) act=true;
	    //printf("acting...");
	    if (act){
	    //printf("act!\n");
		switch(filter->action())
		{
		    case 0: // kill
			printf("Процесс №%d (%s) будет завершён\n", prc->get_prc_pid(), prc->get_cmdline());
			res=kill(prc->get_prc_pid(), sig);
			break;
		    case 1: // restart
			printf("Процесс №%d (%s) будет перезапущен\n", prc->get_prc_pid(), prc->get_cmdline());
			res=kill(prc->get_prc_pid(), sig);
			break;
		}
		result=err_result_ok;
		act=false;
	    }
	}
    }
    
    
    return result;
}

fltr* fltrList::getProcFilter(WORD id)
{
    fltr* result=0;
    list<fltr*>::iterator iter;
    
    
    for(iter=procFilters.begin(); iter!= procFilters.end(); ++iter)
    {
	if ((*iter)->get_filter_id()==id) result=(*iter);
    }
    
    
    return result;
}

fltr* fltrList::getProcFilterByIndex(WORD index)
{
    fltr* result=0;
    list<fltr*>::iterator iter;
    WORD step=0;
    bool terminate=false;
    
    iter=procFilters.begin(); 
    
    do {
        if ((step==index) || (iter==procFilters.end())) terminate=true;
	else {
	    iter++;
	    step++;
	}
    } while (!terminate);
    
    if ((step==index)&&(iter!=procFilters.end())) result=(*iter);
    
    return result;
}

WORD fltrList::quantity()
{
    return procFilters.size();
}

void fltrList::dbgPrint()
{
    list<fltr*>::iterator iter;
    printf ("Process filters have these PIDs: \n");
    for(iter=procFilters.begin(); iter!= procFilters.end(); ++iter)
    {
	(*iter)->dbgPrint();
	//printf("%d ",(*iter)->get_filter_id());
	//printf("cmdline: %s\n", (*iter)->get_cmdline());
    }
    printf("\n");
}

DWORD fltrList::getFilterListSize()
{
    return filters_size;
}

errType fltrList::decode (BYTE* dst_array)//, DWORD* dst_array_length)
{
    errType result=err_not_init;
    int offset=0;
    DWORD len;
    list<fltr*>::iterator iter;

    for(iter=procFilters.begin(); iter!= procFilters.end(); ++iter)
    {
	(*iter)->decode(dst_array+offset, &len);
        offset+=len;
    }
    
    return result;
}

errType fltrList::clear()
{
    procFilters.clear();
    return err_result_ok;
}

errType fltrList::encode (BYTE* src_array, DWORD src_array_len)
{
    errType result=err_not_init;
    fltr* flt;
    int offset=0;
    procFilters.clear();
    
    do {
	flt=(fltr*)src_array+offset;
	filters_size+=flt->getSize();
	procFilters.push_back(flt);
	offset+=sizeof(fltr);
    } while (offset<src_array_len);
    
    return result;

}

fltr* fltrList::searchProcFilterByCmdline(const char* cmdline, size_t size)
{
    fltr* result=0;
    list<fltr*>::iterator iter;                                                        
    WORD foundCount=0;                                                                    
    bool founded=false;                                                                   
    BYTE res=0;                                                                           
    
    if ((quantity()>0) && (cmdline!=0)) {
	iter=procFilters.begin();                                                              
	iter++;                                                                               
    
	do {                                                                              
	    res=strncmp((*iter)->get_cmdline(), cmdline, size);                       
	    if (res==0) {                                                             
                result=(*iter);                                                       
                founded=true;                                                         
                foundCount++;                                                         
	    }                                                                         
	    iter++;                                                                   
	} while (iter!=procFilters.end());                                                  
//	if (founded) result=(*iter);
    }

    return result;
}
