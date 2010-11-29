#include <deque>                                                                                                  
#include <pthread.h>                                                                                              
#include <netinet/in.h>                                                                                           
#include "../../rcsLib/rcsLib.h"
#include "../buffer/ssBuffer.h"
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
	    func=new FunctionNode(1,0,1,SASC_PowerON);
	    func->setFuncName("Запрос на включение СКСЮ");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(2,0,1,SASC_PowerOFF);
	    func->setFuncName("Запрос на выключение СКСЮ");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(3,7,1,StartMeasuring);
	    func->setFuncName("Запрос на начало измерений");
	    
	    func->setParamDescriptor(0,type_DWORD);
	    func->setParamName(0, "binning");
	    func->setParamDescriptor(1,type_DWORD);
	    func->setParamName(1, "Niter");
	    func->setParamDescriptor(2,type_FLOAT);
	    func->setParamName(2, "FiH");
	    func->setParamDescriptor(3,type_FLOAT);
	    func->setParamName(3, "FiA");
	    func->setParamDescriptor(4,type_FLOAT);
	    func->setParamName(4, "TC");
	    func->setParamDescriptor(5,type_FLOAT);
	    func->setParamName(5, "Wm_c");
	    func->setParamDescriptor(6,type_FLOAT);
	    func->setParamName(6, "Wa");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    
	    func=new FunctionNode(4,6,1,ZeroMeasuring);
	    func->setFuncName("Запрос на измерение нуля");
	
	    func->setParamDescriptor(0,type_DWORD);
	    func->setParamName(0, "binning");
	    func->setParamDescriptor(1,type_FLOAT);
	    func->setParamName(1, "FiH");
	    func->setParamDescriptor(2,type_FLOAT);
	    func->setParamName(2, "FiA");
	    func->setParamDescriptor(3,type_FLOAT);
	    func->setParamName(3, "TC");
	    func->setParamDescriptor(4,type_FLOAT);
	    func->setParamName(4, "Wm_c");
	    func->setParamDescriptor(5,type_FLOAT);
	    func->setParamName(5, "Wa");
	    
	
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");

	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(5,0,1,linkTest);
	    func->setFuncName("Тест связи между сервером и АРМ СКСЮ");
	
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");

	    appLayer->CreateNewFunction(func);
	    
	    
	    
	    func=new FunctionNode(6,0,1,GetMeasuringResult);
	    func->setFuncName("Запрос получения результатов измерения");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(7,0,1,ZeroDB);
	    func->setFuncName("Запрос на обнуление базы измерений");
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(8,5,1,TaringStart);
	    func->setFuncName("Запрос на начало процесса тарировки");
	    
	    func->setParamDescriptor(0,type_FLOAT);
	    func->setParamName(0, "FiH");
	    func->setParamDescriptor(1,type_FLOAT);
	    func->setParamName(1, "FiA");
	    func->setParamDescriptor(2,type_FLOAT);
	    func->setParamName(2, "TC");
	    func->setParamDescriptor(3,type_FLOAT);
	    func->setParamName(3, "Wm_c");
	    func->setParamDescriptor(4,type_FLOAT);
	    func->setParamName(4, "Wa");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(9,12,1,GetTaringPoint);
	    func->setFuncName("Запрос на получение тарировочной точки");
	    
	    func->setParamDescriptor(0,type_DWORD);
	    func->setParamName(0, "binning");                             
	    func->setParamDescriptor(1,type_DWORD);
	    func->setParamName(1, "Niter");                               
	    func->setParamDescriptor(2,type_FLOAT);
	    func->setParamName(2, "FiH");
	    func->setParamDescriptor(3,type_FLOAT);
	    func->setParamName(3, "FiA");
	    func->setParamDescriptor(4,type_FLOAT);
	    func->setParamName(4, "TC");
	    func->setParamDescriptor(5,type_FLOAT);
	    func->setParamName(5, "Wm_c");
	    func->setParamDescriptor(6,type_FLOAT);
	    func->setParamName(6, "Wa");
	    func->setParamDescriptor(7,type_FLOAT);
	    func->setParamName(7, "X");
	    func->setParamDescriptor(8,type_FLOAT);
	    func->setParamName(8, "Y");
	    func->setParamDescriptor(9,type_FLOAT);
	    func->setParamName(9, "Z");
	    func->setParamDescriptor(10,type_FLOAT);
	    func->setParamName(10, "Fix");
	    func->setParamDescriptor(11,type_FLOAT);
	    func->setParamName(11, "Fiy");
	    
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
	    appLayer->CreateNewFunction(func);
	    
	    func=new FunctionNode(10,5,1,TaringStop);
	    func->setFuncName("Запрос остановки процесса тарировки");
	    
	    func->setParamDescriptor(0,type_FLOAT);
	    func->setParamName(0, "FiH");
	    func->setParamDescriptor(1,type_FLOAT);
	    func->setParamName(1, "FiA");
	    func->setParamDescriptor(2,type_FLOAT);
	    func->setParamName(2, "TC");
	    func->setParamDescriptor(3,type_FLOAT);
	    func->setParamName(3, "Wm_c");
	    func->setParamDescriptor(4,type_FLOAT);
	    func->setParamName(4, "Wa");
	    
	    func->setResultDescriptor(0,type_ERRTYPE);
	    func->setResultName(0, "Квитанция исполнения");
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