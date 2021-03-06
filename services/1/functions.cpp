// Scheduling Control Service
//----------------------------
#include <stdio.h>
#include <string>
#include <string.h>

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
#include <storage/ssBuffer/ssBuffer.h>
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
      app->srv_yield();
    }
  // delete array;
  return user;
}

errType equipListenProcessing(BYTE *writingBuffer, size_t *sz)
{
  errType result = err_result_ok;
  size_t size(*sz);
  //if (size>sizeof(SASC_msg_type)) size=sizeof(SASC_msg_type);

  //answerFrame->encode(writingBuffer, size);
  printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
  printf("\t[");
  for (size_t k = 0; k < size; k++)
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
  if (ret == 0)
    result = err_result_ok;
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
  errType result = err_not_init;
  printf("*************************************\n");
  printf("*** EmergencyShutdown was called! ***\n");
  printf("*************************************\n");

  functionNode* func = (functionNode*) fn;

  func->dbgPrint();

  return result;
}

errType controlModeChange(void* fn)
{
  errType result = err_result_ok;
  functionNode* func = (functionNode*) fn;

  func->printParams();
  BYTE mode = *(BYTE*) func->getParamPtr(0);
  if (mode < 2)
    app->setServiceMode(mode);
  else
    result = err_params_value;
  func->printResults();

  return result;
}

errType getStateVector(void* fn)
{
  errType result = err_result_ok;
  functionNode* func = (functionNode*) fn;
  func->printParams();
  func->setResult(1, &app->getStateVector());
  func->printResults();

  return result;
}

errType addScheduleJob(void* fn)
{
  errType result = err_abort;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  DWORD objId = *(DWORD*) (func->getParamPtr(1));
  DWORD nextObjId = *(DWORD*) (func->getParamPtr(2));
  DWORD timeStart = *(DWORD*) func->getParamPtr(3);
  DWORD timeLong = *(DWORD*) func->getParamPtr(4);
  DWORD IPaddr = *(DWORD*) func->getParamPtr(5);
  WORD UdpPort = *(WORD*) func->getParamPtr(6);

  BYTE service_id = *(BYTE*) func->getParamPtr(7);
  BYTE func_id = *(BYTE*) func->getParamPtr(8);
  BYTE* cmd = (BYTE*) func->getParamPtr(9);

  job* newJob = new job(objId, IPaddr, UdpPort);

  newJob->set_dwNextJobID(nextObjId);
  newJob->set_btServiceId(service_id);
  newJob->set_dwStartTime(timeStart);
  newJob->set_dwLongTime(timeLong);
  newJob->setJobCmd(func_id, *((WORD*) cmd), cmd + 2);

  _schedule[isEmergency].set_cpListenerPortNum(app->getListenerPortNum());
  _schedule[isEmergency].addJob(newJob);

  return result;
}

errType runSchedule(void* fn)
{
  errType result(err_not_init);
  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  _schedule[isEmergency].run();

  return result;
}

errType stopSchedule(void* fn)
{
  errType result(err_result_ok);
  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  _schedule[isEmergency].clear();
  _schedule[isEmergency].stop();

  return result;
}

errType readJobState(void* fn)
{
  errType result(err_result_ok);
  functionNode* func = (functionNode*) fn;

 // func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  BYTE jobID = *(BYTE*) (func->getParamPtr(1)); // Job ID

  BYTE state = 0;
  BYTE* answerVector = 0;
  BYTE* answer = 0;
  WORD answerLength = 0;
  job* requestedJob = _schedule[isEmergency].getJobById(jobID);

  state = requestedJob->getState();
  printf("\njob state %d", state);
  requestedJob->lastAnswer(&answer, &answerLength);

  answerVector = new BYTE[answerLength + 2];
  memcpy(answerVector + 2, answer, answerLength);
  *(WORD*) answerVector = answerLength;

  func->setResult(1, &jobID);
  func->setResult(2, &state);
  func->setResult(3, answerVector);

  delete[] answerVector;
  return result;
}

errType getCursorPosition(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No

  BYTE *jobID_vector;

  if (_schedule[isEmergency].cursorPos(&jobID_vector) != err_result_ok)
    printf("empty!\n");
  for (int i=0; i<*(WORD*)jobID_vector; i++) printf(" %.2X",*((BYTE*)jobID_vector+2+i));

  func->setResult(1, jobID_vector);
  func->printResults();

  delete[] jobID_vector;
  return result;
}

errType readJobEntity(void* fn)
{
  errType result = err_not_init;

  functionNode* func = (functionNode*) fn;

  //func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  DWORD jobID = *(BYTE*) (func->getParamPtr(1));

  job* requestedJob = _schedule[isEmergency].getJobById(jobID);

  DWORD nextObjId = (requestedJob->get_dwNextObjId());
  DWORD timeStart = (requestedJob->get_dwTimeStart());
  DWORD timeLong = (requestedJob->get_dwTimeLong());

  struct in_addr IPaddr;
  (requestedJob->get_dwServiceIPaddr(&IPaddr));

  WORD udp = (requestedJob->get_wServiceUdpPort());
  BYTE servId = (requestedJob->get_btServId());
  BYTE funcId = (requestedJob->get_btFuncId());

  func->setResult(1, &jobID);
  func->setResult(2, &nextObjId);
  func->setResult(3, &timeStart);
  func->setResult(4, &timeLong);
  func->setResult(5, &IPaddr);
  func->setResult(6, &udp);
  func->setResult(7, &servId);
  func->setResult(8, &funcId);

  WORD answerLength = requestedJob->get_paramsLength();
  BYTE* paramsPtr = (BYTE*) requestedJob->get_paramsPtr();

  BYTE* answerVector = new BYTE[answerLength + 2];
  memcpy(answerVector + 2, paramsPtr, answerLength);
  *(WORD*) answerVector = answerLength;

  func->setResult(9, answerVector);
  result = err_result_ok;
  delete[] answerVector;

  return result;
}

errType getOpsId(void* fn)
{
  errType result = err_result_ok;
  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No

  WORD quantity = _schedule[isEmergency].getJobsQuantity();
  DWORD id = 0;
  BYTE* answerVector = new BYTE[sizeof(id) * quantity + 2];
  ((WORD*) answerVector)[0] = quantity * sizeof(DWORD);

  answerVector += 2;

  for (int i = 0; i < quantity; i++)
    {
      job *j = _schedule[isEmergency].getJobByIndex(i);
      id = j->get_dwObjId();
      ((DWORD*) answerVector)[i] = id;
    }
  answerVector -= 2;

  func->setResult(1, answerVector);
  delete[] answerVector;
  return result;
}

errType executeJob(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE isEmergency = *(BYTE*) (func->getParamPtr(0)); // Packet No
  DWORD jobID = *(BYTE*) (func->getParamPtr(1)); // jobID

  result = _schedule[isEmergency].execute(jobID);

  return result;
}

