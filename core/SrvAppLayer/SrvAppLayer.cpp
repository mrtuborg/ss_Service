/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class srvAppLayer implementation.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <iostream>
#include <queue>

#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>

#include <global.h>
#include <ssBuffer.h>
#include <comm/udp_port/udp_port.h>
#include <deqUdp.h>
#include <rcsLib/udpAction/udpAction.h>
#include <param_desc.h>
#include <functionNode.h>

#include <SrvAppLayer.h>
#include <commonFuncsMgr.h>

/// todo msc diagramm
bool rcvComplete_flag=false; ///< Flag purpose: synchronize state between receiving clients requests thread and reading for received data
/// todo msc diagramm
bool sndAllow_flag=false;    ///< Flag purpose: synchronize state between sending clients answers thread and preparing sending data

SrvAppLayer *app;

/**********************************************************************************//**
 * @brief       Thread to sending data to clients from functions answer queue.
 *
 * @details    Thread also includes work with port opening, initializing and closing.
 *************************************************************************************/
void* udpSenderThread (void* user)
{
    SrvAppLayer *app=(SrvAppLayer*)user;

    deqUdp *uPort;

    uPort=new deqUdp(wUdp+1);//,true);
    if ((app->terminated()) || (uPort->open_port()!=err_result_ok)) { // open for sending
        printf("Ошибка открытия сокета. Подсистема сокетной посылки не запущена\n");
        app->terminate();
        delete uPort;
    } else {
        printf("Подсистема сокетной посылки запущена\n");
        //sendBuffer=new ssBuffer();
        //*sz=1024;
        sndAllow_flag=false;
        //in_addr remote_ip;
        while (!app->terminated())
        {

            if (sndAllow_flag==true)
            {
                //if (app->functionsAnswersQueue->size()>0)
                uPort->sendData(app->functionsAnswersQueue);
                printf("\tОбработка запроса завершена\n");
                printf("-------------------------------------------------");
                printf("\n\n\n\n\n\n\n\n\n\n");
                sndAllow_flag=false;
                rcvComplete_flag=false;
            }

            sched_yield();
        }

	//	*sz=1024;//sizeof(struct ip)+sizeof(struct udphdr);
        printf("Завершение подсистемы сокетной посылки\n");
        uPort->close_port();
    }


    delete uPort;
    return user;
}

/****************************************************************************************//**
 * @brief       Thread to listening requests from clients and form queue of clients requests.
 *
 * @details    Thread also includes work with port opening, initializing and closing.
 **************************************************************************************/
void* _ss_udpListenerThread (void* user)
{
    SrvAppLayer *app=(SrvAppLayer*)user;

    WORD wUdp=app->getListenerPortNum();
    deqUdp *uPort;
    //struct udphdr *udp;
    size_t sz[0];

    uPort=new deqUdp(wUdp);//,true);
    if ((app->terminated()) || (uPort->open_port(true)!=err_result_ok)) {
        printf("Ошибка открытия сокета. Подсистема сокетного приёма не запущена!\n");
        app->terminate();
    } else {
        printf("Подсистема сокетного приёма запущена\n");
        *sz=1024;
        rcvComplete_flag=false;
        while (!app->terminated()){
            if (rcvComplete_flag==false) {
		//Get DataPtr from RecvBuffer
		*sz=1024;//sizeof(struct ip)+sizeof(struct udphdr);
		uPort->readData(app->clientsRequestsQueue,sz);
		// Put ip, udp to RecvBuffer

                rcvComplete_flag=true;
            }


            sched_yield();
        }

        printf("Завершение подсистемы сокетного приёма\n");
        uPort->close_port();
    }
    delete uPort;
    return user;
}

/**********************************************************************************//**
 * @brief      Method to check application internal termination signal.
 *
 * @retval     0 - Application runs normally
 * @retval     1 - Application terminating and preparing to exit to the OS
 * @retval     2 - Application terminating and preparing to reboot the OS
 **************************************************************************************/
BYTE SrvAppLayer::terminated()
{
    return AppTerminated;
}

/**************************************************************************************************************//**
 * @brief      Constructs instance of srvAppLayer and links it to udp port number for listening clients requests
 *
 * @param[in]   portNum - udp port number that will use for clients requests listening
 ******************************************************************************************************************/

SrvAppLayer::SrvAppLayer(WORD portNum)
{
    AppTerminated=false;
    cpListenerPortNum=portNum;
    
    func_quantity=0;
    ifCount=0;

    setServiceMode(0); // automatic mode startup
}

SrvAppLayer::~SrvAppLayer()
{
    for (int i=0;i<func_quantity;i++) if (Functions) delete Functions[i];
    func_quantity=0;
    equip_listen->close_port();
    delete equip_listen;
}

/**********************************************************************************//**
 * @brief      Method to add new declared function to service layer.
 *
 * @param[in]   func - functionNode instance
 * @retval      err_result_ok - function added successfully
 **************************************************************************************/
errType SrvAppLayer::CreateNewFunction(functionNode *func)
{
    errType result=err_result_ok;
    Functions[func->id()]=func;
    func_quantity++;
    
    return result;
}

/**********************************************************************************//**
 * @brief      Method to delete function from service layer.
 *
 * @param[in]   id - functionNode identifier
 * @retval      err_result_ok - function deleted successfully
 **************************************************************************************/
errType SrvAppLayer::DeleteFunction(BYTE id)
{
    errType result=err_result_ok;
    delete Functions[id];
    func_quantity--;
    
    return result;
}

/**********************************************************************************//**
 * @brief      Method to encode data from rcsCmd message.
 *
 * @param[in]   ss_cmd - rcsCmd message
 * @param[out]  data   - pointer to pointer that will include result of method
 * @retval      err_result_ok - Block encoded successfully
 * @todo nobody uses this method. need to be deleted.
 **************************************************************************************/
//errType srvAppLayer::encodeBlock(rcsCmd* ss_cmd, BYTE** data)
//{
//    errType result=err_result_ok;
//
//    *data=(BYTE*) ss_cmd;
//
//    return result;
//}

/**********************************************************************************//**
 * @brief      Method to prepare and start base communication engine
 * @details     if errors has been occured,method initiate appTerminate signal.
 * @retval      err_result_ok   - communication engine starts successfully
 * @retval      err_sock_error  - udp sockets prerparing error
 **************************************************************************************/
errType SrvAppLayer::StartListening()
{
    errType result=err_not_init;
    int ret=0;
    /// @brief 1. Prepare \ref clientsRequestsQueue.
    /// @details Queue stores all received messages from clients and clients adresses.
    this->clientsRequestsQueue=new ssBuffer;

    /// @brief 2. Prepare \ref functionsAnswersQueue.
    /// @details Queue stores all functions answers to clients requests.
    this->functionsAnswersQueue=new ssBuffer;

    /// @brief 3. Start \ref udpListenerThread
    /// @details Thread listen for udp messages from clients and stores messages in \ref clientsRequestsQueue.
    if (!AppTerminated) ret=pthread_create(&listenerThread, NULL, _ss_udpListenerThread, (void*) this);

    /// @brief 4. Prepare \ref equip_listen.
    /// @details This is an udp port instance that using to listen data from equipment
    if ((!AppTerminated) && (ret==0))
    {   
	equip_listen=new udp_port(eq_udp_listen_port);
	ifCount=2;//equip_listen->scanIfaces();
	//printf("interfaces at start: %d\n", ifCount);
	result=equip_listen->open_port(true);
    }
    /// @brief 5. Start \ref udpSenderThread
    /// @details Thread sending udp messageds to clients from \ref functionsAnswersQueue
    if ((!AppTerminated) && (ret==0)) ret=pthread_create(&senderThread, NULL, udpSenderThread, (void*) this);

    /// @brief 6. Start \ref equipListenPolling
    /// @details Thread listen for udp messages from equipment
    if ((!AppTerminated) && (ret==0)) ret=pthread_create(&equipListenThread, NULL, equipListenPolling, (void*) this);

    if ((result==err_result_ok) && (!AppTerminated)) result=err_result_ok;
    else result = err_sock_error;

    return result;

}

/**********************************************************************************//**
 * @brief      Method to stop base communication engine
 * @details    method initiate appTerminate signal, deletes queues.
 * @retval      err_result_ok   - engine stopped successfully
 **************************************************************************************/
errType SrvAppLayer::StopListening()
{
    errType result=err_result_ok;
    if (verbose_level) printf("Останов подсистемы сокетного приёма\n");
    //1. Stop udpListening thread
    //pthread_join waits for normal terminating of thread;
    AppTerminated=true;
    
    //ret=pthread_join(listenerThread, NULL);
    //pthread_exit - ????
    //2. Stop udpSending thread
    //ret=pthread_join(senderThread, NULL);
    //pthread_exit - ????
    //3. Flush/freeing RecvBuffer
    delete clientsRequestsQueue;
    delete functionsAnswersQueue;
    //4. Flush/freeing SendBuffer
    return result;

}

/**********************************************************************************//**
 * @brief      Method to asynchonous polling of \ref equip_listen socket
 * @todo        too strange method. May be it need be refactoring.
 * @retval      err_result_ok   - udp socket received data
 **************************************************************************************/
errType SrvAppLayer::equip_reading_event(){
	    errType result=err_not_init;
	    BYTE event=0;
	    result=equip_listen->udp_async_process(&event);
	    if ((result==err_result_ok) && ((event&0x1)==0x1)) result=err_result_ok;
	    return result;
	}

/**********************************************************************************//**
 * @brief       Method to read data from \ref equip_listen socket
 * @todo        too strange method. May be it need be refactoring.
 * @param[out]  buffer - uses to store recevied data
 * @param[out]  sz - size in bytes of received data
 * @retval      err_result_ok   - udp socket received data has been readed
 **************************************************************************************/
errType SrvAppLayer::equip_read_data(BYTE* buffer, size_t *sz){
    return equip_listen->readData(buffer, sz);
}

/**********************************************************************************//**
 * @brief       Method to decode message data to function call and parameters set.
 * @details     Method using method \ref rcsCmd::encode to transform data array to \ref rcsCmd message.
 * @details     Method checks for correct sign, calling function existing and compares real
 *              received data length with header information about data length.
 * @param[in]   dataBlock - pointer to received bytes array
 * @param[in]   length - size of received bytes array.
 * @param[out]  ss_cmd - decoded message.
 * @retval      err_result_ok   - decoded message is correct.
 * @retval      err_params_decode - params in decoded message is incorrect.
 * @retval      err_not_found - decoded message is calling for non existing function
 * @retval      err_crc_error - decoded message signature is incorrect.
 **************************************************************************************/
errType SrvAppLayer::decodeMessage(BYTE* dataBlock, DWORD length, rcsCmd *ss_cmd) {
    errType result=err_not_init;
    bool decoded;
    int fn_num;


    ss_cmd->encode(dataBlock);

    if (length!=ss_cmd->getCmdLength()) {
        printf("Нарушен формат заголовка пакета: количество принятых байт разнится с количеством заявленных в заголовке!\n");
        result=err_params_decode;
    }
    else {
        decoded = ss_cmd->checkSign();
        printf("Принято сообщение:");
        ss_cmd->dbgPrint();
	
	
        if (decoded)
        {
            fn_num=ss_cmd->get_func_id();
            if ((!Functions[fn_num]) || ((Functions[fn_num])->id()!=ss_cmd->get_func_id()))
            {
		// Function doesn't exist!!!
                printf ("ОШИБКА: Запрос на обслуживание нереализованной функции\n");
                result=err_not_found;
            }
            else {
                result=err_result_ok;
            }
        } else {
            printf("ОШИБКА: Несоответствие контрольной суммы принятого пакета\n");
            result=err_crc_error;
        }
    }

    return result;
}

/**********************************************************************************//**
 * @brief       Method to execute function by id in rcsCmd message.
 * @details     Method checks for correct params by comparing params with description in
 *              commonFuncsMgr or specFuncsMgr.
 * @details     After requested function calling method changes ServiceState vector and
 *              set answer ticket for return value from requested function.
 * @todo        ServiceState vector need to change before function calling.
 * @param[in]   ss_cmd - message with request for function calling.
 * @retval      err_result_ok   - message executed successfully.
 * @retval      err_params_decode - params in message differs with description.
 **************************************************************************************/
errType SrvAppLayer::execMessage(rcsCmd* ss_cmd)
{
    errType result=err_not_init;
    int fn_num=ss_cmd->get_func_id();
    
    result=(Functions[fn_num])->decodeParams(ss_cmd);
    if (result==err_result_ok) {
	result=(Functions[fn_num])->callFunction();
    } else {
	printf("Доставлен пакет с ошибками в формате описания параметров!\n");
	result=err_params_decode;

    }
    ServiceState.lastFuncId=fn_num;
    ServiceState.lastResult=result;


    (Functions[fn_num])->setResult(0,(BYTE*)&result);

    return result;
}

/**********************************************************************************//**
 * @brief       Method encoding all executed function results to rcsCmd message.
 * @details     Method preparing rcsCmd message with making of message signing.
 * @todo        Need a refactoring.
 * @param[in]   in_cmd - message with request for function calling.
 * @param[out]  out_cmd - message with results from requested function.
 * @retval      err_result_ok   - message executed successfully.
 * @retval      err_not_found   - function to execute was not found.
 **************************************************************************************/
errType SrvAppLayer::encodeFuncResult(rcsCmd* in_cmd, rcsCmd* out_cmd)
{
    BYTE *ret;
    BYTE *resData;
    int offset=0; //BYTE func_id + DWORD paramsLength = 5

    int size;
    errType result=err_result_ok;

    OrtsType  *err_val;
    DWORD err_len;
    int fn_num=in_cmd->get_func_id();
    if (Functions[fn_num]) {
        (Functions[fn_num])->getResult(0, (void**)&err_val, &err_len);
	
	
        int func_resultsQuantity=(Functions[fn_num])->getResultsQuantity();
        size=(Functions[fn_num])->getAllResultsLength();
	
        resData=new BYTE[size];
	
        DWORD ret_len=0;
	
        // printf("Q=%d\n", func_resultsQuantity);
        for (int i=0; i<func_resultsQuantity; i++) {
            //     printf("%d: \n",i);
            (Functions[fn_num])->getResult(i, (void**)&ret, &ret_len);
            memcpy(resData+offset, ret, ret_len);
            offset+=ret_len;
        }
    } else {
        resData=new BYTE[1];
        *resData=(int)err_not_found;
        offset=1;
	
	}
	out_cmd->encode(fn_num, offset, resData);
	out_cmd->makeSign();
	
	return result;
	delete []resData;
}

/**********************************************************************************//**
 * @brief       Method sending rcsCmd message to needed recepient.
 * @details     Method preparing data array from rcsCmd message and push data array
 *              with address of recepient in to functionsAnswersQueue
 * @param[in]   sfrom - recepient address.
 * @param[in]   ss_cmd - message needed to send.
 * @retval      err_result_ok   - message added to sending queue successfully.
 **************************************************************************************/
errType SrvAppLayer::sendResult(sockaddr_in *sfrom, rcsCmd* ss_cmd)
{
	errType result=err_result_ok;
	BYTE* dataBlock;
	dataBlock=new BYTE[ss_cmd->getCmdLength()];
	ss_cmd->decode(dataBlock);
	
	sfrom->sin_port=htons(wUdp+1);
	
	//printf("answer: [");
	//for (int i=0; i<ss_cmd->getCmdLength(); i++) printf("%.2X ",dataBlock[i]);
	//printf("]\n");
	
	functionsAnswersQueue->pushBlock(sfrom, dataBlock, ss_cmd->getCmdLength());
	delete []dataBlock;
	
	return result;
}



/**********************************************************************************//**
 * @brief       Method to make one step of srvAppLayer step.
 * @details     Method checks at start \ref AppTerminated signal, size of \ref clientsRequestsQueue
 *              and \ref rcvComplete_flag before it continuous execution.
 * @retval      err_result_ok   - one step of engine execution was successfully.
 * @retval      err_not_init    - method break execution.
 **************************************************************************************/
errType SrvAppLayer::processMessages(){
		errType result=err_not_init;

		if (!rcvComplete_flag) return result;

		if (AppTerminated==true) {
			result=err_result_error;
			printf("Запуск отменён!\n");
			return result;
		}

		rcsCmd *inCmd;
		sockaddr_in sfrom;

		if (clientsRequestsQueue->size()) {
			inCmd=new rcsCmd();

			result=processInMessages(&sfrom, inCmd);
			//if (result!==err_result_ok) {
					result=processOutMessages(&sfrom, inCmd, result);

			//}
			delete inCmd;

		}


	/// 7) Release allocated memory



	/// 8)  Sync listening and sending threads by \ref rcvComplete_flag and \ref sndAllow_flag


	    if (result==err_result_ok) {

	        sndAllow_flag=true;

	    }
	    return result;
}

errType SrvAppLayer::processOutMessages(sockaddr_in *sfrom, rcsCmd *inCmd, errType outResult)
{
			errType result=outResult;
			rcsCmd *out_cmd;
			out_cmd=new rcsCmd();

	/// 4) Encoding function result ticket if execution was not successfully
		    //if ((result!=err_result_ok) && (result!=err_not_found))
		    {

		    		result=(Functions[inCmd->get_func_id()])->setResult(0,&result);
		    }

	/// 5) Encode remains function results be \ref encodeFuncResult

		    result=encodeFuncResult(inCmd, out_cmd);

	/// 6)  Write results to sending queue by \ref sendResult

		    if (result==err_result_ok) sendResult(sfrom, out_cmd);
		    delete out_cmd;

		    return result;
}
errType SrvAppLayer::processInMessages(sockaddr_in *sfrom, rcsCmd *inCmd)
{
    errType result=err_not_init;
	BYTE* dataBlock;
    DWORD length;
    


//   TODO: link bit in serviceState
//    if (equip_listen->scanIfaces()<ifCount)
//    {
//	ServiceState.servState.fields.overflow_link_sec=1;
//    } else ServiceState.servState.fields.overflow_link_sec=0;

///  1) Read from \ref clientsRequestsQueue one new request
    
    int len=clientsRequestsQueue->getFrontBlockSize();
    dataBlock=new BYTE[len];
    
    //clientsRequestsQueue->dbgPrint();
    length=clientsRequestsQueue->popBlock(sfrom, dataBlock);
    
    
/// 2) Decode readed request by \ref decodeMessage
    result=decodeMessage(dataBlock, length, inCmd);

    //in_cmd->dbgPrint();
/// 3) Execute requested function if decoding was successfully by \ref execMessage

    BYTE func_id=inCmd->get_func_id();
    bool isRemoteCaller=false;

    if (sfrom->sin_addr.s_addr!=inet_addr("127.0.0.1")) isRemoteCaller=true;

    if ((Functions[func_id])->isMutator()) {
        switch (serviceMode()){
        case 0:// 0 - in automatic mode (locals calls mode)
            if (!isRemoteCaller) result=err_result_ok;
            else result=err_not_allowed;
            break;

        case 1:// 1- in manual mode	(remote calls mode)
            if (isRemoteCaller) result=err_result_ok;
            else result=err_not_allowed;
            break;
        }
    }

    if (result==err_result_ok){
    		result=execMessage(inCmd);

    }

    delete []dataBlock;
    
    return result;

}


/// getter for udp port number that listens all clients requests
WORD SrvAppLayer::getListenerPortNum()
{
    return cpListenerPortNum;
}

/// getter for AppTerminated signal
void SrvAppLayer::terminate(BYTE mode)
{
    AppTerminated=mode;
}

///getter for ServiceState vector
stateVector_type SrvAppLayer::getStateVector()
{
    return ServiceState;
}
// 0 - automatic, 1- manual
BYTE SrvAppLayer::serviceMode()
{
	if (ServiceState.state.mode_auto == 1) return 0;
	if (ServiceState.state.mode_manual == 1) return 1;

}

errType SrvAppLayer::setServiceMode(BYTE mode)
{
	ServiceState.state.mode_manual=0;
	ServiceState.state.mode_auto=0;

	if (mode==0) ServiceState.state.mode_auto = 1;
	else ServiceState.state.mode_manual = 1;

}
