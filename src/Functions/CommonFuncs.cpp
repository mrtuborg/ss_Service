#include <pthread.h>
#include <netinet/in.h>
#include <queue>
#include "../myTypes.h"
#include "../buffer/ssBuffer.h"
#include "../ICAppLayer/cmd.h"
#include "../udp/udp_port.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "CommonFuncs.h"
#include "functions.h"



void* EquipListenPolling(void* user)                                             
{                                                                               
    ICAppLayer *app=(ICAppLayer*)user;                                          
    DWORD sz=1024;                                                              
    BYTE *writingBuffer;                                                        
    writingBuffer=new BYTE[256];                                               
    errType result=err_not_init;                                                
    
    while (!app->terminated())                                                  
    {                                                                           
        // Listening equipment answer - status vector:                          

    	    result=app->equip_reading_event();
    	    

        if (result==err_result_ok) {
	    app->equip_read_data(writingBuffer, (size_t*)&sz);
	    EquipListenProcessing(writingBuffer, sz);
	}
	sched_yield();
    }                                                                           
    delete writingBuffer;                                                       
}

CommonFuncs::CommonFuncs(ICAppLayer *appl)
{
    appLayer=appl;
}

CommonFuncs::~CommonFuncs()
{
}

errType CommonFuncs::StartCommonFuncs()
{
    errType result=err_not_init;
    FunctionNode* func;
    
    // Function #32. EmergencyShtudown
    //----------------------------------
    // 1. Function definition
    //    Function #32 have 1 result argument and haven't parameters
    //    Set pointer to function implementaion
    func=new FunctionNode(32,0,1,EmergencyShutdown);
    func->setFuncName("Аварийное завершение подсистемы");
    // 2. Definitions for parameters: not needed
    //func->setParamDescriptor();
    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");

    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    // Function #33. ControlModeChange
    //---------------------------------
    // 1. Function definition
    //    Function #33 have 1 parameter and 1 result argument
    //    Set pointer to function implementaion
    func=new FunctionNode(33,1,1,ControlModeChange);
    func->setFuncName("Смена оперативного режима службы");
    // 2. Definitions for parameters:
    func->setParamDescriptor(0,type_BYTE);
    func->setParamName(0,"Новый режим");

    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE); 
    func->setResultName(0,"Квитанция исполнения");
    
    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    // Function #34. GetStateVector
    //-------------------------------
    // 1. Function definition
    //    Function #34 have 0 parameters and 1 result argument
    //    Set pointer to function implementaion
    func=new FunctionNode(34,0,2,GetStateVector);
    func->setFuncName("Запрос вектора состояния службы");

    // 2. Definitions for parameters: not needed
    //func->setParamDescriptor(0,1);
    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE); 
    func->setResultName(0,"Квитанция исполнения");

    func->setResultDescriptor(1, type_DWORD); 
    func->setResultName(1,"Вектор состояния службы");

    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    return result;
}

errType CommonFuncs::StopCommonFuncs()
{
    errType result=err_not_init;
    // Function #32. EmergencyShutdown
    //----------------------------------
    result=appLayer->DeleteFunction(32);
    
    // Function #33. ControlModeChange
    //----------------------------------
    result=appLayer->DeleteFunction(33);
    
    // Function #34. GetStateVector
    //-------------------------------
    result=appLayer->DeleteFunction(34);
    
    return result;
}
