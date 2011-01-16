// Covering Control Service
//--------------------------
#include <stdio.h> 
#include <string.h> 
#include <arpa/inet.h> 
#include <time.h> 
#include <deque>
#include "extra/ortsTypes/ortsTypes.h"
#include "ssBuffer.h" 
#include "buffer.h" 
#include "comm/udp_port/udp_port.h" 
#include "rcsLib/rcsCmd/rcsCmd.h" 
#include "param_desc.h" 
#include "FunctionNode.h" 
#include "srvAppLayer.h" 
#include "global.h" 

#include "cmdFrame.h"
#include "statusFrame.h"
#include "cm688_packet.h"

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
  srvAppLayer* app = (srvAppLayer*) user;
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
  for (size_t k = 0; k < sz; k++)
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
  functionNode* func = (functionNode*) fn;
  //func->dbgPrint();
  func->printResults();
  return result;
}

errType controlModeChange(void* fn)
{
  errType result = err_not_init;
  functionNode* func = (functionNode*) fn;
  func->printParams();
  //func->dbgPrint();
  func->printResults();
  return result;
}

errType getStateVector(void* fn)
{
  errType result = err_result_ok;
  functionNode* func = (functionNode*) fn;
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

  functionNode* func = (functionNode*) fn;

  func->printParams();
  func->printResults();

  return result;
}

errType shieldPowerOFF(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();
  func->printResults();

  return result;
}

errType hydroSystemPowerON(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setHydroStationStart();

  func->printResults();

  return result;
}

errType hydroSystemPowerOFF(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setHydroStationStop();

  func->printResults();

  return result;
}

errType hydroSystemGetParams(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();
  
  BYTE pressure=answerFrame->getHydroSystemValues(0);
  BYTE oilLevel=answerFrame->getHydroSystemValues(1);
  BYTE temp=answerFrame->getHydroSystemValues(2);
  BYTE filters=answerFrame->getHydroSystemValues(3);
  BYTE status=answerFrame->getHydroSystemStatus();

  // 1 - pressure, 2 - oil_level, 3 - temp, 4 - filters
  func->setResult(1, &pressure);
  func->setResult(2, &oilLevel);
  func->setResult(3, &temp);
  func->setResult(4, &filters);
  func->setResult(5, &status);

  func->printResults();

  return result;
}

errType foldOpen(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldOpen(foldId);
  func->setResult(1, &foldId);
  func->printResults();

  return result;
}

errType foldClose(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldClose(foldId);
  func->setResult(1, &foldId);
  func->printResults();

  return result;
}

errType foldStop(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE foldId = *(BYTE*) func->getParamPtr(0);

  sendFrame->setFoldStop(foldId);
 
  func->setResult(1, &foldId);
 
  func->printResults();

  return result;
}

errType shieldOpen(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setSchieldOpen();

  func->printResults();

  return result;
}

errType shieldClose(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setSchieldClose();

  func->printResults();

  return result;
}

errType shieldStop(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setSchieldStop();

  func->printResults();

  return result;
}

errType foldGetParams(void* fn)
{
  BYTE foldState;
  WORD foldPos;
  
  errType result = err_not_init;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE fold_id = *(BYTE*)func->getParamPtr(0);


  func->setResult(1, &fold_id);
  
  if (fold_id<3) {
    foldState = answerFrame->getFoldState(fold_id);
    foldPos = answerFrame->getFoldPosition(fold_id);
    result=err_result_ok;
    func->setResult(2, &foldState);
    func->setResult(3, &foldPos);
  }
  
  func->printResults();

  return result;
}

errType semiaxisSensorsGetState(void* fn)
{
  errType result = err_not_init;

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE fold_id     = *(BYTE*)func->getParamPtr(0);
  BYTE semiaxis_id = *(BYTE*)func->getParamPtr(1);
  BYTE sensor_id   = *(BYTE*)func->getParamPtr(2);
  BYTE sensorState = 0;

  switch (sensor_id)
  {
  case 0: //(in-path-sensor)
    sensorState = answerFrame->get_psa_sensors(fold_id);
    result=err_result_ok;
    break;
  case 1: //ending sensor
    sensorState = answerFrame->get_esa_sensors(fold_id);
    result=err_result_ok;
    break;
  default:
    result = err_params_value;
    break;
  }

  switch (semiaxis_id)
  {
  case 0: //right semiaxis
    sensorState &= 0x01;
    result=err_result_ok;
    break;
  case 1: //left semiaxis
    sensorState &= 0x02;
    sensorState >>= 1;
    result=err_result_ok;
    break;
  default:
    result = err_params_value;
    break;
  }

  func->setResult(1, &fold_id);
  func->setResult(2, &semiaxis_id);
  func->setResult(3, &sensor_id);
  func->setResult(4, &sensorState);

  func->printResults();

  return result;
}

errType changeControlMode(void* fn)
{
  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

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
  functionNode* func = (functionNode*) fn;
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
  functionNode* func = (functionNode*) fn;

  func->printParams();

  sendFrame->setStartFuncControl();

  func->printResults();

  return result;
}

errType correctHydroCyl(void* fn)
{

  errType result = err_result_ok;

  functionNode* func = (functionNode*) fn;

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

  functionNode* func = (functionNode*) fn;

  func->printParams();

  BYTE UZ_id=*(BYTE*)(func->getParamPtr(0));
  uzState_type UZ;

  UZ.fields.UZ_locked   = answerFrame->isUZ_locked(UZ_id);
  UZ.fields.UZ_unlocked = answerFrame->isUZ_unlocked(UZ_id);
  UZ.fields.UZ_busy     = answerFrame->isUZ_busy(UZ_id);
  UZ.fields.UZ_manual   = answerFrame->isUZ_manual(UZ_id);
  UZ.fields.UZ_alert    = answerFrame->isUZ_alert(UZ_id);

  func->setResult(1, &UZ_id);
  func->setResult(2, &UZ);
  
  func->printResults();

  return result;
}

errType getAllUZstate(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;
    uzState_type UZ[3];

    for(int i=0; i<3;i ++)
    {
	UZ[i].fields.UZ_locked   = answerFrame->isUZ_locked(i);
	UZ[i].fields.UZ_unlocked = answerFrame->isUZ_unlocked(i);
	UZ[i].fields.UZ_busy     = answerFrame->isUZ_busy(i);
	UZ[i].fields.UZ_manual   = answerFrame->isUZ_manual(i);
	UZ[i].fields.UZ_alert    = answerFrame->isUZ_alert(i);
    }

    func->setResult(1, &UZ[0]);
    func->setResult(2, &UZ[1]);
    func->setResult(3, &UZ[2]);
    
    return result;
}

typedef union semiaxisSensorsVector_t {
    struct{
	struct {
	    union {
		struct { 
		    BYTE right:1;
		    BYTE left:1;
		} fields;
		BYTE sa;
	    } psa;
	    
	    union {
		struct {
		    BYTE right:1;
		    BYTE left:1;
		} fields;
		BYTE sa;
	    } esa;
	    
	} fold[3];
    } fields;
    
    WORD wValue;
} __attribute__((packed)) semiaxisSensorsVector_t;


errType allSemiaxisSensorsGetState(void* fn)
{
    errType result=err_result_ok;
    functionNode* func = (functionNode*) fn;
    
    semiaxisSensorsVector_t vector;
    
    vector.fields.fold[0].psa.sa=answerFrame->get_psa_sensors(0);
    vector.fields.fold[0].esa.sa= answerFrame->get_esa_sensors(0);

    vector.fields.fold[1].psa.sa=answerFrame->get_psa_sensors(1);
    vector.fields.fold[1].esa.sa= answerFrame->get_esa_sensors(1);

    vector.fields.fold[2].psa.sa=answerFrame->get_psa_sensors(2);
    vector.fields.fold[2].esa.sa= answerFrame->get_esa_sensors(2);


    func->setResult(1, &vector);
    
    return result;
}

errType allFoldsGetParams(void* fn)
{
    errType result=err_result_ok;
    functionNode* func = (functionNode*) fn;
    BYTE foldState;
    WORD foldPos;
    
    for (int i=0; i<3; i++){
	foldState = answerFrame->getFoldState(i);
	foldPos = answerFrame->getFoldPosition(i);
	func->setResult(2*i+1, &foldState); // index: 1, 3 ,5
	func->setResult(2*i+2, &foldPos);   // index: 2, 4, 6
    }
    
    return result;
}

errType getHydroCylConsystency(void* fn)
{
  errType result=err_result_ok;
  functionNode* func = (functionNode*) fn;
  BYTE hcConsistency=answerFrame->getHydroCylStatus(0);
  func->setResult(1, &hcConsistency);
  hcConsistency=answerFrame->getHydroCylStatus(0);
  func->setResult(2, &hcConsistency);
  hcConsistency=answerFrame->getHydroCylStatus(0);
  func->setResult(3, &hcConsistency);
  return result;
}
