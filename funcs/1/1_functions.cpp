// Scheduling Control Service
//----------------------------
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <deque>
#include <list>
#include <sched.h>
#include "../myTypes.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/job.h"
#include "../ICAppLayer/schedule.h"
#include "../buffer/ssBuffer.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "../udp/udp_port.h"
#include "../global.h"

udp_port* equipment;
FILE *scheduleFile;
schedule *batchSched;

//#define EQ_UDP_PORT 5004
//#define EQ_IP_ADDR "127.0.0.1"

errType srvInit()
{
    equipment=new udp_port(EQ_UDP_PORT);
    equipment->open_port();
    equipAddr.s_addr=inet_addr(EQ_IP_ADDR);
    
    batchSched=new schedule;
    
    return err_result_ok;
}

errType srvDeinit()
{
    equipment->close_port();
    delete equipment;
    delete batchSched;
    return err_result_ok;
}

void* PollingThreadFunc(void* user)
{
    ICAppLayer *app=(ICAppLayer*) user;
    while (!app->terminated())
    {
	batchSched->update();
	batchSched->checkAlarm();
	
	//=loadSchedule();
	//schedule->setAlarm(); // try decrease all times on current time
			      // if any time<=65534 seconds - install alarm
	
	sched_yield();
    }

}

errType EmergencyShutdown(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** EmergencyShutdown was called! ***\n");
    printf("*************************************\n");
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** ControlModeChange was called! ***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_result_ok;
    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
    func->setResult(1,&app->getStateVector());
    func->printResults();
    
    return result;
}



errType CreateGeneralSchedule(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*CreateGeneralSchedule was called!***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    BYTE packetNo=*(func->getParamPtr(0)); // Packet No
    char str[255];
    printf("Schedule #%d\n",packetNo);
    sprintf(str, "schedule_%d.dat",packetNo);
    scheduleFile = fopen (str,"w");
    //task schedule[100];
    BYTE TaskQuantity;
    WORD offset=0;
    
    WORD i=0;
    do {
/*	schedule[i].encode(func->getParamPtr(1)+offset);
	schedule[i].dbgPrint();
	fprintf(scheduleFile, "0x%.8X ", schedule[i].get_dwTime());
	fprintf(scheduleFile, "0x%d ", schedule[i].get_btServId());
	fprintf(scheduleFile, "0x%d ", schedule[i].get_btFuncId());
	for (int k=0; k<schedule[i].get_paramsLen(); k++)
	fprintf(scheduleFile, "%.2X", schedule[i].get_paramsPtr()+k);
	fprintf(scheduleFile, "\n");
	offset+=schedule[i].getLength();
	i++;
*/    } while (offset<func->getAllParamsLength()-1);
    
    fclose(scheduleFile);
    return result;
}

errType CreateEmergencySchedule(void* fn)
{
    errType result=err_not_init;
    printf("**************************************\n");
    printf("*CreateEmergencySchedule was called!**\n");
    printf("**************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();

    return result;
}

errType ReadGeneralSchedule(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("** ReadGeneralSchedule was called! **\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();

    return result;
}

errType ReadEmergencySchedule(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*ReadEmergencySchedule was called! **\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();

    return result;
}

errType GetCursorPosition(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** GetCursorPosition was called! ***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();

    return result;
}

