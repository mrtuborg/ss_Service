// SASC Service
//---------------
#include <stdio.h>                                                                                                
#include <string.h>                                                                                               
#include <netinet/in.h>                                                                                           
#include <arpa/inet.h>                                                                                            
#include <deque>                                                                                                  
#include "../myTypes.h"                                                                                           
#include "../buffer/ssBuffer.h"                                                                                   
#include "../buffer/buffer.h"                                                                                     
#include "../ICAppLayer/cmd.h"                                                                                    
#include "../udp/udp_port.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"                                                                
#include "../ICAppLayer/FunctionNode/FunctionNode.h"                                                              
#include "../ICAppLayer/ICAppLayer.h"                                                                             
#include "../global.h"

#include "SASC_packet/comm_SASC.h"

udp_port *equip_sending;                                                                           

comm_SASC SASCmsg;
SASC_answer_mod typeinf;
    
errType EquipListenProcessing(BYTE *writingBuffer, size_t sz)
{
    if (sz>sizeof(SASC_msg_type)) sz=sizeof(SASC_msg_type);
    
    SASCmsg.encode(writingBuffer, sz);
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(int k=0; k<sz; k++) printf("%.2X ", writingBuffer[k]);
    //equip_recvBuffer->unlockBufferChunkForExternWriting(sz);
    printf("]\n\n");
    printf("\tРасшифровка:\n");
    if (SASCmsg.checkAnswer(&typeinf)==err_result_ok) {
	printf("\tПринята ответная квитанция: %s\n", SASC_answer_str[typeinf]);
    } else {
	printf("\tМодификатор ответа не распознан. Обобщённое сообщение:\n");
	SASCmsg.dbgPrint();
    }
    printf("\t===========================================\n\n");
}                                                                                                         


errType srvInit()
{
    errType result=err_not_init;
    printf("\tСлужба системы обеспечения  СКСЮ\n");
    printf("=============================================================\n\n");
//-    equip_listen=new udp_port(eq_udp_listen_port);
//-    result=equip_listen->open_port(true);
    
    //if (result==err_result_ok)
    {
	equip_sending=new udp_port(eq_udp_sending_port);
	result=equip_sending->open_port();
	equipAddr.s_addr=inet_addr(eq_ip_addr);
	//equip_recvBuffer=new buffer(2*sizeof(SASC_msg_type));
    }
    return result;
}

errType srvDeinit()
{
    equip_sending->close_port();
    //delete equip_listen;
    delete equip_sending;
    //delete equip_recvBuffer;
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
    func->printParams();
    func->setResult(1,&app->getStateVector());
    func->printResults();
    return result;

}


errType SASC_PowerON(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_power_on);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType SASC_PowerOFF(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_power_off);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType StartMeasuring(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *params[6];
    //binning
    params[0]=(BYTE*)func->getParamPtr(0);
    //Niter
    params[1]=(BYTE*)func->getParamPtr(1);
    //FiH
    params[2]=(BYTE*)func->getParamPtr(2);
    //FiA
    params[3]=(BYTE*)func->getParamPtr(3);
    //TC
    params[4]=(BYTE*)func->getParamPtr(4);
    //Wm_c
    params[5]=(BYTE*)func->getParamPtr(5);
    //Wa
    params[6]=(BYTE*)func->getParamPtr(6);
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_measure, params);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    
    return result;
}

errType ZeroMeasuring(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *params[6];
    //binning
    params[0]=(BYTE*)func->getParamPtr(0);
    //FiH
    params[1]=(BYTE*)func->getParamPtr(1);
    //FiA
    params[2]=(BYTE*)func->getParamPtr(2);
    //TC
    params[3]=(BYTE*)func->getParamPtr(3);
    //Wm_c
    params[4]=(BYTE*)func->getParamPtr(4);
    //Wa
    params[5]=(BYTE*)func->getParamPtr(5);
    
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_zero_measure, params);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType GetMeasuringResult(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_get_db);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType ZeroDB(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_erase_db);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType TaringStart(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();

    BYTE *params[5];

    //FiH
    params[0]=(BYTE*)func->getParamPtr(0);
    //FiA
    params[1]=(BYTE*)func->getParamPtr(1);
    //TC
    params[2]=(BYTE*)func->getParamPtr(2);
    //Wm_c
    params[3]=(BYTE*)func->getParamPtr(3);
    //Wa
    params[4]=(BYTE*)func->getParamPtr(4);
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_tare_start, params);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType GetTaringPoint(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *params[12];
    //binning
    params[0]=(BYTE*)func->getParamPtr(0);
    //Niter
    params[1]=(BYTE*)func->getParamPtr(1);
    //FiH
    params[2]=(BYTE*)func->getParamPtr(2);
    //FiA
    params[3]=(BYTE*)func->getParamPtr(3);
    //TC
    params[4]=(BYTE*)func->getParamPtr(4);
    //Wm_c
    params[5]=(BYTE*)func->getParamPtr(5);
    //Wa
    params[6]=(BYTE*)func->getParamPtr(6);

    //X
    params[7]=(BYTE*)func->getParamPtr(7);

    //Y
    params[8]=(BYTE*)func->getParamPtr(8);

    //Z
    params[9]=(BYTE*)func->getParamPtr(9);

    //Fix
    params[10]=(BYTE*)func->getParamPtr(10);

    //Fiy
    params[11]=(BYTE*)func->getParamPtr(11);

    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_get_tare_value, params);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    return result;
}

errType TaringStop(void* fn)
{
    errType result=err_result_ok;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    BYTE *params[5];
    
    //FiH
    params[0]=(BYTE*)func->getParamPtr(0);
    //FiA
    params[1]=(BYTE*)func->getParamPtr(1);
    //TC
    params[2]=(BYTE*)func->getParamPtr(2);
    //Wm_c
    params[3]=(BYTE*)func->getParamPtr(3);
    //Wa
    params[4]=(BYTE*)func->getParamPtr(4);
    
    
    BYTE *frame;
    frame=new BYTE[sizeof(SASC_msg_type)];
    SASCmsg.apply_mod(_tare_stop,params);
    SASCmsg.decode(&frame);
    equip_sending->sendData(equipAddr, frame, sizeof(SASC_msg_type));
    delete frame;

    func->printResults();
    
    return result;
}
