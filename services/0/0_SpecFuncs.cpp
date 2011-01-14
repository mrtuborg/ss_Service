#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <deque>
#include "../../ortsTypes/ortsTypes.h"
#include "../buffer/ssBuffer.h"
#include "../../rcsLib/rcsCmd/rcsCmd.h"
#include "../../udp_port/udp_port.h"
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
    // 4. Add function to Application interchange layer:
    //code: appLayer->CreateNewFunction(func);
    
    
    
    
	    
	    func=new functionNode(1,0,2,GetProcessesList);
	    func->setFuncName("Запрос идентификаторов списка подконтрольных процессов");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultDescriptor(1,type_WORDVECTOR); // is not scalar
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(2,1,10,GetProcessInfo);
	    func->setFuncName("Запрос информации о процессе");
	    func->setParamDescriptor(0,type_WORD);
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    
	    func->setResultDescriptor(1,type_WORD); 	  // ppid
	    func->setResultName(1,"Идентификатор родительского процесса");
	    
	    func->setResultDescriptor(2,type_WORD); 	  // euid
	    func->setResultName(2,"Идентификатор пользователя запустившего процесс");
	    
	    func->setResultDescriptor(3,type_BYTE); 	  // cpu
	    func->setResultName(3,"Доля используемого времени центрального процессора");
	    
	    func->setResultDescriptor(4,type_CHARVECTOR); // state
	    func->setResultName(4,"Строка маркеров состояния");
	    
	    func->setResultDescriptor(5,type_QWORD); // state
	    func->setResultName(5,"Время работы процесса");
	    
	    func->setResultDescriptor(6,type_DWORD); 	  // vsz
	    func->setResultName(6,"Всего задействовано памяти под процесс");
	    
	    func->setResultDescriptor(7,type_DWORD); 	  // rss
	    func->setResultName(7,"Объём задействованной памяти для резидентой части процесса");
	    
	    func->setResultDescriptor(8,type_CHARVECTOR); // cmdline
	    func->setResultName(8,"Строка запуска");
	    
	    func->setResultDescriptor(9,type_CHARVECTOR); // environment
	    func->setResultName(9,"Переменные среды окружения");
	    
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(3,0,1,RebootSystem);
	    func->setResultDescriptor(0,type_ERRTYPE);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(4,1,2,SearchProcess);
	    func->setFuncName("Поиск идентификатора");
	    func->setParamDescriptor(0, type_CHARVECTOR);
	    func->setParamName(0, "Часть имени процесса");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    
	    func->setResultDescriptor(1,type_WORD);
	    func->setResultName(1,"Идентификатор процесса");
	    
	    
	    appLayer->CreateNewFunction(func);
    

	    func=new functionNode(5,1,1,KillProcess);
	    func->setFuncName("Завершить процесс");
	    func->setParamDescriptor(0,type_WORD);
	    func->setParamName(0,"Идентификатор процесса");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(6,1,1,RestartProcess);
	    func->setParamDescriptor(0,type_ERRTYPE);
	    func->setResultDescriptor(0,type_ERRTYPE);
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(7,4,1,InstallFilter);
	    func->setFuncName("Установить фильтр");
	    func->setParamDescriptor(0,type_CHARVECTOR);
	    func->setParamName(0, "Строка запуска");
	    func->setParamDescriptor(1,type_BYTE);
	    func->setParamName(1, "Максимальная нагрузка на ЦП(%)");
	    func->setParamDescriptor(2,type_DWORD);
	    func->setParamName(2, "Максимальный объём используемой памяти(Кб)");
	    func->setParamDescriptor(3,type_BYTE);
	    func->setParamName(3, "Реакция на превышение значения(0/1)");
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
	appLayer->DeleteFunction(3);
	appLayer->DeleteFunction(4);
	appLayer->DeleteFunction(5);
    return result;
}
