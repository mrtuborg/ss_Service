#include <deque>
#include <pthread.h>
#include <netinet/in.h>
#include "../myTypes.h"
#include "../buffer/ssBuffer.h"
#include "../ICAppLayer/cmd.h"
#include "../udp/udp_port.h"
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

    
    extern errType SetProgrammMode(void*);                                                                            
    extern errType SetKNMode(void*);                                                                                  
    extern errType SetZVVMode(void*);                                                                                 
    extern errType SetSearchingMode(void*);                                                                           
    extern errType StopSearchingMode(void*);
    
	    func=new functionNode(1,0,2,GetFunctionMode);
	    func->setFuncName("Запрос текущего функционального режима");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    func->setResultDescriptor(1,type_BYTE);
	    func->setResultName(1,"Режим функционирования");
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(2,0,7,GetCurrentCoordinates);
	    func->setFuncName("Запрос текущих координат системы наведения");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    func->setResultDescriptor(1,type_QWORD);
	    func->setResultName(1,"Текущее календарное время");
	    func->setResultDescriptor(2,type_QWORD);
	    func->setResultName(2,"Текущее звёздное время");
	    func->setResultDescriptor(3,type_DOUBLE);
	    func->setResultName(3,"Азимут ДОС");
	    func->setResultDescriptor(4,type_DOUBLE);
	    func->setResultName(4,"Угол места ДОС");
            func->setResultDescriptor(5,type_DOUBLE);
	    func->setResultName(5,"Азимут программный");
	    func->setResultDescriptor(6,type_DOUBLE);
	    func->setResultName(6,"Угол места программный");

	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(3,0,14,GetPointingExtraParams);
	    func->setFuncName("Запрос расширенных параметров системы АРМ");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    func->setResultDescriptor(1,type_DWORD);
	    func->setResultName(1,"Поправка по азимуту");
	    func->setResultDescriptor(2,type_DWORD);
	    func->setResultName(2,"Поправка по углу места");
	    func->setResultDescriptor(3,type_DWORD);
	    func->setResultName(3,"Поправка по времени");
	    func->setResultDescriptor(4,type_DWORD);
	    func->setResultName(4,"Программная скорость по азимуту");
	    func->setResultDescriptor(5,type_DWORD);
	    func->setResultName(5,"Фактическая скорость по азимуту");
	    func->setResultDescriptor(6,type_DWORD);
	    func->setResultName(6,"Программная скорость по углу места");
	    func->setResultDescriptor(7,type_DWORD);
	    func->setResultName(7,"Фактическая скорость по углу места");
	    func->setResultDescriptor(8,type_DWORD);
	    func->setResultName(8,"Пеленг по азимуту");
	    func->setResultDescriptor(9,type_DWORD);
	    func->setResultName(9,"Пеленг по углу места");
	    func->setResultDescriptor(10,type_DWORD);
	    func->setResultName(10,"Привязка по азимуту");
	    func->setResultDescriptor(11,type_DWORD);
	    func->setResultName(11,"Привязка по углу места");
	    func->setResultDescriptor(12,type_WORD);
	    func->setResultName(12,"Состояние аппаратуры");
	    func->setResultDescriptor(13,type_BYTE);
	    func->setResultName(13,"Режим поля");
	    appLayer->CreateNewFunction(func);
	    
	    
	    func=new functionNode(4,3,1,SetCorrection);
	    func->setFuncName("Задать поправки АРМ");
	    
	    func->setParamDescriptor(0,type_DWORD);
	    func->setParamName(0,"Поправка по азимуту");
	    func->setParamDescriptor(1,type_DWORD);
	    func->setParamName(1,"Поправка по углу места");
	    func->setParamDescriptor(2,type_DWORD);
	    func->setParamName(2,"Поправка времени");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    
	    appLayer->CreateNewFunction(func);
	    func=new functionNode(5,0,1,SetProgrammMode);
	    func->setFuncName("Переключиться в режим программного наведения");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(6,0,1,SetKNMode);
	    func->setFuncName("Переключиться в режим программного наведения");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	        
	    func=new functionNode(7,0,1,SetZVVMode);
	    func->setFuncName("Переключиться в режим программного наведения");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(8,3,1,StartSearchingMode);
	    func->setFuncName("Включить режим поиска");
	    
	    func->setParamDescriptor(0,type_BYTE);
	    func->setParamName(0,"Число витков");
	    func->setParamDescriptor(1,type_BYTE);
	    func->setParamName(1,"Величина шага");
	    func->setParamDescriptor(2,type_BYTE);
	    func->setParamName(2,"Время стояния");
	    
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0,"Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new functionNode(9,0,1,StopSearchingMode);
	    func->setFuncName("Отключить режим поиска");
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
