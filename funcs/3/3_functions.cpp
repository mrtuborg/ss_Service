// Covering Control Service
//--------------------------
#include <stdio.h> 
#include <string.h> 
#include <arpa/inet.h> 
#include <time.h> 
#include <deque>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "../buffer/ssBuffer.h" 
#include "../buffer/buffer.h" 
#include "../../../udp_port/udp_port.h" 
#include "../../../rcsLib/rcsCmd/rcsCmd.h" 
#include "../ICAppLayer/FunctionNode/param_desc.h" 
#include "../ICAppLayer/FunctionNode/FunctionNode.h" 
#include "../ICAppLayer/ICAppLayer.h" 
#include "../global.h" 

#include "../cm688_packet/cmdFrame.h"
#include "../cm688_packet/statusFrame.h"
#include "../cm688_packet/cm688_packet.h"

udp_port *equip_sending;
pthread_t PollingThreadHandle;

cmdFrame *sendFrame;
statusFrame *answerFrame;

typedef union serviceStateByte
{
  struct
  {
    //BYTE
    //BYTE
    //BYTE
    //BYTE
    BYTE reserved :6;
    BYTE close :1;
    BYTE open :1;
    //BYTE foldsMoving:1;
  } fields;
  BYTE bValue;
}__attribute__ ((packed)) serviceStateByte;

// 00|0 not initialized
// 00|1 error
// 11|0 Stopped
// 11|1 Stopping
//------------
// 01|0 Opened
// 01|1 Opening
// 10|0 Closed
// 10|1 Closing
serviceStateByte stateByte;

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
  errType result = err_not_init;
  int ret = 0;

  sendFrame = new cmdFrame;
  answerFrame = new statusFrame;

  printf("\tСлужба системы контроля САУ укрытия\n");
  printf("=============================================================\n\n");
  equip_sending = new udp_port(eq_udp_sending_port);
  result = equip_sending->open_port();
  equipAddr.s_addr = inet_addr(eq_ip_addr);

  ret = pthread_create(&PollingThreadHandle, NULL, pollingThread, app);
  //if (ret!=0) Error:  Need to stop application initializing process...
  if (ret == 0)
    result = err_result_ok;

  stateByte.bValue = 0;

  return result;
}

errType srvDeinit()
{
  equip_sending->close_port();
  delete equip_sending;

  delete sendFrame;
  delete answerFrame;

  return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
  errType result = err_not_init;
  FunctionNode* func = (FunctionNode*) fn;
  //func->dbgPrint();
  func->printResults();
  return result;
}

errType controlModeChange(void* fn)
{
  errType result = err_not_init;
  FunctionNode* func = (FunctionNode*) fn;
  func->printParams();
  //func->dbgPrint();
  func->printResults();
  return result;
}

errType getStateVector(void* fn)
{
  errType result = err_result_ok;
  FunctionNode* func = (FunctionNode*) fn;
  stateVector_type stateVector;
  stateVector = app->getStateVector();

  //stateByte.fields.open=answerFrame->getShieldState(0) | answerFrame->getShieldState(2);
  //stateByte.fields.close=answerFrame->getShieldState(1) | answerFrame->getShieldState(2);

  //stateByte.fields.foldsMoving;


  func->printParams();
  func->setResult(1, &stateVector);
  func->printResults();
  return result;
}

errType shieldPowerON(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();
  func->printResults();

  return result;
}

errType shieldPowerOFF(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();
  func->printResults();

  return result;
}

errType hydroSystemPowerON(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setHydroStationStart();

  func->printResults();

  return result;
}

errType hydroSystemPowerOFF(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setHydroStationStop();

  func->printResults();

  return result;
}

errType hydroSystemGetParams(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();
  BYTE pressure=answerFrame->getHydroSystemValues(0);
  BYTE oilLevel=answerFrame->getHydroSystemValues(1);
  BYTE temp=answerFrame->getHydroSystemValues(2);
  BYTE filters=answerFrame->getHydroSystemValues(3);

  // 0 - pressure, 1 - oil_level, 2 - temp, 3 - filters
  func->setResult(1, &pressure);
  func->setResult(2, &oilLevel);
  func->setResult(3, &temp);
  func->setResult(4, &filters);

  func->printResults();

  return result;
}

errType foldOpen(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldOpen(foldId);

  func->printResults();

  return result;
}

errType foldClose(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldClose(foldId);

  func->printResults();

  return result;
}

errType foldStop(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldStop(foldId);

  func->printResults();

  return result;
}

errType shieldOpen(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setSchieldOpen();

  func->printResults();

  return result;
}

errType shieldClose(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setSchieldClose();

  func->printResults();

  return result;
}

errType shieldStop(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setSchieldStop();

  func->printResults();

  return result;
}

errType foldGetParams(void* fn)
{
  errType result = err_not_init;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE fold_id = *(BYTE*)func->getParamPtr(0);
  BYTE foldState = answerFrame->getFoldState(fold_id);
  WORD foldPos = answerFrame->getFoldPosition(fold_id);
  func->setResult(1, &foldState);
  func->setResult(2, &foldPos);

  func->printResults();

  return result;
}

errType semiaxisSensorsGetState(void* fn)
{
  errType result = err_not_init;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE fold_id     = *(BYTE*)func->getParamPtr(0);
  BYTE semiaxis_id = *(BYTE*)func->getParamPtr(1);
  BYTE sensor_id   = *(BYTE*)func->getParamPtr(2);
  BYTE sensorState = 0;

  switch (sensor_id)
  {
  case 0: //(in-path-sensor)
    sensorState = answerFrame->get_psa_sensors(fold_id);
    break;
  case 1: //ending sensor
    sensorState = answerFrame->get_esa_sensors(fold_id);
    break;
  default:
    result = err_params_value;
    break;
  }

  switch (semiaxis_id)
  {
  case 0: //right semiaxis
    sensorState &= 0x01;
    break;
  case 1: //left semiaxis
    sensorState &= 0x02;
    sensorState >>= 1;
    break;
  default:
    result = err_params_value;
    break;
  }

  func->setResult(1, &sensorState);

  func->printResults();

  return result;
}

errType changeControlMode(void* fn)
{
  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE mode = *(BYTE*) func->getParamPtr(0);

  switch (mode)
  {
  case 0:
    sendFrame->setCPControl();
    break; //Control panel control
  case 1:
    sendFrame->setRemoteControl();
    break; //Remote panel control
  case 2:
    sendFrame->setLocalControl();
    break; //Local panel control
  }

  func->printResults();

  return result;
}

errType getControlMode(void* fn)
{
  errType result = err_result_ok;
  FunctionNode* func = (FunctionNode*) fn;
  func->printParams();
  BYTE mode=3;

  if (answerFrame->isLocalControl()) mode=2;
  if (answerFrame->isRemoteControl()) mode=1;
  if (answerFrame->isCPControl()) mode=0;

  func->setResult(1, &mode);
  func->printResults();
  return result;
}

errType startFuncControl(void* fn)
{

  errType result = err_result_ok;
  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setStartFuncControl();

  func->printResults();

  return result;
}

errType correctHydroCyl(void* fn)
{

  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  sendFrame->setHydroCyl_correct();

  func->printResults();

  return result;

}

typedef union uzState_type{
    struct {
        BYTE reserved:3;
        BYTE UZ_locked:1;
        BYTE UZ_unlocked:1;
        BYTE UZ_busy:1;
        BYTE UZ_manual:1;
        BYTE UZ_alert:1;
    }  fields;
    BYTE bValue;
} __attribute__((packed)) uzState_type;

errType getUZstate(void* fn)
{

  errType result = err_result_ok;

  FunctionNode* func = (FunctionNode*) fn;

  func->printParams();

  BYTE UZ_id=*(BYTE*)(func->getParamPtr(0));
  uzState_type UZ;

  UZ.fields.UZ_locked   = answerFrame->isUZ_locked(UZ_id);
  UZ.fields.UZ_unlocked = answerFrame->isUZ_unlocked(UZ_id);
  UZ.fields.UZ_busy     = answerFrame->isUZ_busy(UZ_id);
  UZ.fields.UZ_manual   = answerFrame->isUZ_manual(UZ_id);
  UZ.fields.UZ_alert    = answerFrame->isUZ_alert(UZ_id);

  func->setResult(1, &UZ);
  func->printResults();

  return result;

 }
