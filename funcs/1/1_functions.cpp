// Scheduling Control Service
//----------------------------
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <deque>
#include <list>
#include <sched.h>
#include "../../rcsLib/ortsTypes/ortsTypes.h"
#include "../../rcsLib/rcsCmd/rcsCmd.h
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "job.h"
#include "../ICAppLayer/schedule.h"
#include "../buffer/ssBuffer.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "../../rcsLib/udp_port/udp_port.h"
#include "../global.h"

udp_port* equipment;
FILE *scheduleFile;
schedule *batchSched;

//#define EQ_UDP_PORT 5004
//#define EQ_IP_ADDR "127.0.0.1"


void* pollingThread(void* user)
{
  ICAppLayer* app = (ICAppLayer*) user;
  BYTE* array;
  WORD old_crc = 0xFFFF;
  while (!app->terminated())
    {
      if (sendFrame->setCheckSumm() != old_crc)
        {
          sendFrame->dbgPrint();
          old_crc = sendFrame->setCheckSumm();

          sendFrame->decode(&array);
          equip_sending->sendData(equipAddr, array, sizeof(cmdFrame_t));
        }
      sched_yield();
    }
  delete array;
  return user;
}

errType equipListenProcessing(BYTE *writingBuffer, size_t sz)
{
  errType result = err_result_ok;
  //if (sz>sizeof(SASC_msg_type)) sz=sizeof(SASC_msg_type);

  answerFrame->encode(writingBuffer, sz);
  printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
  printf("\t[");
  for (int k = 0; k < sz; k++)
    printf("%.2X ", writingBuffer[k]);
  printf("]\n\n");
  printf("\tРасшифровка:\n");
  answerFrame->dbgPrint();
  printf("\t===========================================\n\n");
  return result;
}

errType srvInit()
{
  void* pollingThread(void* user)
  {
    ICAppLayer* app = (ICAppLayer*) user;
    BYTE* array;
    WORD old_crc = 0xFFFF;
    while (!app->terminated())
      {
        if (sendFrame->setCheckSumm() != old_crc)
          {
            sendFrame->dbgPrint();
            old_crc = sendFrame->setCheckSumm();

            sendFrame->decode(&array);
            equip_sending->sendData(equipAddr, array, sizeof(cmdFrame_t));
          }
        sched_yield();
      }
    delete array;
    return user;
  }

errType srvDeinit()
{
    equipment->close_port();
    delete equipment;
    delete batchSched;
    return err_result_ok;
}


errType emergencyShutdown(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** EmergencyShutdown was called! ***\n");
    printf("*************************************\n");
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType controlModeChange(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** ControlModeChange was called! ***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType getStateVector(void* fn)
{
    errType result=err_result_ok;
    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
    func->setResult(1,&app->getStateVector());
    func->printResults();
    
    return result;
}



errType addScheduleJob(void* fn)
{
    errType result=err_not_init;


    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE isEmergencySchedule=*(BYTE*)(func->getParamPtr(0)); // Packet No
    DWORD objId=*(DWORD*)(func->getParamPtr(1));
    WORD timeStart=*(WORD*)func->getParamPtr(2);
    WORD timeEnd=*(WORD*)func->getParamPtr(3);
    BYTE service_id=*(BYTE*)func->getParamPtr(4);
    BYTE func_id=*(BYTE*)func->getParamPtr(5);
    WORD paramsLength=*(WORD*)func->getParamPtr(6);
    BYTE* params=(BYTE*)func->getParamPtr(7);



    char str[255];
    //printf("Schedule #%d\n",packetNo);

    //if (isEmergencySchedule) sprintf(str, "schedule.emergency");
    //else sprintf(str, "schedule.general");

    //scheduleFile = fopen (str,"a");

    //task schedule[100];
    //BYTE TaskQuantity;
    //WORD offset=0;
    
    //WORD i=0;
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

