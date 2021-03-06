#include <pthread.h>
#include "../myTypes.h"
#include "../buffer/buffer.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "SpecFuncs.h"
#include "functions.h"

specFuncsMgr::specFuncsMgr(srvAppLayer *appl)
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
    // 4. Set mutator status if it necessery (mutators are not allowed in automatic mode)
    // non initiated: non mutator status - function allowed in automatic mode
    // code: func->setMutatorStatus(true)
    //
    // 5. Setting timeout of waiting of the answer from the equipment
    // non initiated: no verification of link status
    //code: appLayer->set_timeout_equipment_answer(2)
    //
    // 6. Add function to Application interchange layer:
    //code: appLayer->CreateNewFunction(func);

    
	    func=new functionNode(1,0,2,GetFunctionMode);
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(1,1);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(2,1,2,GetCurrentCoordinates);
	    func->setParamDescriptor(0,1);
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(0,8);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(3,2,2,GetPointingExtraParams);
	    func->setParamDescriptor(0,1);
	    func->setParamDescriptor(1,4,false); // not scalar
	    func->setResultDescriptor(0,1);
	    func->setResultDescriptor(1,8);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(4,2,1,SetMode);
	    func->setParamDescriptor(0,1);
	    func->setParamDescriptor(1,255);
	    func->setResultDescriptor(0,1);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(5,3,1,SetCorrection);
	    func->setParamDescriptor(0,2);
	    func->setParamDescriptor(1,2);
	    func->setParamDescriptor(2,2);
	    func->setResultDescriptor(0,1);
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
	    appLayer->DeleteFunction(3);
	    appLayer->DeleteFunction(4);
	    appLayer->DeleteFunction(5);
    
    return result;
}
