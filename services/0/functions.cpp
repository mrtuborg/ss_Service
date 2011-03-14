// SelfDiagnistics System Service
//--------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <list>
#include <queue>

#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>
#include <ssBuffer.h>
#include <peripheral/udp_port/udp_port.h>
#include <srvAppLayer.h>
#include <prcList/prcTask.h>
#include <prcList/prcList.h>
#include <fltrList/fltr.h>
#include <fltrList/fltrList.h>
#include <global.h>

udp_port* equipment;
FILE *scheduleFile;

pthread_t PollingThreadHandle; 

typedef struct monitor_t {
    prcList *processes;
    fltrList *filters;
    SrvAppLayer *application;
} monitor_t;


prcList *prcsMgr;
fltrList *fltrMgr;


//#define EQ_UDP_PORT 5004
//#define EQ_IP_ADDR "127.0.0.1"

using namespace std;


errType equipListenProcessing(BYTE *writingBuffer, size_t *sz)
{
    size_t size (*sz);
    //if (size>sizeof(SASC_msg_type)) size=sizeof(SASC_msg_type);
                                                                                                   
    //SASCmsg.encode(writingBuffer, size);
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");                                
    printf("\t[");                                                                                 
    for(size_t k=0; k<size; k++) printf("%.2X ", writingBuffer[k]);
    //equip_recvBuffer->unlockBufferChunkForExternWriting(size);
    printf("]\n\n");                                                                               
    /*printf("\tРасшифровка:\n");                                                                    
    if (SASCmsg.checkAnswer(&typeinf)==err_result_ok) {                                            
	printf("\tПринята ответная квитанция: %s\n", SASC_answer_str[typeinf]);                    
    } else {                                                                                       
        printf("\tМодификатор ответа не распознан. Обобщённое сообщение:\n");                      
        SASCmsg.dbgPrint();                                                                        
    } */                                                                                             
    printf("\t===========================================\n\n");                                   
    return err_result_ok;
}

void* PollingThread(void* user)                                                           
{                                                                                         
    monitor_t *mon=(monitor_t*)user;
    
    fltrList* fltrMgr=mon->filters;                                                
    prcList* procMgr=mon->processes;                                                
    SrvAppLayer* app=mon->application;
    
    procMgr->scanUserProcesses();
    
    
    while (!app->terminated()){                                                       
	
	if (fltrMgr->quantity()>0) {
	    
	    fltrMgr->applyFilters(procMgr);
	    sleep(3);
	}
	
        app->srv_yield();
    }                                                                                 
    return user;
}


errType srvInit()
{
    errType result=err_not_init;
    WORD ret;
    
    prcsMgr=new prcList;
    fltrMgr=new fltrList;
    
    
    monitor_t *monitor =new monitor_t;
    
    monitor->processes=prcsMgr;
    monitor->filters=fltrMgr;
    monitor->application=app;
    
    //printf("prcsMgr=%p\n", monitor->processes);

    ret=pthread_create(&PollingThreadHandle, NULL, PollingThread, monitor); 
    if (ret==0) result=err_result_ok;
    return result;
}

errType srvDeinit()
{
    delete prcsMgr;
    delete fltrMgr;
    //delete monitor;
    return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** EmergencyShutdown was called! ***\n");
    printf("*************************************\n");
    
    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType controlModeChange(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** ControlModeChange was called! ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType getStateVector(void* fn)
{
    errType result=err_result_ok;
    
    functionNode* func=(functionNode*)fn;
    func->printParams();
    
    func->setResult(1,&app->getStateVector());
    
    func->printResults();

    return result;
}

errType GetProcessesList(void* fn)
{
    errType result=err_not_init;
    
    functionNode* func=(functionNode*)fn;
    
    WORD *sendingArray;

    WORD length;
    prcsMgr->scanUserProcesses();
    
    length=prcsMgr->quantity();

    sendingArray=new WORD[length+1];
    
    sendingArray[0]=length*2;
    
    for(int i=0;i<length; i++)
    {
	if (prcsMgr->getUserProcessByIndex(i)) {
	    sendingArray[i+1]=prcsMgr->getUserProcessByIndex(i)->get_prc_pid();
	}
    }
    
    func->setResult(1,sendingArray);
    if (length>0) result=err_result_ok;
    
    delete []sendingArray;
    return result;
}

errType GetProcessInfo(void* fn)
{
    errType result=err_not_init;
    
    WORD ppid, euid;
    QWORD uptime;
    BYTE cpu;
    DWORD vsz, rss;
    
    char *state;
    WORD state_len;
    
    char *cmdline;
    WORD cmdline_len;
    
    char *environment;
    WORD environment_len;
    
    functionNode* func=(functionNode*)fn;
    
    WORD pid=*(WORD*)func->getParamPtr(0);
    
    prcsMgr->scanUserProcesses();
    
    bool pid_founded=prcsMgr->getUserProcess(pid);
    
    if (pid_founded) {
	ppid=prcsMgr->getUserProcess(pid)->get_prc_ppid();
	euid=prcsMgr->getUserProcess(pid)->get_prc_euid();
	cpu=prcsMgr->getUserProcess(pid)->get_prc_cpu();
	vsz=prcsMgr->getUserProcess(pid)->get_prc_vsz();
	rss=prcsMgr->getUserProcess(pid)->get_prc_rss();
	uptime=prcsMgr->getUserProcess(pid)->get_prc_uptime();
    
	state_len=strlen(prcsMgr->getUserProcess(pid)->get_prc_state());
	state=new char[state_len+3];
	((WORD*)state)[0]=state_len;
	strcpy(state+2,prcsMgr->getUserProcess(pid)->get_prc_state());

	cmdline_len=strlen(prcsMgr->getUserProcess(pid)->get_cmdline());
	cmdline=new char[cmdline_len+3];
	((WORD*)cmdline)[0]=cmdline_len;
	strcpy(cmdline+2,prcsMgr->getUserProcess(pid)->get_cmdline());

	environment_len=strlen(prcsMgr->getUserProcess(pid)->get_prc_environment());
	environment=new char[environment_len+3];
	((WORD*) environment)[0]=environment_len;
	strcpy(environment+2,prcsMgr->getUserProcess(pid)->get_prc_environment());
    
	prcsMgr->getUserProcess(pid)->dbgPrint();
	printf("\n");
    
    
	func->setResult(1,&ppid);
	func->setResult(2,&euid);
	func->setResult(3,&cpu);
	func->setResult(4,state);
	func->setResult(5,&uptime);
	func->setResult(6,&vsz);
	func->setResult(7,&rss);
	func->setResult(8,cmdline);
	func->setResult(9,environment);
	printf("\n");
	 
	delete []state;
	delete []cmdline;
	delete []environment;

	result=err_result_ok;
    } else {
	printf("\tПроцесс с запрошенным идентификатором  не найден\n\n");
	result=err_params_value;
    }
    
    
    func->setResult(0,&result);
    if (result==err_result_ok) func->printResults(); 
    
    return result;
}


errType SearchProcess(void* fn)
{
    errType result=err_params_value;
    
    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    
    

    WORD pattern_size=*(WORD*)(func->getParamPtr(0));
    char *pattern=(char*)(func->getParamPtr(0))+2;
    
    prcsMgr->scanUserProcesses();
    
    prcTask* prc=prcsMgr->searchUserProcessByCmdline(pattern, pattern_size);
    
    if (prc) 
    {
	WORD resPid=prc->get_prc_pid();
	func->setResult(1,&resPid);
	result=err_result_ok;
    }
    else result=err_not_found;
    
    func->printResults();
    
    return result;
}



errType RebootSystem(void* fn)
{
    errType result=err_not_init;
    
    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    app->terminate(2); // terminate & reboot
    return result;
}

errType KillProcess(void* fn)
{
    errType result=err_not_init;
    
    functionNode* func=(functionNode*)fn;
    
    BYTE sig=SIGKILL;
    WORD pid=*(WORD*)(func->getParamPtr(0));
    //BYTE sig=*(BYTE*)(func->getParamPtr(1));
    
    int res=kill(pid, sig);
    if (res<0) 
    {
	//errno: EINVAL, ESRCH, EPERM, 
    } else result=err_result_ok;
    
    return result;
}

errType RestartProcess(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** RestartProcess was called!    ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    
    //WORD pid=*(WORD*)(func->getParamPtr(0));
    // 0. Find pid in processeslist
    // 1. saving running string with arguments
    // 2. Kill pid
    // 3. Wait for killing
    // 4. Start earlier saving string
    // 5. Wait for starting

    return result;
}

errType InstallFilter(void* fn)
{
    errType result=err_not_init;
    
    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    
    char* pattern      = (char*)func->getParamPtr(0)+sizeof(WORD);
    WORD pattern_size  = *(WORD*)func->getParamPtr(0);
    BYTE  cpu_load_max =*(BYTE*)func->getParamPtr(1);
    DWORD mem_alloc_max=*(DWORD*)func->getParamPtr(2);
    BYTE  reaction     =*(BYTE*)func->getParamPtr(3); 
    char* cmdline=new char[pattern_size+1];
    memcpy(cmdline, pattern, pattern_size);
    cmdline[pattern_size]=0;

    fltr* prcFilter=new fltr;
    prcFilter->set_filter_id(fltrMgr->quantity()+1);
    prcFilter->set_filter_cpu(cpu_load_max);
    prcFilter->set_filter_mem(mem_alloc_max);
    prcFilter->set_filter_cmdline(cmdline, pattern_size+1);
    prcFilter->set_filter_action(reaction);
    
    fltrMgr->addProcFilter(prcFilter);
//    prcsMgr->scanUserProcesses();
//    while (fltrMgr->applyFilters(prcsMgr)==err_result_ok);
    result=err_result_ok;
    func->printResults();
    return result;
}
