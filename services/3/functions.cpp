// Covering Control Service
//--------------------------
#include <stdio.h> 
#include <unistd.h>
#include <string.h> 
#include <arpa/inet.h> 
#include <time.h> 
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

#include <shield_types.h>
#include <cmdFrame.h>
#include <statusFrame.h>
#include <cm688_packet.h>
#include <peripheral/timers/passive_timer.h>

extern bool array_contain_frame(BYTE* array, size_t size, BYTE* shifted_bytes);

udp_port  *equip_sending;
pthread_t PollingThreadHandle;

cmdFrame    *sendFrame;
statusFrame *answerFrame;

typedef union SpecificServiceState
{
    struct
    {
        BYTE open     :1;
        BYTE close    :1;
        BYTE stopped  :1;
        BYTE reserved :5;
    } fields;
    BYTE bValue;
}__attribute__ ((packed)) SpecificServiceState;

// 00|0 not initialized
// 00|1 error
// 11|0 Stopped
// 11|1 Stopping
//------------
// 01|0 Opened
// 01|1 Opening
// 10|0 Closed
// 10|1 Closing
SpecificServiceState spec_service_state;

void* pollingThread(void* user)
{
    SrvAppLayer* app = (SrvAppLayer*) user;
    WORD old_crc = 0xFFFF;
    PassiveTimer timer;
    timer.setInterval(kIntervalOfSendinToEquip);

    while (!app->terminated())
    {
        if (!timer.isActive() || (sendFrame->setCheckSumm() != old_crc))
        {
            if (verbose_level) sendFrame->dbgPrint();
            old_crc = sendFrame->setCheckSumm();

            BYTE array[cmdFrame::kPacketSize];
            sendFrame->decode(array);
            equip_sending->sendData(equipAddr, array, cmdFrame::kPacketSize);

            if (timer.isActive()) timer.stop();
            timer.start();
        }
        app->srv_yield();
    }
    return user;
}


//This method process byte arrays, has received from srvAppLayer.
//It's searches packet in array and process it if packet is there and it's whole;
// otherwise it's deletes excess bytes, changes length of array
// and return err_frame_incomplete
errType equipListenProcessing(BYTE *writingBuffer, size_t *sz)
{
    errType result = err_result_ok;
    size_t size (*sz);
    //if (size>sizeof(SASC_msg_type)) size=sizeof(SASC_msg_type);

    //checks array for containing packet
    BYTE count_shifted_bytes;
    bool has_packet;
    has_packet = array_contain_frame(writingBuffer, size, &count_shifted_bytes);

    //modifying array for bring it into accord with packet's format
    if (!has_packet)  {
        printf("\nС иерархии нижнего уровня получен некорректный пакет пакет (hex): ");
        for (size_t k = 0; k < size; k++)
            printf("%.2X ", writingBuffer[k]);
        printf("]\n");
        if (count_shifted_bytes)  {
            printf("Его начало будет смещено на %d байт\n", count_shifted_bytes);

            *sz -= count_shifted_bytes;
            memcpy(writingBuffer, writingBuffer + count_shifted_bytes, *sz);
            memset(writingBuffer + *sz, 0, count_shifted_bytes);
            if (*sz > statusFrame::kPacketSize)
                *sz = statusFrame::kPacketSize;       //deleting shifted bytes from the end
        }
        else *sz = 0;
        printf("\n");
    }

    if (*sz == statusFrame::kPacketSize) has_packet = true;
    else                                     result = err_frame_incomplete;

    //processing of correct packet
    if (has_packet)  {
        statusFrame temporary;
        temporary.encode(writingBuffer, *sz);
        if (temporary.testCheckSumm())  {
            answerFrame->encode(writingBuffer, *sz);
            printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
            printf("\t[");
            for (size_t k = 0; k < *sz; k++)
                printf("%.2X ", writingBuffer[k]);
            printf("]\n\n");
            if (verbose_level)  {
                printf("\tРасшифровка:\n");
                answerFrame->dbgPrint();
                printf("\t===========================================\n\n");
            }
        }
        else
            printf("CRC check fails! Packet ignored!\n\n");
    }

    return result;
}

//checks packet on contaning a whole statusFrame packet
//shifted_bytes - count of bytes from beginning of, those are not accords
// with format of statusFrame packet
//return value:
// true - if array is a ready packet
// false - in otherwise cases
bool array_contain_frame(BYTE* array, size_t size, BYTE* shifted_bytes)
{
    bool result (false);
    *shifted_bytes = 0;

    for (size_t i = 0; i < (size - 1); i++)  {
        if (*((WORD*)(array + i)) != 0xAA55) *shifted_bytes = i+1;
        else break;
    }
    if (!*shifted_bytes)  {
        if (size == statusFrame::kPacketSize) result = true;
    }

    return result;
}

errType srvInit()
{
    errType result = err_not_init;
    int ret = 0;

    sendFrame = new cmdFrame;
    answerFrame = new statusFrame;

    printf("\tСлужба системы контроля САУ укрытия\n");
    printf("=============================================================\n\n");
    equip_sending = new udp_port(eq_udp_sending_port);
    result = equip_sending->open_port();
    equipAddr.s_addr = inet_addr(eq_ip_addr);

    ret = pthread_create(&PollingThreadHandle, NULL, pollingThread, app);
    //if (ret!=0) Error:  Need to stop application initializing process...
    if (ret == 0)
        result = err_result_ok;

    spec_service_state.bValue = 0;

    return result;
}

errType srvDeinit()
{
    equip_sending->close_port();
    delete equip_sending;

    delete sendFrame;
    delete answerFrame;

    return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
    errType result = err_not_init;
    functionNode* func = (functionNode*) fn;
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType controlModeChange(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;
    func->printParams();
    BYTE mode=*(BYTE*)func->getParamPtr(0);
    if (mode<2) app->setServiceMode(mode);
    else result=err_params_value;
    func->printResults();
    return result;
}

errType getStateVector(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;
    stateVector_type stateVector;
    stateVector = app->getStateVector();

    spec_service_state.fields.open  = answerFrame->getShieldState(0);
    spec_service_state.fields.close = answerFrame->getShieldState(1);
    spec_service_state.fields.close = answerFrame->getShieldState(2);

    stateVector.reserved = spec_service_state.bValue;

    BYTE emergency (0);

    emergency =  !answerFrame->getSystemLinkStatus(KEGP);       //0001
    emergency |= !answerFrame->getSystemLinkStatus(PMU)  << 1;  //0010
    emergency |= !answerFrame->getSystemLinkStatus(AUGS) << 2;  //0100
    emergency |= !answerFrame->getSystemLinkStatus(BUZ)  << 3;  //1000
    stateVector.state.reserved = emergency;

    func->printParams();
    func->setResult(1, &stateVector);
    func->printResults();
    return result;
}

errType shieldPowerON(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();
    func->printResults();

    return result;
}

errType shieldPowerOFF(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();
    func->printResults();

    return result;
}

errType hydroSystemPowerON(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setHydroStationStart();

    func->printResults();

    return result;
}

errType hydroSystemPowerOFF(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setHydroStationStop();

    func->printResults();

    return result;
}

errType hydroSystemGetParams(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE pressure = answerFrame->getHydroSystemValues(0);
    BYTE oilLevel = answerFrame->getHydroSystemValues(1);
    BYTE temp     = answerFrame->getHydroSystemValues(2);
    BYTE filters  = answerFrame->getHydroSystemValues(3);
    BYTE status   = answerFrame->getHydroSystemStatus();

    // 1 - pressure, 2 - oil_level, 3 - temp, 4 - filters
    func->setResult(1, &pressure);
    func->setResult(2, &oilLevel);
    func->setResult(3, &temp);
    func->setResult(4, &filters);
    func->setResult(5, &status);

    func->printResults();

    return result;
}

errType foldOpen(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE foldId = *(BYTE*) func->getParamPtr(0);
    sendFrame->setFoldOpen((FoldDscr_type) foldId);
    func->setResult(1, &foldId);
    func->printResults();

    return result;
}

errType foldClose(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE foldId = *(BYTE*) func->getParamPtr(0);

    sendFrame->setFoldClose((FoldDscr_type) foldId);
    func->setResult(1, &foldId);
    func->printResults();

    return result;
}

errType foldStop(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE foldId = *(BYTE*) func->getParamPtr(0);

    sendFrame->setFoldStop((FoldDscr_type) foldId);

    func->setResult(1, &foldId);

    func->printResults();

    return result;
}

errType shieldOpen(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setSchieldOpen();

    func->printResults();

    return result;
}

errType shieldClose(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setSchieldClose();

    func->printResults();

    return result;
}

errType shieldStop(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setSchieldStop();

    func->printResults();

    return result;
}

errType foldGetParams(void* fn)
{
    BYTE foldState;
    WORD foldPos;

    errType result = err_not_init;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE fold_id = *(BYTE*)func->getParamPtr(0);


    func->setResult(1, &fold_id);

    if (fold_id < 3)  {
        foldState = answerFrame->getFoldState((FoldDscr_type) fold_id);
        foldPos = answerFrame->getFoldPosition((FoldDscr_type) fold_id);
        result = err_result_ok;
        func->setResult(2, &foldState);
        func->setResult(3, &foldPos);
    }

    func->printResults();

    return result;
}

errType changeControlMode(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE mode = *(BYTE*) func->getParamPtr(0);

    switch (mode)
    {
    case 0:
        sendFrame->setCPControl();
        break; //Control panel control
    case 1:
        sendFrame->setRemoteControl();
        break; //Remote panel control
    case 2:
        sendFrame->setLocalControl();
        break; //Local panel control
    }

    func->printResults();

    return result;
}

errType getControlMode(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;
    func->printParams();
    BYTE mode (2);
    if (answerFrame->isLocalControl()) mode = 2;
    if (answerFrame->isRemoteControl())  {
        mode = 1;
        if (mode == 2) mode = 3;
    }
    if (answerFrame->isCPControl()) mode = 0;

    func->setResult(1, &mode);
    func->printResults();
    return result;
}

errType startFuncControl(void* fn)
{

    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setStartFuncControl();

    func->printResults();

    return result;
}

errType correctHydroCyl(void* fn)
{

    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    sendFrame->setHydroCyl_correct();

    func->printResults();

    return result;

}

typedef union uzState_type{
    struct {
        BYTE UZ_alert:1;
        BYTE UZ_manual:1;
        BYTE UZ_busy:1;
        BYTE UZ_unlocked:1;
        BYTE UZ_locked:1;
        BYTE reserved:3;
    }  fields;
    BYTE bValue;
} __attribute__((packed)) uzState_type;

errType getUZstate(void* fn)
{
    errType result = err_result_ok;

    functionNode* func = (functionNode*) fn;

    func->printParams();

    BYTE UZ_id=*(BYTE*)(func->getParamPtr(0));
    uzState_type UZ;

    UZ.fields.UZ_locked   = answerFrame->isUZ_locked((FoldDscr_type) UZ_id);
    UZ.fields.UZ_unlocked = answerFrame->isUZ_unlocked((FoldDscr_type) UZ_id);
    UZ.fields.UZ_busy     = answerFrame->isUZ_busy((FoldDscr_type) UZ_id);
    UZ.fields.UZ_manual   = answerFrame->isUZ_manual((FoldDscr_type) UZ_id);
    UZ.fields.UZ_alert    = answerFrame->isUZ_alert((FoldDscr_type) UZ_id);

    func->setResult(1, &UZ_id);
    func->setResult(2, &UZ);

    func->printResults();

    return result;
}

errType getAllUZstate(void* fn)
{
    errType result = err_result_ok;
    functionNode* func = (functionNode*) fn;
    uzState_type UZ[3];

    for(int i=0; i<3; i ++)
    {
        UZ[i].bValue = 0;                       //cleaning bits
        UZ[i].fields.UZ_locked   = answerFrame->isUZ_locked((FoldDscr_type) i);
        UZ[i].fields.UZ_unlocked = answerFrame->isUZ_unlocked((FoldDscr_type) i);
        UZ[i].fields.UZ_busy     = answerFrame->isUZ_busy((FoldDscr_type) i);
        UZ[i].fields.UZ_manual   = answerFrame->isUZ_manual((FoldDscr_type) i);
        UZ[i].fields.UZ_alert    = answerFrame->isUZ_alert((FoldDscr_type) i);
    }

    func->setResult(1, &UZ[0]);
    func->setResult(2, &UZ[1]);
    func->setResult(3, &UZ[2]);
    
    return result;
}

typedef union semiaxisSensorsVector_t {
    struct{
	struct {
	    union {
		struct { 
		    BYTE right:1;
		    BYTE left:1;
		} fields;
		BYTE sa;
	    } psa;
	    
	    union {
		struct {
		    BYTE right:1;
		    BYTE left:1;
		} fields;
		BYTE sa;
	    } esa;
	    
	} fold[3];
    } fields;
    
    WORD wValue;
} __attribute__((packed)) semiaxisSensorsVector_t;

errType allFoldsGetParams(void* fn)
{
    errType result=err_result_ok;
    functionNode* func = (functionNode*) fn;
    BYTE foldState;
    WORD foldPos;
    
    for (int i=0; i<3; i++){
        foldState = answerFrame->getFoldState((FoldDscr_type) i);
        foldPos = answerFrame->getFoldPosition((FoldDscr_type) i);
	func->setResult(2*i+1, &foldState); // index: 1, 3 ,5
	func->setResult(2*i+2, &foldPos);   // index: 2, 4, 6
    }
    
    return result;
}
