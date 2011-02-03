#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <ssBuffer.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <comm/udp_port/udp_port.h>
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
    // 4. Add function to Application interchange layer:
    //code: appLayer->CreateNewFunction(func);

    func = new functionNode(1,1,2,shieldPowerON);
    func->setFuncName("Включить питание укрытия");
    func->setParamDescriptor(0,type_WORD);
    func->setParamName(0,"Время на раскрытие");
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");
    func->setResultDescriptor(1, type_WORD);
    func->setResultName(1,"Состояние гидросистемы");
    func->setMutatorStatus(true);
    appLayer->CreateNewFunction(func);

    func = new functionNode(2,0,1,shieldPowerOFF);
    func->setFuncName("Выключить питание укрытия");
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");
    func->setMutatorStatus(true);
    appLayer->CreateNewFunction(func);

    func = new functionNode(3,0,1,hydroSystemPowerON);
    func->setFuncName("Включить питание гидросистемы");
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");
    func->setMutatorStatus(true);
    appLayer->CreateNewFunction(func);

    func = new functionNode(4,0,1,hydroSystemPowerOFF);
    func->setFuncName("Выключить питание гидросистемы");
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");
    func->setMutatorStatus(true);
    appLayer->CreateNewFunction(func);

    func = new functionNode(5,0,6,hydroSystemGetParams);
    func->setFuncName("Получить параметры гидросистемы");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Давление в системе");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Уровень жидкости");
	func->setResultDescriptor(3, type_BYTE);
	func->setResultName(3,"Температура жидкости");
	func->setResultDescriptor(4, type_BYTE);
	func->setResultName(4,"Загрязнение фильтров");
	func->setResultDescriptor(5, type_BYTE);
	func->setResultName(5,"Состояние гидросистемы");
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(6,1,2,foldOpen);
	func->setFuncName("Открыть створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер створки");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);

	func = new functionNode(7,1,2,foldClose);
	func->setFuncName("Закрыть створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер створки");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);

	func = new functionNode(8,1,2,foldStop);
	func->setFuncName("Остановить створку");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");	
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер створки");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(9,0,1,shieldOpen);
	func->setFuncName("Открыть укрытие полностью");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(10,0,1,shieldClose);
	func->setFuncName("Закрыть укрытие полностью");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(11,0,1,shieldStop);
	func->setFuncName("Аварийный останов движения укрытия");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(12,1,4,foldGetParams);
	func->setFuncName("Получить параметры створки");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер створки");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Код состояния");
	func->setResultDescriptor(3, type_WORD);
	func->setResultName(3,"Угол положения");
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(13,3,5,semiaxisSensorsGetState);
	func->setFuncName("Получить значения полуосей");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер створки");
	func->setParamDescriptor(1, type_BYTE);
	func->setParamName(1,"Номер полуоси");
	func->setParamDescriptor(2, type_BYTE);
	func->setParamName(2,"Номер датчика");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер створки");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Номер полуоси");
	func->setResultDescriptor(3, type_BYTE);
	func->setResultName(3,"Номер датчика");
	func->setResultDescriptor(4, type_BYTE);
	func->setResultName(4,"Состояние датчика полуоси");
	appLayer->CreateNewFunction(func);
		
	func = new functionNode(14,1,3,getUZstate);
	func->setFuncName("Получить вектор состояния УЗ");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Номер УЗ");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Номер УЗ");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Вектор состояния");
	appLayer->CreateNewFunction(func);

	func = new functionNode(15,1,1,changeControlMode);
	func->setFuncName("Переключить режим управления");
	func->setParamDescriptor(0, type_BYTE);
	func->setParamName(0,"Режим (0-ЦПУ, 1-ПДУ, 2-ПМУ)");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);

	func = new functionNode(16,0,2,getControlMode);
	func->setFuncName("Запросить значение выбранного пульта управления");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Режим (0-ЦПУ, 1-ПДУ, 2-ПМУ)");
	appLayer->CreateNewFunction(func);

	func = new functionNode(17,0,1,startFuncControl);
	func->setFuncName("Запуск процесса контроля функционирования САУ");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(18,0,1,correctHydroCyl);
	func->setFuncName("Произвести коррекцию гидроцилиндров");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setMutatorStatus(true);
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(19,0,7,allFoldsGetParams);
	func->setFuncName("Получить параметры всех створок");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Код состояния створки 0");
	func->setResultDescriptor(2, type_WORD);
	func->setResultName(2,"Угол положения створки 0");
	func->setResultDescriptor(3, type_BYTE);
	func->setResultName(3,"Код состояния створки 1");
	func->setResultDescriptor(4, type_WORD);
	func->setResultName(4,"Угол положения створки 1");
	func->setResultDescriptor(5, type_BYTE);
	func->setResultName(5,"Код состояния створки 2");
	func->setResultDescriptor(6, type_WORD);
	func->setResultName(6,"Угол положения створки 2");
	appLayer->CreateNewFunction(func);
	
	func = new functionNode(20,0,2,allSemiaxisSensorsGetState);
	func->setFuncName("Получить значения всех датчиков полуосей");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_WORD);
	func->setResultName(1,"Вектор состояния датчиков");
	appLayer->CreateNewFunction(func);

	func = new functionNode(21,0,4,getAllUZstate);
	func->setFuncName("Получить вектор состояния УЗ");
	func->setResultDescriptor(0, type_ERRTYPE);
	func->setResultName(0,"Квитанция исполнения");
	func->setResultDescriptor(1, type_BYTE);
	func->setResultName(1,"Вектор состояния УЗ верхней створки");
	func->setResultDescriptor(2, type_BYTE);
	func->setResultName(2,"Вектор состояния УЗ нижней створки Б");
	func->setResultDescriptor(3, type_BYTE);
	func->setResultName(3,"Вектор состояния УЗ нижней створки А");
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
