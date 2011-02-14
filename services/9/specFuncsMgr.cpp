#include <deque>
#include <pthread.h>
#include <netinet/in.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <storage/ssBuffer/ssBuffer.h>
#include <comm/udp_port/udp_port.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>
#include <SrvAppLayer.h>
#include <specFuncsMgr.h>
#include "functions.h"

specFuncsMgr::specFuncsMgr(SrvAppLayer *appl)
{
    appLayer=appl;
}

specFuncsMgr::~specFuncsMgr()
{
}

errType specFuncsMgr::startSpecFuncs()
{
    errType result=err_not_init;
    functionNode* func;
        
    // SAMPLE FOR FUTURE PURPOSES
    //--------------------------------------------
    // Function #0..31. Write here function name
    //--------------------------------------------
    // 1. Function definition
    //    Function #0..31 have <k> parameters  and <n> result arguments
    //    Set pointer to function implementaion
    //code: func=new functionNode(<func_id>,<k>,<n>,<function_name>);
    //
    // 2. Definitions for parameters:
    //code: func->setParamDescriptor(<k[i]>,<k[i]_length>);
    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    //code: func->setResultDescriptor(0, 2);
    //
    // 4. Add function to Application interchange layer:
    //code: appLayer->CreateNewFunction(func);
    
	    func=new functionNode(1,0,1,StartMeasuringProcess);
	    func->setFuncName("Запрос включения системы адаптивной оптики");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(2,0,1,FinishMeasuringProcess);
	    func->setFuncName("Запрос отключения системы адаптивной оптики");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    
	    
	    appLayer->CreateNewFunction(func);
	    
    return result;
}

errType specFuncsMgr::stopSpecFuncs()
{
    errType result=err_not_init;
    
    // SAMPLE FOR FUTURE PURPOSES
    //--------------------------------------------
    // Function #0..31. Write here function name
    //--------------------------------------------
    //code: result=appLayer->DeleteFunction(<func_id>);
    
	    appLayer->DeleteFunction(1);
	    appLayer->DeleteFunction(2);
    
    return result;
}
