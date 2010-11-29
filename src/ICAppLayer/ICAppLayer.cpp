#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <iostream.h>
#include <queue>

#include "../../ortsTypes/ortsTypes.h"
#include "../../rcsLib/rcsCmd/rcsCmd.h"

#include "../global.h"
#include "../buffer/ssBuffer.h"
#include "../../udp_port/udp_port.h"
#include "../deqUdp/deqUdp.h"
#include "FunctionNode/param_desc.h"
#include "FunctionNode/FunctionNode.h"
#include "ICAppLayer.h"
#include "../Functions/CommonFuncs.h"

bool rcvComplete_flag=false;
bool sndAllow_flag=false;

ICAppLayer *app;

void* udpSenderThread (void* user)
{
	ICAppLayer *app=(ICAppLayer*)user;
	ssBuffer *sendBuffer=app->ICAppLayer_sendBuffer;

	deqUdp *uPort;
	size_t sz;
	

	bool raw_mode=false;
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
		    //if (app->ICAppLayer_sendBuffer->size()>0) 
		    uPort->sendData(app->ICAppLayer_sendBuffer);
		    printf("\tОбработка запроса завершена\n");
		    printf("-------------------------------------------------");
		    printf("\n\n\n\n\n\n\n\n\n\n");
		    sndAllow_flag=false;
		}
		
		sched_yield();
	    }
		
	//	*sz=1024;//sizeof(struct ip)+sizeof(struct udphdr);
	    printf("Завершение подсистемы сокетной посылки\n");
	    uPort->close_port();
	}
	
	    
    	    delete uPort;
}


void* udpListenerThread (void* user)
{
	ICAppLayer *app=(ICAppLayer*)user;
	
	WORD wUdp=app->getListenerPortNum();
	ssBuffer *recvBuffer=app->ICAppLayer_recvBuffer;
	deqUdp *uPort;
	//struct udphdr *udp;
	size_t sz[0];
	
	bool raw_mode=false;
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
		uPort->readData(app->ICAppLayer_recvBuffer,sz);
		// Put ip, udp to RecvBuffer
		
		    rcvComplete_flag=true;
		}
		
		*sz=1024;//sizeof(struct ip)+sizeof(struct udphdr);
		sched_yield();
	    }
	
	    printf("Завершение подсистемы сокетного приёма\n");
    	    uPort->close_port();
    	}
    delete uPort;

}

BYTE ICAppLayer::terminated()
{
    return AppTerminated;
}


ICAppLayer::ICAppLayer(WORD portNum)
{
    AppTerminated=false;
    cpListenerPortNum=portNum;
    
    func_quantity=0;
    ifCount=0;
}

ICAppLayer::~ICAppLayer()
{
    for (int i=0;i<func_quantity;i++) if (Functions) delete Functions[i];
    func_quantity=0;
    equip_listen->close_port();
    delete equip_listen;
}

errType ICAppLayer::CreateNewFunction(FunctionNode *func)
{
    errType result=err_not_init;
    Functions[func->id()]=func;
    func_quantity++;
    
    return result;
}

errType ICAppLayer::DeleteFunction(BYTE id)
{
    errType result=err_not_init;
    delete Functions[id];
    func_quantity--;
    
    return result;
}


errType ICAppLayer::encodeBlock(rcsCmd* ss_cmd, BYTE** data)
{
    errType result=err_not_init;
    
    *data=(BYTE*) ss_cmd;
    
    return result;
}

errType ICAppLayer::StartListening()
{
    errType result=err_not_init;
    int ret=0;
    //1. Prepare RecvBuffer
    this->ICAppLayer_recvBuffer=new ssBuffer;
    //2. Prepare SendBuffer
    this->ICAppLayer_sendBuffer=new ssBuffer;
    //3. Start udpListening thread
    if (!AppTerminated) ret=pthread_create(&listenerThread, NULL, udpListenerThread, (void*) this);
    //4. Start udpSending thread
    
    if ((!AppTerminated) && (ret==0))
    {   
	equip_listen=new udp_port(eq_udp_listen_port);
	ifCount=2;//equip_listen->scanIfaces();
	//printf("interfaces at start: %d\n", ifCount);
	result=equip_listen->open_port(true);
    }

    if ((!AppTerminated) && (ret==0)) ret=pthread_create(&senderThread, NULL, udpSenderThread, (void*) this);

    // Equipment listening
    if ((!AppTerminated) && (ret==0)) ret=pthread_create(&equipListenThread, NULL, EquipListenPolling, (void*) this);

    if ((result==err_result_ok) && (!AppTerminated)) result=err_result_ok;
    else result = err_sock_error;

    return result;

}

errType ICAppLayer::StopListening()
{
    errType result=err_not_init;
    if (verbose_level) printf("Останов подсистемы сокетного приёма\n");
    int ret=0;
    //1. Stop udpListening thread
    //pthread_join waits for normal terminating of thread;
    AppTerminated=true;
    
    //ret=pthread_join(listenerThread, NULL);
    //pthread_exit - ????
    //2. Stop udpSending thread
    //ret=pthread_join(senderThread, NULL);
    //pthread_exit - ????
    //3. Flush/freeing RecvBuffer
    delete ICAppLayer_recvBuffer;
    delete ICAppLayer_sendBuffer;
    //4. Flush/freeing SendBuffer
    return result;

}

errType ICAppLayer::equip_reading_event(){
	    errType result=err_not_init;
	    BYTE event=0;
	    result=equip_listen->udp_async_process(&event); 
	    if ((result==err_result_ok) && ((event&0x1)==0x1)) result=err_result_ok;
	    return result;
	}
	
errType ICAppLayer::equip_read_data(BYTE* buffer, size_t *sz){
	return equip_listen->readData(buffer, sz);
}

errType ICAppLayer::Decode_message(BYTE* dataBlock, DWORD length, rcsCmd *ss_cmd) {
	errType result=err_not_init;
	bool decoded;
	int fn_num;
	
	
	ss_cmd->encode(dataBlock);
	
	if (length!=ss_cmd->getCmdLength()) {
	    printf("Количество принятых байт разнится с количеством заявленных в заголовке!\n");
	    result=err_params_decode;
	}
	else {
	    decoded = ss_cmd->checkSign();
	    printf("Принято сообщение:");
	    ss_cmd->dbgPrint();
	
	
	    if (decoded)
	    {
		fn_num=ss_cmd->get_func_id();
		if ((Functions[fn_num])->id()!=ss_cmd->get_func_id()) 
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

errType ICAppLayer::execMessage(rcsCmd* ss_cmd)
{
    errType result=err_not_init;
    BYTE *tmp;
    DWORD len;
    int fn_num=ss_cmd->get_func_id();
    
    result=(Functions[fn_num])->decodeParams(ss_cmd);
    if (result==err_result_ok) {
	result=(Functions[fn_num])->callFunction();
    } else {
	printf("Доставлен пакет с ошибками в формате описания параметров!\n");
	result=err_params_decode;
    }
	ServiceState.execState.fields.func_exec_id=fn_num;
	(Functions[fn_num])->setResult(0,(BYTE*)&result);
	
    return result;
}

errType ICAppLayer::prepare_FuncResult(rcsCmd* in_cmd, rcsCmd* out_cmd)
{
	BYTE *ret;
	BYTE *resData;
	int size;
	errType result=err_result_ok;

	OrtsType  *err_val;
	DWORD err_len;
	int fn_num=in_cmd->get_func_id();
	(Functions[fn_num])->getResult(0, (void**)&err_val, &err_len);
	
	ServiceState.execState.fields.func_exec_result=*(BYTE*)err_val;
	
	
	int func_resultsQuantity=(Functions[fn_num])->getResultsQuantity();
	size=(Functions[fn_num])->getAllResultsLength();
	
	resData=new BYTE[size];
	
	int offset=0; //BYTE func_id + DWORD paramsLength = 5
	DWORD ret_len=0;
	
	for (int i=0; i<func_resultsQuantity; i++) {
	    (Functions[fn_num])->getResult(i, (void**)&ret, &ret_len);
	    memcpy(resData+offset, ret, ret_len);
	    offset+=ret_len;
	}
	out_cmd->encode(fn_num, offset, resData);
	out_cmd->makeSign();
	
	return result;
	delete resData;
}

errType ICAppLayer::sendResult(sockaddr_in *sfrom, rcsCmd* ss_cmd)
{
	errType result=err_not_init;
	BYTE* dataBlock;
	dataBlock=new BYTE[ss_cmd->getCmdLength()];
	ss_cmd->decode(dataBlock);
	
	sfrom->sin_port=htons(wUdp+1);
	
	//printf("answer: [");
	//for (int i=0; i<ss_cmd->getCmdLength(); i++) printf("%.2X ",dataBlock[i]);
	//printf("]\n");
	
	ICAppLayer_sendBuffer->pushBlock(sfrom, dataBlock, ss_cmd->getCmdLength());
	delete dataBlock;
	
	return result;
}

errType ICAppLayer::ProcessMessages()
{
    errType result=err_not_init;
    FunctionNode* func;
    BYTE* dataBlock;
    DWORD length;
    rcsCmd *in_cmd, *out_cmd;
    sockaddr_in sfrom;
    
    if (AppTerminated==true) {
	result=err_result_error;
	printf("Запуск отменён!\n");
	return result;
    }
    if (ICAppLayer_recvBuffer->size()==0) return err_not_init;
    if (rcvComplete_flag==false) return err_not_init;
    
    if (equip_listen->scanIfaces()<ifCount) 
    {
	ServiceState.servState.fields.overflow_link_sec=1; 
    } else ServiceState.servState.fields.overflow_link_sec=0;

//  1) Read RecvBuffer --------------------------------------
    
    int len=ICAppLayer_recvBuffer->getFrontBlockSize();
    
    in_cmd=new rcsCmd();
    out_cmd=new rcsCmd();

    dataBlock=new BYTE[len];
    
    ICAppLayer_recvBuffer->dbgPrint();
    length=ICAppLayer_recvBuffer->popBlock(&sfrom, dataBlock);
    
    
//  2) Decode message ---------------------------------------
    result=Decode_message(dataBlock, length, in_cmd);

    //in_cmd->dbgPrint();
//  3) exec function ----------------------------
    if (result==err_result_ok) result=execMessage(in_cmd);
    
//  4) Encode function result
    //if (result==err_result_ok) 
    
    
    if (result!=err_result_ok) result=(Functions[in_cmd->get_func_id()])->setResult(0,&result);
    
    result=prepare_FuncResult(in_cmd, out_cmd);
    
// 5)  Write SendBuffer
    if (result==err_result_ok) sendResult(&sfrom, out_cmd);
    
// 6) Freeing heap: 
                               
	delete dataBlock;
	delete in_cmd;
	delete out_cmd;

// 7)  Sync with r/w threads

	rcvComplete_flag=false;
    if (result==err_result_ok) {
	sndAllow_flag=true;
    }
    
    return result;

}

WORD ICAppLayer::getListenerPortNum()
{
    return cpListenerPortNum;
}

void ICAppLayer::terminate(BYTE mode)
{
    AppTerminated=mode;
}

type_StateVector ICAppLayer::getStateVector()
{
    return ServiceState;
}
