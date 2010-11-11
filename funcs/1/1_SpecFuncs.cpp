#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <deque>
#include "../myTypes.h"
#include "../buffer/ssBuffer.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "SpecFuncs.h"
#include "functions.h"

SpecFuncs::SpecFuncs(ICAppLayer *appl)
{
    appLayer=appl;
}

SpecFuncs::~SpecFuncs()
{
}

errType SpecFuncs::StartSpecFuncs()
{
    errType result=err_not_init;
    FunctionNode* func;
        
    // SAMPLE FOR FUTURE PURPOSES
    //--------------------------------------------
    // Function #0..31. Write here function name
    //--------------------------------------------
    // 1. Function definition
    //    Function #0..31 have <k> parameters  and <n> result arguments
    //    Set pointer to function implementaion
    //code: func=new FunctionNode(<func_id>,<k>,<n>,<function_name>);
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
	    func=new FunctionNode(1,2,1,CreateGeneralSchedule);
	    func->setParamDescriptor(0,1);
	    func->setParamDescriptor(1,255,false); // is not scalar
	    func->setResultDescriptor(0,1);
	    appLayer->CreateNewFunction(func);
	    func=new FunctionNode(2,1,1,CreateEmergencySchedule);
	    func->setParamDescriptor(0,10, false); // is not scalar
	    func->setResultDescriptor(0,1);
	    appLayer->CreateNewFunction(func);
	    func=new FunctionNode(3,1,2,ReadGeneralSchedule);
	    func->setParamDescriptor(0,4);
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(1,10);
	    appLayer->CreateNewFunction(func);
	    func=new FunctionNode(4,0,2,ReadEmergencySchedule);
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(1,10);
	    appLayer->CreateNewFunction(func);
	    func=new FunctionNode(5,1,2,GetCursorPosition);
	    func->setParamDescriptor(0,1);
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(1,8);
	    appLayer->CreateNewFunction(func);
    return result;
}

errType SpecFuncs::StopSpecFuncs()
{
    errType result=err_not_init;
    
    // SAMPLE FOR FUTURE PURPOSES
    //--------------------------------------------
    // Function #0..31. Write here function name
    //--------------------------------------------
    //code: result=appLayer->DeleteFunction(<func_id>);
    
    return result;
}
