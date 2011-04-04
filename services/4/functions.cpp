// SASC Service
//---------------
#include <stdio.h>                                                                                                
#include <string.h>                                                                                               
#include <netinet/in.h>                                                                                           
#include <arpa/inet.h>                                                                                            
#include <deque>                                                                                                  

#include <deque>                                                                
#include <extra/ortsTypes/ortsTypes.h>                                          
#include <storage/ssBuffer/ssBuffer.h>                                                           
#include <storage/buffer/buffer.h>                                                             
#include <peripheral/udp_port/udp_port.h>                                             
#include <rcsLib/rcsCmd/rcsCmd.h>                                               
#include <param_desc.h>                                                         
#include <functionNode.h>                                                       
#include <SrvAppLayer.h>
#include <global.h> 

#include "SASC_packet/comm_SASC.h"

pthread_t PollingThreadHandle;

udp_port *equip_sending;                                                                           

comm_SASC sndSASCmsg;
comm_SASC rcvSASCmsg;

SASC_answer_mod typeinf;

BYTE frame_current_command[sizeof(SASC_msg_type)];
bool event_new_message (false);

buffer* resultStorage = 0;

extern errType getNextDBRecord();

    
errType equipListenProcessing(BYTE *writingBuffer, size_t *sz)
{
    size_t size (*sz);
    if (size > comm_SASC::kSASCMsgSize) size = comm_SASC::kSASCMsgSize;
    
    rcvSASCmsg.encode(writingBuffer, size);
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(size_t k = 0; k < size; k++)  printf("%.2X ", writingBuffer[k]);
    //equip_recvBuffer->unlockBufferChunkForExternWriting(size);
    printf("]\n\n");
    printf("\tРасшифровка:\n");
    if (rcvSASCmsg.checkAnswer(&typeinf) == err_result_ok)  {
	printf("\tПринята ответная квитанция: %s\n", SASC_answer_str[typeinf]);
	switch(typeinf)
	{
	    case _db_record:
                    //keep writingBuffer, (len=size);
		    //memcpy(resultStorage, writingBuffer, 
		    
		    if (resultStorage){
                        resultStorage->write(writingBuffer,size);
			getNextDBRecord();
		    } else
		    {
			//db record without request
		    }
		    break;
	
	    case _db_last_record:
		    //keep writingBuffer, (len=sz);
		    if (resultStorage){
                        resultStorage->write(writingBuffer,*sz);
			//db record without request
		    } else 
		    {
			//db record without request
		    }
		    break; 
	}
	
    } else {
	printf("\tМодификатор ответа не распознан. Обобщённое сообщение:\n");
	rcvSASCmsg.dbgPrint();
    }
    printf("\t===========================================\n\n");
}                                                                                                         


void* pollingThread(void* user)
{
    SrvAppLayer* app = (SrvAppLayer*) user;
//    WORD old_crc = 0xFFFF;
//    PassiveTimer timer;
//    timer.setInterval(kIntervalOfSendinToEquip);

    while (!app->terminated())
    {
        if (event_new_message)  {
            equip_sending->sendData(equipAddr, frame_current_command, comm_SASC::kSASCMsgSize);

            event_new_message = false;
        }
//        if (!timer.isActive() || (sendFrame->setCheckSumm() != old_crc))
//        {
//            sendFrame->dbgPrint();
//            old_crc = sendFrame->setCheckSumm();

//            BYTE array[cmdFrame::kPacketSize];
//            sendFrame->decode(array);
//            equip_sending->sendData(equipAddr, array, cmdFrame::kPacketSize);

//            if (timer.isActive()) timer.stop();
//            timer.start();
//        }
        app->srv_yield();
    }
//    return user;
}


errType srvInit()
{
    errType result = err_not_init;
    int ret = 0;

    printf("\tСлужба системы обеспечения  СКСЮ\n");
    printf("=============================================================\n\n");
    equip_sending = new udp_port(eq_udp_sending_port);
    result = equip_sending->open_port();
    equipAddr.s_addr = inet_addr(eq_ip_addr);
    
    ret = pthread_create(&PollingThreadHandle, NULL, pollingThread, app);
    //if (ret!=0) Error:  Need to stop application initializing process...
    if (ret == 0)
        result = err_result_ok;

    return result;
}

errType srvDeinit()
{
    equip_sending->close_port();
    delete equip_sending;
    return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
    errType result = err_not_init;
    
    functionNode* func = (functionNode*)fn;
    
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType controlModeChange(void* fn)
{
    errType result = err_not_init;

    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType getStateVector(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*)fn;
    func->printParams();
    func->setResult(1, &(app->getStateVector()));
    func->printResults();
    return result;

}

inline errType SendSASCMsg(SASC_cmd_mod mode, BYTE** params = 0)
{
    errType result (err_result_ok);

    sndSASCmsg.apply_mod(mode, params);
    sndSASCmsg.decode(frame_current_command);
    event_new_message = true;

    return result;
}

errType SASC_PowerON(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    SendSASCMsg(_power_on);

    func->printResults();
    return result;
}

errType SASC_PowerOFF(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    SendSASCMsg(_power_off);

    func->printResults();
    return result;
}

errType StartMeasuring(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    BYTE *params[7];
    //binning
    params[0] = (BYTE*)func->getParamPtr(0);
    //Niter
    params[1] = (BYTE*)func->getParamPtr(1);
    //FiH
    params[2] = (BYTE*)func->getParamPtr(2);
    //FiA
    params[3] = (BYTE*)func->getParamPtr(3);
    //TC
    params[4] = (BYTE*)func->getParamPtr(4);
    //Wm_c
    params[5] = (BYTE*)func->getParamPtr(5);
    //Wa
    params[6] = (BYTE*)func->getParamPtr(6);
    
    SendSASCMsg(_measure, params);

    func->printResults();
    
    return result;
}

errType ZeroMeasuring(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    BYTE *params[6];
    //binning
    params[0] = (BYTE*)func->getParamPtr(0);
    //FiH
    params[1] = (BYTE*)func->getParamPtr(1);
    //FiA
    params[2] = (BYTE*)func->getParamPtr(2);
    //TC
    params[3] = (BYTE*)func->getParamPtr(3);
    //Wm_c
    params[4] = (BYTE*)func->getParamPtr(4);
    //Wa
    params[5] = (BYTE*)func->getParamPtr(5);
    
    
    SendSASCMsg(_zero_measure, params);

    func->printResults();
    return result;
}

errType getNextDBRecord()
{
    errType result = err_result_ok;
    SendSASCMsg(_get_next_record);

    return result;
}

errType PrepareMeasuringResult(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    delete resultStorage;
    resultStorage = new buffer(5*comm_SASC::kSASCMsgSize);
    
    SendSASCMsg(_get_db);

    func->printResults();
    return result;
}

errType GetMeasuringResult(void* fn)
{
    errType result = err_result_ok;
    BYTE* buffer = 0;
    DWORD length = resultStorage->length();
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
//    #error Надо предположить - собраны ли все данные
    buffer = new BYTE[length + 2];
    *(WORD*)buffer = length / 4;
    length = resultStorage->read(buffer + 2);
    
    func->setResult(1, buffer);
    
    func->printResults();
    return result;
}

errType ZeroDB(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    SendSASCMsg(_erase_db);

    func->printResults();
    return result;
}

errType TaringStart(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();

    BYTE *params[5];

    //FiH
    params[0] = (BYTE*)func->getParamPtr(0);
    //FiA
    params[1] = (BYTE*)func->getParamPtr(1);
    //TC
    params[2] = (BYTE*)func->getParamPtr(2);
    //Wm_c
    params[3] = (BYTE*)func->getParamPtr(3);
    //Wa
    params[4] = (BYTE*)func->getParamPtr(4);
    
    SendSASCMsg(_tare_start, params);

    func->printResults();
    return result;
}

errType GetTaringPoint(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    BYTE *params[12];
    //binning
    params[0] = (BYTE*)func->getParamPtr(0);
    //Niter
    params[1] = (BYTE*)func->getParamPtr(1);
    //FiH
    params[2] = (BYTE*)func->getParamPtr(2);
    //FiA
    params[3] = (BYTE*)func->getParamPtr(3);
    //TC
    params[4] = (BYTE*)func->getParamPtr(4);
    //Wm_c
    params[5] = (BYTE*)func->getParamPtr(5);
    //Wa
    params[6] = (BYTE*)func->getParamPtr(6);

    //X
    params[7] = (BYTE*)func->getParamPtr(7);

    //Y
    params[8] = (BYTE*)func->getParamPtr(8);

    //Z
    params[9] = (BYTE*)func->getParamPtr(9);

    //Fix
    params[10] = (BYTE*)func->getParamPtr(10);

    //Fiy
    params[11] = (BYTE*)func->getParamPtr(11);

    SendSASCMsg(_get_tare_value, params);

    func->printResults();
    return result;
}

errType TaringStop(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    BYTE *params[5];
    
    //FiH
    params[0] = (BYTE*)func->getParamPtr(0);
    //FiA
    params[1] = (BYTE*)func->getParamPtr(1);
    //TC
    params[2] = (BYTE*)func->getParamPtr(2);
    //Wm_c
    params[3] = (BYTE*)func->getParamPtr(3);
    //Wa
    params[4] = (BYTE*)func->getParamPtr(4);
    
    
    SendSASCMsg(_tare_stop, params);

    func->printResults();
    
    return result;
}


errType linkTest(void* fn)
{
    errType result = err_result_ok;
    
    functionNode* func = (functionNode*)fn;
    
    func->printParams();
    
    SendSASCMsg(_link_test);

    func->printResults();
    
    return result;
}
