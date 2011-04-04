// Adaptive Optics Control Service
//--------------------------
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <storage/ssBuffer/ssBuffer.h>
#include <storage/buffer/buffer.h>
#include <peripheral/udp_port/udp_port.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>
#include <SrvAppLayer.h>
#include <global.h>

udp_port *equip_listen, *equip_sending;
buffer* equip_recvBuffer;



errType equipListenProcessing(BYTE *writingBuffer, size_t *sz)
{
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(int k=0; k<*sz; k++) printf("%.2X ", writingBuffer[k]);
    printf("]\n\n");
    printf("\t===========================================\n\n");
}


errType srvInit()
{

    printf("\tСлужба взаимодействия с АРМ \"Система получения изображения при помощи адаптивной оптики\"\n");
    printf("=======================================================================\n\n");
    
    equip_listen=new udp_port(eq_udp_listen_port);
    equip_listen->open_port();
    equip_sending=new udp_port(eq_udp_sending_port);
    equip_sending->open_port();
    equipAddr.s_addr=inet_addr(eq_ip_addr);
    //equip_recvBuffer=new buffer(sizeof(psStateVector_type));
    
    return err_result_ok;
}

errType srvDeinit()
{
    equip_listen->close_port();
    delete equip_listen;
    delete equip_sending;
    delete equip_recvBuffer;
    return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;
    
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType controlModeChange(void* fn)
{
    errType result=err_result_ok;
    
    functionNode* func=(functionNode*)fn;
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType getStateVector(void* fn)
{
    errType result=err_result_ok;

    functionNode* func=(functionNode*)fn;
    
    stateVector_type stateVector;
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

