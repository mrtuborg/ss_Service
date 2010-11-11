// Tube Devices Control Service
//------------------------------
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "../myTypes.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../buffer/buffer.h"
#include "../udp/udp_port.h"
#include "../global.h"

udp_port* equipment;

errType srvInit()
{
    equipment=new udp_port(EQ_UDP_PORT);
    equipment->open_port();
    equipAddr.s_addr=inet_addr(EQ_IP_ADDR);
    return err_result_ok;
}

errType srvDeinit()
{
    equipment->close_port();
    delete equipment;
    return err_result_ok;
}

void PollingThreadFunc(void*)
{


}

errType EmergencyShutdown(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** EmergencyShutdown was called! ***\n");
    printf("*************************************\n");
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** ControlModeChange was called! ***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** GetStateVector was called! ***\n");
    printf("*************************************\n");

    FunctionNode* func=(FunctionNode*)fn;
    
    func->dbgPrint();

    return result;
}
