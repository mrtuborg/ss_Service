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
  struct {
  //BYTE
  //BYTE
  //BYTE
  //BYTE
  BYTE reserved: 6;
  BYTE close:1;
  BYTE open:1;
  //BYTE foldsMoving:1;
  } fields;
  BYTE bValue;
}  __attribute__ ((packed)) serviceStateByte;

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

void* PollingThread(void* user)
{
	ICAppLayer* app=(ICAppLayer*)user;
	BYTE* array;
	WORD old_crc=0xFFFF;
	while (!app->terminated()){
	    if (sendFrame->setCheckSumm()!=old_crc) 
	    {
		sendFrame->dbgPrint();
		old_crc=sendFrame->setCheckSumm();
	    
	     
	    
	    sendFrame->decode(&array);
	    equip_sending->sendData(equipAddr,array,sizeof(cmdFrame_t));
	    }
	    sched_yield();
	}
	delete array;
}


errType EquipListenProcessing(BYTE *writingBuffer, size_t sz)
{
    //if (sz>sizeof(SASC_msg_type)) sz=sizeof(SASC_msg_type);
    
    answerFrame->encode(writingBuffer, sz);
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(int k=0; k<sz; k++) printf("%.2X ", writingBuffer[k]);
    printf("]\n\n");
    printf("\tРасшифровка:\n");
    answerFrame->dbgPrint();
    printf("\t===========================================\n\n");
}


errType srvInit()
{
    errType result=err_not_init;
    int ret=0;
    
    sendFrame=new cmdFrame;
    answerFrame=new statusFrame;
    
    printf("\tСлужба системы контроля САУ укрытия\n");
    printf("=============================================================\n\n");
    equip_sending=new udp_port(eq_udp_sending_port);
    result=equip_sending->open_port();
    equipAddr.s_addr=inet_addr(eq_ip_addr);
    
    ret=pthread_create(&PollingThreadHandle, NULL, PollingThread, app);
    //if (ret!=0) Error:  Need to stop application initializing process...
    if (ret==0) result=err_result_ok;
    
    stateByte.bValue=0;

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


errType EmergencyShutdown(void* fn)
{
    errType result=err_not_init;
    FunctionNode* func=(FunctionNode*)fn;
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_not_init;
    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_result_ok;
    FunctionNode* func=(FunctionNode*)fn;
    stateVector_type stateVector;
    stateVector=app->getStateVector();


    stateByte.fields.open=answerFrame->getShieldState(0) | answerFrame->getShieldState(2);
    stateByte.fields.close=answerFrame->getShieldState(1) | answerFrame->getShieldState(2);

    //stateByte.fields.foldsMoving;




    func->printParams();
    func->setResult(1,&stateVector);
    func->printResults();
    return result;
}

errType ShieldPowerON(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    func->printResults();
    
    return result;
}

errType ShieldPowerOFF(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    func->printResults();

    return result;
}

errType HydroSystemPowerON(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    sendFrame->setHydroStationStart();
    
    func->printResults();

    return result;
}

errType HydroSystemPowerOFF(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    sendFrame->setHydroStationStop();

    func->printResults();

    return result;
}


errType HydroSystemGetParams(void* fn)
{
    errType result=err_not_init;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    func->printResults();
    

    return result;
}

errType FoldOpen(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();

    BYTE foldId=*(BYTE*)func->getParamPtr(0);
    
    sendFrame->setFoldOpen(foldId);

    func->printResults();

    return result;
}

errType FoldClose(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();

    BYTE foldId=*(BYTE*)func->getParamPtr(0);
    
    sendFrame->setFoldClose(foldId);

    func->printResults();
    
    return result;
}

errType FoldStop(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE foldId=*(BYTE*)func->getParamPtr(0);
    
    sendFrame->setFoldStop(foldId);

    func->printResults();
    

    return result;
}


errType ShieldOpen(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();

    sendFrame->setSchieldOpen();

    func->printResults();
    
    return result;
}


errType ShieldClose(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();

    sendFrame->setSchieldClose();

    func->printResults();
    
    return result;
}

errType ShieldStop(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    sendFrame->setSchieldStop();

    func->printResults();
    

    return result;
}


errType FoldGetParams(void* fn)
{
    errType result=err_not_init;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    func->printResults();
    

    return result;
}

errType SemiaxisGetValue(void* fn)
{
    errType result=err_not_init;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    func->printResults();
    

    return result;
}


errType ControlMode(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE mode=*(BYTE*)func->getParamPtr(0);
    
    switch (mode)
    {
	case 0: sendFrame->setCPControl();     break; //Control panel control
	case 1: sendFrame->setRemoteControl(); break; //Remote panel control
	case 2: sendFrame->setLocalControl();  break; //Local panel control
    }
    
    func->printResults();

    return result;
}

errType StartFuncControl(void* fn)
{

    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;

    func->printParams();
    
    sendFrame->setStartFuncControl();
    
    func->printResults();
    
    return result;

}

errType CorrectHydroCyl(void* fn)
{

    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;

    func->printParams();
    
    sendFrame->setHydroCyl_correct();
    
    func->printResults();
    
    return result;

}
