/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class commonFuncsMgr interface header.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <pthread.h>
#include <netinet/in.h>
#include <queue>

#include <extra/ortsTypes/ortsTypes.h>
#include <ssBuffer.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <peripheral/udp_port/udp_port.h>
#include <param_desc.h>
#include <functionNode.h>
#include <SrvAppLayer.h>
#include <commonFuncsMgr.h>
#include <functions.h>


/**********************************************************************************//**
 * @brief      Thread to polling listen udp for equipment data.
 *
 * @details    Calls equipListenProcessing for decoding data received from equipment
 * @retval     user
 **************************************************************************************/
void* equipListenPolling(void* user)                                             
{                                                                               
    SrvAppLayer *app = (SrvAppLayer*)user;
    DWORD   sz (1024);
    BYTE    *writingBuffer;
    errType result (err_not_init);
    writingBuffer = new BYTE[256];
    DWORD timeout;

    while (!app->terminated())
    {                                                                           
        /// @todo Listening equipment answer - status vector:
        timeout = app->get_equipment_timeOut_Value();


        result = app->equip_reading_event(timeout);

        if (result == err_timeout && app->is_awaiting_equip_answer())  {
            printf("Разрыв связи с оборудованием!\n\n");
            app->set_state_vector_linked(false);
        }

        if (result == err_result_ok) {
	    app->equip_read_data(writingBuffer, (size_t*)&sz);
	    equipListenProcessing(writingBuffer, sz);
            app->set_awaiting_equip_answer(false);
            if (!app->get_state_vector_linked())  {
                app->set_state_vector_linked(true);
                printf("Связь с оборудованием восстановлена!\n\n");
            }
	}
	sched_yield();
    }                                                                           
    delete []writingBuffer;
    return user;
}

commonFuncsMgr::commonFuncsMgr(SrvAppLayer *appl)
{
    appLayer=appl;
}

commonFuncsMgr::~commonFuncsMgr()
{
}

/******************************************************************************************//**
 * @brief      Declaration of common functions
 *
 * @details    Declares functions:
 * @details    - \ref emergencyShutdown, \ref controlModeChange, \ref getStateVector
 * @retval     err_result_ok    - declaration was successful
 *********************************************************************************************/
errType commonFuncsMgr::startCommonFuncs()
{
    errType result=err_result_ok;
    functionNode* func;
    
    // Function #32. EmergencyShtudown
    //----------------------------------
    // 1. Function definition
    //    Function #32 have 1 result argument and haven't parameters
    //    Set pointer to function implementaion
    func=new functionNode(32,0,1,emergencyShutdown);
    func->setFuncName("Аварийное завершение подсистемы");
    // 2. Definitions for parameters: not needed
    //func->setParamDescriptor();
    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE);
    func->setResultName(0,"Квитанция исполнения");

    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    // Function #33. ControlModeChange
    //---------------------------------
    // 1. Function definition
    //    Function #33 have 1 parameter and 1 result argument
    //    Set pointer to function implementaion
    func=new functionNode(33,1,1,controlModeChange);
    func->setFuncName("Смена оперативного режима службы");
    // 2. Definitions for parameters:
    func->setParamDescriptor(0,type_BYTE);
    func->setParamName(0,"Новый режим");

    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE); 
    func->setResultName(0,"Квитанция исполнения");
    
    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    // Function #34. GetStateVector
    //-------------------------------
    // 1. Function definition
    //    Function #34 have 0 parameters and 1 result argument
    //    Set pointer to function implementaion
    func=new functionNode(34,0,2,getStateVector);
    func->setFuncName("Запрос вектора состояния службы");

    // 2. Definitions for parameters: not needed
    //func->setParamDescriptor(0,1);
    //
    // 3. Defenitions for result arguments:
    //    Result argument have WORD type - 2 bytes
    func->setResultDescriptor(0, type_ERRTYPE); 
    func->setResultName(0,"Квитанция исполнения");

    func->setResultDescriptor(1, type_DWORD); 
    func->setResultName(1,"Вектор состояния службы");

    // 4. Add function to Application interchange layer:
    appLayer->CreateNewFunction(func);
    
    return result;
}

/******************************************************************************************//**
 * @brief      Undeclaration of common functions
 *
 * @details    Undeclares functions:
 * @details    - \ref emergencyShutdown, \ref controlModeChange, \ref getStateVector
 * @retval     err_result_ok    - undeclaration was successful
 *********************************************************************************************/
errType commonFuncsMgr::stopCommonFuncs()
{
    errType result=err_result_ok;
    // Function #32. EmergencyShutdown
    //----------------------------------
    result=appLayer->DeleteFunction(32);
    
    // Function #33. ControlModeChange
    //----------------------------------
    result=appLayer->DeleteFunction(33);
    
    // Function #34. GetStateVector
    //-------------------------------
    result=appLayer->DeleteFunction(34);
    
    return result;
}
