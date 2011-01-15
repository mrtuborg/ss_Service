// Angular coordinates measuring system
//---------------------------------------
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include "../myTypes.h"
#include "../buffer/ssBuffer.h"
#include "../buffer/buffer.h"
#include "../udp/udp_port.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "../global.h"

udp_port *equip_listen=0, *equip_sending=0;
buffer* equip_recvBuffer=0;

typedef struct psFuncStateVector_type {
  union {
    WORD reserved: 13;
    BYTE MeasuringSystemState: 3; // 0 - power on, 1 - measuring, 2 - awaiting, 3 - emergency alert
  } fields;
  WORD value;
} psFuncStateVector_type;

errType EquipListenProcessing(BYTE *writingBuffer, size_t sz)
{
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(int k=0; k<sz; k++) printf("%.2X ", writingBuffer[k]);
    printf("]\n\n");
    printf("\t===========================================\n\n");
}


errType srvInit()
{

    printf("\tСлужба взаимодействия с АРМ \"Система измерения угловых координат\"\n");
    printf("=======================================================================\n\n");
    
    equip_listen=new udp_port(eq_udp_listen_port);
    equip_listen->open_port();
    equip_sending=new udp_port(eq_udp_sending_port);
    equip_sending->open_port();
    equipAddr.s_addr=inet_addr(eq_ip_addr);
    equip_recvBuffer=new buffer(256);
    
    return err_result_ok;
}

errType srvDeinit()
{
    if (equip_listen)		equip_listen->close_port();
    if (equip_listen)		delete equip_listen;
    if (equip_sending)		delete equip_sending;
    if (equip_recvBuffer)	delete equip_recvBuffer;

    return err_result_ok;
}

errType EmergencyShutdown(void* fn)
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;
    
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_result_ok;
    
    functionNode* func=(functionNode*)fn;
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_result_ok;

    functionNode* func=(functionNode*)fn;
    
    type_StateVector stateVector;
    stateVector=app->getStateVector();
    
    func->printParams();
    func->setResult(1,&stateVector);
    func->printResults();
    return result;
}

//============================================================


errType StartMeasuringProcess(void* fn) // func_id=1
{
    errType result=err_result_ok;

    BYTE res=0;
    BYTE mode=1;

    functionNode* func=(functionNode*)fn;
    func->printParams();
    equip_sending->sendData(equipAddr, (BYTE*)&mode, 1);
    
//    func->setResult(0,&res);
    
    func->printResults();
    
    return result;
}


errType FinishMeasuringProcess(void* fn) // func_id=2
{
    errType result=err_result_ok;

    functionNode* func=(functionNode*)fn;
    
    BYTE res=0;
    BYTE mode=3;
    
    func->printParams();    
    equip_sending->sendData(equipAddr, (BYTE*)&mode, 1);

    
//    func->setResult(0,&res,type_ERRTYPE);
    
    func->printResults();
        
    return result;
}

