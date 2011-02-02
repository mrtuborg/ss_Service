#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <ssBuffer.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <comm/udp_port/udp_port.h>
#include <param_desc.h>
#include <functionNode.h>
#include <srvAppLayer.h>
#include <specFuncsMgr.h>
#include <functions.h>

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
    // 4. Add function to Application interchange layer:
    //code: appLayer->CreateNewFunction(func);
	    func=new functionNode(1,7,2,addScheduleJob);
	    func->setFuncName("Добавить пакетное задание");
	    func->setParamDescriptor(0, type_BYTE);
	    func->setParamName(0,"Признак аварийной операции");

	    func->setParamDescriptor(1, type_DWORD);
	    func->setParamName(1,"Идентификатор операции");

	    func->setParamDescriptor(2, type_DWORD);
	    	func->setParamName(2,"Идентификатор следующей операции");

	    func->setParamDescriptor(3, type_WORD);
	    func->setParamName(3,"Время начала операции");

	    func->setParamDescriptor(4, type_WORD);
	    func->setParamName(4,"Время окончания операции");

	    func->setParamDescriptor(5, type_BYTE);
	    func->setParamName(5,"Идентификатор службы-исполнителя");

	    func->setParamDescriptor(6, type_BYTE);
	    func->setParamName(6,"Идентификатор функции-исполнителя");

	    //func->setParamDescriptor(6, type_WORD);
	    //func->setParamName(6,"Длина параметрической части");

	    func->setParamDescriptor(7, type_BYTE);
	    func->setParamName(7,"Параметрическая часть");

	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    
	    func->setResultDescriptor(1,type_DWORD);         /// bug: if not initialize same as declared - programm crashes
	    func->setResultName(1, "Расширенный результат");

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
    
    return result;
}
