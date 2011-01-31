// Scheduling Control Service
//----------------------------
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <deque>
#include <list>
#include <sched.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>
#include "schedule/cronTab.h"
#include "schedule/job.h"
#include "schedule/schedule.h"
#include <ssBuffer.h>
#include <comm/udp_port/udp_port.h>
#include <srvAppLayer.h>
#include <global.h>

//udp_port* equipment;
pthread_t PollingThreadHandle;
FILE *scheduleFile;
schedule *batchShed;

//#define EQ_UDP_PORT 5004
//#define EQ_IP_ADDR "127.0.0.1"


void* pollingThread(void* user)
{
  srvAppLayer* app = (srvAppLayer*) user;
//  BYTE* array;
//  WORD old_crc = 0xFFFF;
  while (!app->terminated())
    {
      
      sched_yield();
    }
 // delete array;
  return user;
}

errType equipListenProcessing(BYTE *writingBuffer, size_t sz)
{
  errType result = err_result_ok;
  //if (sz>sizeof(SASC_msg_type)) sz=sizeof(SASC_msg_type);

  //answerFrame->encode(writingBuffer, sz);
  printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
  printf("\t[");
  for (size_t k = 0; k < sz; k++)
    printf("%.2X ", writingBuffer[k]);
  printf("]\n\n");
  printf("\tРасшифровка:\n");
//  answerFrame->dbgPrint();
  printf("\t===========================================\n\n");
  return result;
}

errType srvInit()
{
    errType result = err_not_init;
    int ret = 0;
    //sendFrame = new cmdFrame;
    //answerFrame = new statusFrame;
    printf("\tСлужба контроля технологического расписания работ\n");
    printf("=============================================================\n\n");
 //   equip_sending = new udp_port(eq_udp_sending_port);
 //   result = equip_sending->open_port();
 //   equipAddr.s_addr = inet_addr(eq_ip_addr);
    ret = pthread_create(&PollingThreadHandle, NULL, pollingThread, app);
    //if (ret!=0) Error:  Need to stop application initializing process...
    if (ret == 0) result = err_result_ok;
    //stateByte.bValue = 0;
    return result;
}

errType srvDeinit()
{
  //  equipment->close_port();
  //delete equipment;
    delete batchShed;
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



errType addScheduleJob(void* fn)
{
    errType result=err_abort;


    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    
    BYTE isEmergencySchedule=*(BYTE*)(func->getParamPtr(0)); // Packet No
    DWORD objId=*(DWORD*)(func->getParamPtr(1));
    DWORD nextObjId=*(DWORD*)(func->getParamPtr(2));
    WORD timeStart=*(WORD*)func->getParamPtr(2);
    WORD timeEnd=*(WORD*)func->getParamPtr(3);
    BYTE service_id=*(BYTE*)func->getParamPtr(4);
    BYTE func_id=*(BYTE*)func->getParamPtr(5);
    DWORD paramsLength=*(DWORD*)func->getParamPtr(6);
    BYTE* params=(BYTE*)func->getParamPtr(7);

    job* newJob=new job(objId);
    newJob->set_dwNextJobID(nextObjId);
    newJob->set_btServiceId(service_id);
    newJob->set_wStartTime(timeStart);
    newJob->set_wFinishTime(timeEnd);
    newJob->setJobCmd(func_id, paramsLength, params);

    batchShed->addJob(newJob);
    batchShed->update();

    /// use these files for cron:
    /// data_%jobId%.sdata - rcsCmd to send by cron scheduling
    /// addr_%jobId%.saddr - ip_addr/udp_port for sending data_%jobId%.sdata

    /*
    WORD i=0;
    do {
	schedule[i].encode((BYTE*)func->getParamPtr(1)+offset);
	schedule[i].dbgPrint();

	//  crontab: fprintf(scheduleFile, "0x%d ", schedule[i].get_wTimeStart());
	// addrFile:
	    fprintf(addrFile, "service_id=%d ", schedule[i].get_btServId());
	// dataFile:
	    fprintf(dataFile, "func_id=%d ", schedule[i].get_btFuncId());
	    schedule[i].

	for (int k=0; k<schedule[i].get_paramsLength(); k++)
	fprintf(scheduleFile, "%.2X", *(BYTE*)schedule[i].get_paramsPtr()+k);
	fprintf(scheduleFile, "\n");
	offset+=schedule[i].getLength();
	i++;
    } while (offset<func->getAllParamsLength()-1);
    */
    //fclose(scheduleFile);


    return result;
}

errType CreateEmergencySchedule(void* fn)
{
    errType result=err_not_init;
    printf("**************************************\n");
    printf("*CreateEmergencySchedule was called!**\n");
    printf("**************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    

    return result;
}

errType ReadGeneralSchedule(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("** ReadGeneralSchedule was called! **\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->printParams();

    return result;
}

errType ReadEmergencySchedule(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*ReadEmergencySchedule was called! **\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->printParams();

    return result;
}

errType GetCursorPosition(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** GetCursorPosition was called! ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->printParams();

    return result;
}

