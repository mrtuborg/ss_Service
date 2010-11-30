#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "../buffer/ssBuffer.h"
#include "../../../rcsLib/rcsCmd/rcsCmd.h"
#include "../../../udp_port/udp_port.h"
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

	func=new FunctionNode(1,1,2,ShieldPowerON);
	func->setFuncName("Включить питание укрытия");
	func->setParamDescriptor(0,type_WORD);
	func->setParamName(0,"Время на раскрытие");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_WORD);
	func->setResultName(1,"Состояние гидросистемы");
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(2,0,1,ShieldPowerOFF);
	func->setFuncName("Выключить питание укрытия");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(3,0,1,HydroSystemPowerON);
	func->setFuncName("Включить питание гидросистемы");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(4,0,1,HydroSystemPowerOFF);
	func->setFuncName("Выключить питание гидросистемы");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(5,0,5,HydroSystemGetParams);
	func->setFuncName("Получить параметры гидросистемы");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Уровень жидкости");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Температура жидкости");
	func->setResultDescriptor(3, type_BYTE);
	func->setResultName(3,"Загрязнение фильтров");
	func->setResultDescriptor(4, type_BYTE);
	func->setResultName(4,"Давление в системе");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(6,1,1,FoldOpen);
	func->setFuncName("Открыть створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(7,1,1,FoldClose);
	func->setFuncName("Закрыть створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(8,1,1,FoldStop);
	func->setFuncName("Остановить створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	
	func=new FunctionNode(9,0,1,ShieldOpen);
	func->setFuncName("Открыть укрытие полностью");
	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(10,0,1,ShieldClose);
	func->setFuncName("Закрыть укрытие полностью");
	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(11,0,1,ShieldStop);
	func->setFuncName("Аварийный останов движения укрытия");
	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	
	
	func=new FunctionNode(12,1,3,FoldGetParams);
	func->setFuncName("Получить параметры створки");
	
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");
	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Код состояния");
	
	func->setResultDescriptor(2, type_WORD);
	func->setResultName(2,"Угол положения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(13,1,1,SemiaxisGetValue);
	func->setFuncName("Получить значения полуосей");

	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер полуоси");

	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Состояние полуоси");

	appLayer->CreateNewFunction(func);
		
	func=new FunctionNode(14,1,1,ControlMode);
	func->setFuncName("Переключить режим управления");
	
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Режим (0-ЦПУ, 1-ПДУ, 2-ПМУ)");
	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(16,0,1,StartFuncControl);
	func->setFuncName("Запуск процесса контроля функционирования САУ");

	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
	appLayer->CreateNewFunction(func);
	
	func=new FunctionNode(17,0,1,CorrectHydroCyl);
	func->setFuncName("Произвести коррекцию гидроцилиндров");

	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	
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
	
	appLayer->DeleteFunction(1);
	appLayer->DeleteFunction(2);
	appLayer->DeleteFunction(3);
	appLayer->DeleteFunction(4);
	appLayer->DeleteFunction(5);
	appLayer->DeleteFunction(6);
	appLayer->DeleteFunction(7);
	appLayer->DeleteFunction(8);
	appLayer->DeleteFunction(9);
	appLayer->DeleteFunction(10);
	appLayer->DeleteFunction(11);
	appLayer->DeleteFunction(12);
	appLayer->DeleteFunction(13);
	appLayer->DeleteFunction(14);
	appLayer->DeleteFunction(15);
	appLayer->DeleteFunction(16);
	appLayer->DeleteFunction(17);
	
    return result;
}
