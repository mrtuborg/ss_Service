// Scheduling Control Service
//----------------------------
#include <stdio.h>
#include <string>
#include <fstream>
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
#include "schedule/job/job.h"
#include "schedule/schedule.h"
#include <ssBuffer.h>
#include <peripheral/udp_port/udp_port.h>
#include <SrvAppLayer.h>
#include <global.h>

//udp_port* equipment;
pthread_t PollingThreadHandle;
FILE *scheduleFile;
schedule _shedule[2];


//#define EQ_UDP_PORT 5004
//#define EQ_IP_ADDR "127.0.0.1"


void* pollingThread(void* user)
{
  SrvAppLayer* app = (SrvAppLayer*) user;
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
  //  delete batchShed;
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
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    BYTE mode=*(BYTE*)func->getParamPtr(0);
    if (mode<2) app->setServiceMode(mode);
    else result=err_params_value;
    func->printResults();
    
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
    
    BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
    DWORD objId=*(DWORD*)(func->getParamPtr(1));
    DWORD nextObjId=*(DWORD*)(func->getParamPtr(2));
    DWORD timeStart=*(DWORD*)func->getParamPtr(3);
    DWORD timeLong=*(DWORD*)func->getParamPtr(4);
    DWORD IPaddr=*(DWORD*)func->getParamPtr(5);
    WORD UdpPort=*(WORD*)func->getParamPtr(6);

    BYTE service_id=*(BYTE*)func->getParamPtr(7);
    BYTE func_id=*(BYTE*)func->getParamPtr(8);
    BYTE* cmd=(BYTE*)func->getParamPtr(9);

    job* newJob=new job(objId, IPaddr, UdpPort);

    newJob->set_dwNextJobID(nextObjId);
    newJob->set_btServiceId(service_id);
    newJob->set_dwStartTime(timeStart);
    newJob->set_dwLongTime(timeLong);
    newJob->setJobCmd(func_id, *((WORD*)cmd), cmd+2);

    _shedule[isEmergency].addJob(newJob);

    return result;
}

errType runSchedule(void* fn)
{
	errType result (err_not_init);
	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	_shedule[isEmergency].run();


	return result;
}

errType stopSchedule(void* fn)
{
	errType result (err_not_init);
	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	_shedule[isEmergency].stop();

	return result;
}

errType readJobState(void* fn)
{
    errType result=err_not_init;
    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    
	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	BYTE jobID=*(BYTE*)(func->getParamPtr(1)); // Job ID

	// TODO: _shedule[isEmergency].getJob(jobID);
	// TODO: func->setResult(1, jobDecoded);

    return result;
}


errType getCursorPosition(void* fn)
{
    errType result=err_not_init;

    functionNode* func=(functionNode*)fn;
    
    func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	//TODO: jobID = _shedule[isEmergency].cursorPos();
	//TODO: func->setResult(1, jobID);
    
    return result;
}

errType readJobEntity(void* fn)
{
	errType result=err_not_init;

	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No

	return result;
}

errType getOpsId(void* fn)
{
	 errType result=err_not_init;

	 functionNode* func=(functionNode*)fn;

	 func->printParams();

	 BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No

	 return result;
}

errType executeJob(void* fn)
{
    errType result=err_not_init;

    functionNode* func=(functionNode*)fn;

    func->printParams();

	BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No
	DWORD jobID = *(BYTE*)(func->getParamPtr(1)); // jobID

	//TODO: _shedule[isEmergency].execute(jobID);
	//TODO: func->setResult(1, jobID);

    return result;
}

