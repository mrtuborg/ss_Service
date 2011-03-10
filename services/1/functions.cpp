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
schedule _schedule[2];


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

    _schedule[isEmergency].set_cpListenerPortNum(app->getListenerPortNum());
    _schedule[isEmergency].addJob(newJob);

    return result;
}

errType runSchedule(void* fn)
{
	errType result (err_not_init);
	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	_schedule[isEmergency].run();


	return result;
}

errType stopSchedule(void* fn)
{
	errType result (err_result_ok);
	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	_schedule[isEmergency].stop();

	return result;
}

errType readJobState(void* fn)
{
    errType result (err_result_ok);
    functionNode* func=(functionNode*)fn;
    
    func->printParams();
    job* operation=0;
    BYTE *jobStatus=0;
    BYTE *execAnswer=0;

    
	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No
	BYTE jobID=*(BYTE*)(func->getParamPtr(1)); // Job ID

	BYTE state=0;
	BYTE* answerVector=0;
	BYTE* answer=0;
	WORD answerLength=0;
	job* requestedJob = _schedule[isEmergency].getJobById(jobID);

	state = requestedJob->getState();
	requestedJob->lastAnswer(&answer, &answerLength);

	answerVector = new BYTE[answerLength+2];
	memcpy(answerVector+2,answer, answerLength);
	*(WORD*)answerVector = answerLength;

	func->setResult(1, &state);
	func->setResult(2, answerVector);

	delete []answerVector;
    return result;
}


errType getCursorPosition(void* fn)
{
    errType result=err_not_init;

    functionNode* func=(functionNode*)fn;
    
    func->printParams();

	BYTE isEmergency=*(BYTE*)(func->getParamPtr(0)); // Packet No

	DWORD jobID=0;

	jobID = _schedule[isEmergency].cursorPos();
	func->setResult(1, &jobID);
    
    return result;
}

errType readJobEntity(void* fn)
{
	errType result=err_not_init;

	functionNode* func=(functionNode*)fn;

	func->printParams();

	BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No
	DWORD jobID = *(BYTE*)(func->getParamPtr(1));

	job* requestedJob = _schedule[isEmergency].getJobById(jobID);

	DWORD nextObjId = (requestedJob->get_dwNextObjId());
	DWORD timeStart = (requestedJob->get_dwTimeStart());
	DWORD timeLong  = (requestedJob->get_dwTimeLong());

	struct in_addr IPaddr;
	(requestedJob->get_dwServiceIPaddr(&IPaddr));

	WORD udp       = (requestedJob->get_wServiceUdpPort());
	BYTE servId    = (requestedJob->get_btServId());
	BYTE funcId    = (requestedJob->get_btFuncId());

	func->setResult(1, &nextObjId);
	func->setResult(2, &timeStart);
	func->setResult(3, &timeLong);
	func->setResult(4, &IPaddr);
	func->setResult(5, &udp);
	func->setResult(6, &servId);
	func->setResult(7, &funcId);



	WORD answerLength = requestedJob->get_paramsLength();
	BYTE* paramsPtr = (BYTE*) requestedJob->get_paramsPtr();

	BYTE* answerVector = new BYTE[answerLength+2];
	memcpy(answerVector+2, paramsPtr, answerLength);
	*(WORD*)answerVector = answerLength;

	func->setResult(8, answerVector);
	delete []answerVector;

	return result;
}

errType getOpsId(void* fn)
{
	 errType result=err_not_init;

	 functionNode* func=(functionNode*)fn;

	 func->printParams();

	 BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No
	 WORD quantity = _schedule[isEmergency].getJobsQuantity();
	 DWORD id=0;

	 BYTE* answerVector = new BYTE[sizeof(id)*quantity+2];
	 ((WORD*)answerVector)[0] = quantity;
	 answerVector+=2;
	 for (int i = 0; i < quantity; i++)
	 {
		 id = _schedule[isEmergency].getJobByIndex(i)->get_dwObjId();
		 ((DWORD*)answerVector)[i] = id;
	 }
	 answerVector-=2;

	 func->setResult(1, answerVector);

	 return result;
}

errType executeJob(void* fn)
{
    errType result=err_not_init;

    functionNode* func=(functionNode*)fn;

    func->printParams();

	BYTE isEmergency = *(BYTE*)(func->getParamPtr(0)); // Packet No
	DWORD jobID = *(BYTE*)(func->getParamPtr(1)); // jobID

	_schedule[isEmergency].execute(jobID);

    return result;
}

