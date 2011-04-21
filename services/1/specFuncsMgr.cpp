#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <storage/ssBuffer/ssBuffer.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <peripheral/udp_port/udp_port.h>
#include <param_desc.h>
#include <functionNode.h>
#include <SrvAppLayer.h>
#include <specFuncsMgr.h>
#include <functions.h>

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
	    func = new functionNode(1,10,2,addScheduleJob);
	    func->setMutatorStatus(true);
	    func->setFuncName("Добавить пакетное задание");
	    func->setParamDescriptor(0, type_BYTE);
	    func->setParamName(0,"Идентификатор расписания");

	    func->setParamDescriptor(1, type_DWORD);
	    func->setParamName(1,"Идентификатор операции");

	    func->setParamDescriptor(2, type_DWORD);
	    func->setParamName(2,"Идентификатор следующей операции");

	    func->setParamDescriptor(3, type_DWORD);
	    func->setParamName(3,"Время начала операции");

	    func->setParamDescriptor(4, type_DWORD);
	    func->setParamName(4,"Время окончания операции");

	    func->setParamDescriptor(5, type_DWORD);
	    func->setParamName(5,"IPv4-адресс службы");

	    func->setParamDescriptor(6, type_WORD);
	    func->setParamName(6,"UDP порт службы");

	    func->setParamDescriptor(7, type_BYTE);
	    func->setParamName(7,"Идентификатор службы-исполнителя");

	    func->setParamDescriptor(8, type_BYTE);
	    func->setParamName(8,"Идентификатор функции-исполнителя");

	    func->setParamDescriptor(9, type_BYTEVECTOR);
	    func->setParamName(9,"Параметрическая часть");

	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    
	    func->setResultDescriptor(1,type_DWORD);         /// bug: if not initialize same as declared - programm crashes
	    func->setResultName(1, "Расширенный результат");

	    appLayer->CreateNewFunction(func);

	    func = new functionNode(2, 1, 1, runSchedule);
		func->setMutatorStatus(true);
		func->setFuncName("Запустить пакетное задание на исполнение");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");

		appLayer->CreateNewFunction(func);

		func = new functionNode(3, 1, 1, stopSchedule);
		func->setMutatorStatus(true);
		func->setFuncName("Остановить выполнение пакетного задания");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");

		appLayer->CreateNewFunction(func);

	//extern errType readJobState(void* fn)
		func = new functionNode(4, 2, 3, readJobState);
		func->setMutatorStatus(false);
		func->setFuncName("Запрос статуса операции");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");
		func->setParamDescriptor(1, type_DWORD);
		func->setParamName(1,"Идентификатор операции");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");
		func->setResultDescriptor(1,type_BYTE);
		func->setResultName(1, "Статус операции");
		func->setResultDescriptor(2,type_BYTEVECTOR);
		func->setResultName(2, "Ответ исполнителя");

		appLayer->CreateNewFunction(func);

	//extern errType getCursorPosition(void* fn)
		func = new functionNode(5, 0, 1, getCursorPosition);
		func->setMutatorStatus(false);
		func->setFuncName("Запрос идентификатора выполняющегося задания");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");
		func->setResultDescriptor(1, type_DWORD);
		func->setResultName(1,"Идентификатор операции");

		appLayer->CreateNewFunction(func);

	//extern errType readJobEntity(void* fn)
		func = new functionNode(6, 2, 8, readJobEntity);
		func->setMutatorStatus(false);
		func->setFuncName("Запрос содержимого операции");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");
		func->setParamDescriptor(1, type_DWORD);
		func->setParamName(1,"Идентификатор операции");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");
		func->setResultDescriptor(1, type_DWORD);
		func->setResultName(1,"Идентификатор следующей операции");
		func->setResultDescriptor(2, type_DWORD);
		func->setResultName(2,"Время начала операции");
		func->setResultDescriptor(3, type_DWORD);
		func->setResultName(3,"Длительность времени исполнения операции");
		func->setResultDescriptor(4, type_DWORD);
		func->setResultName(4,"IPv4-адресс службы");
		func->setResultDescriptor(5, type_WORD);
		func->setResultName(5,"UDP порт службы");
		func->setResultDescriptor(6, type_BYTE);
		func->setResultName(6,"Идентификатор службы-исполнителя");
		func->setResultDescriptor(7, type_BYTE);
		func->setResultName(7,"Идентификатор функции-исполнителя");
		func->setResultDescriptor(8, type_BYTEVECTOR);
		func->setResultName(8,"Параметрическая часть");

		appLayer->CreateNewFunction(func);

		//extern errType getOpsId(void* fn)
		func = new functionNode(7, 1, 2, getOpsId);
		func->setMutatorStatus(false);
		func->setFuncName("Запрос списка идентификаторов операций");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");
		func->setResultDescriptor(1, type_DWORDVECTOR);
		func->setResultName(1,"Массив идентификаторов операций");

		appLayer->CreateNewFunction(func);

		//extern errType executeJob(void* fn)
		func = new functionNode(8, 2, 1, executeJob);
		func->setMutatorStatus(true);
		func->setFuncName("Запустить операцию на исполнение");
		func->setParamDescriptor(0, type_BYTE);
		func->setParamName(0,"Идентификатор пакетного задания");
		func->setParamDescriptor(1, type_DWORD);
		func->setParamName(1,"Идентификатор операции");
		//func->setParamDescriptor(2, type_BYTE);
		//func->setParamName(2,"Отложить исполнение, cек");

		func->setResultDescriptor(0,type_ERRTYPE);
		func->setResultName(0, "Квитанция исполнения");
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
