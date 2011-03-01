#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include <string.h>
#include <extra/ortsTypes/ortsTypes.h>
#include "comm_SASC.h"

const WORD comm_SASC::kSASCMsgSize = sizeof(SASC_msg_type);

char SASC_answer_str[13][255]=                                                  
{                                                                               
    {"готов"},                                                                  
    {"выкл"},                                                                   
    {"измерения готовы"},                                                       
    {"измерения нуля готовы"},                                                  
    {"связь в норме"},                                                          
    {"запись БД"},                                                              
    {"последняя запись БД"},                                                    
    {"БД пуста"},                                                               
    {"тарировка включена"},                                                     
    {"тарировка отключена"},                                                    
    {"параметры тарировки"},                                                    
    {"занят"},                                                                  
    {"ожидание"}                                                                 
};


comm_SASC::comm_SASC()
{
    msg=new SASC_msg_type;
}

comm_SASC::~comm_SASC()
{
    delete msg;
}

errType comm_SASC::clear_msg()
{
    errType result=err_result_ok;
    memset (msg,0, comm_SASC::kSASCMsgSize);
    return result;
}

errType comm_SASC::apply_mod(SASC_cmd_mod mode, BYTE **params)
{
    errType result=err_result_ok;
    clear_msg();
    
    msg->time=(DWORD) time((time_t*)&msg->time);
    //printf("dword time=%.8X\n", msg->time);
    //printf("time is=%s", ctime((time_t*)&msg->time));
    switch(mode)
    {
	case _power_on:
	    msg->typeinf=1;
	break;
	
	case _power_off:
	    msg->typeinf=3;
	break;
	
	case _measure: // params length is 2 * sizeof(DWORD) + 5* sizeof(float)
	    if ((params==0) || (*params==0)) result= err_params_decode;
	    else {
		msg->typeinf=2;
		msg->binning=*(uint32_t*)params[0];
		msg->Niter=*(uint32_t*)params[1];
		msg->FiH=*(float*)params[2];
		msg->FiA=*(float*)params[3];
		msg->TC=*(float*)params[4];
		msg->Wm_c=*(float*)params[5];
		msg->Wa=*(float*)params[6];
	    }
	break;
	
	case _zero_measure: // params length is 1 * sizeof(DWORD) + 5* sizeof(float)
	    if ((params==0) || (*params==0)) result=err_params_decode;
	    {
		msg->typeinf=2;
		msg->init=1;
		msg->binning=*(uint32_t*)params[0];
		msg->Niter=1;
		msg->FiH=*(float*)params[1];
		msg->FiA=*(float*)params[2];
		msg->TC=*(float*)params[3];
		msg->Wm_c=*(float*)params[4];
		msg->Wa=*(float*)params[5];
	    }
	break;
	
	case _link_test:
	    msg->typeinf=4;
	break;
	
	case _get_db:
	    msg->typeinf=5;
	break;
	
	case _get_next_record:
	    msg->typeinf=11;
	break;
	
	case _erase_db:
	    msg->typeinf=6;
	break;
	
	case _tare_start: // params length is 5 * sizeof(float)
	    if ((params==0) || (*params==0)) result=err_params_decode;
	    else {
		msg->typeinf=7;
		msg->FiH=*(float*)params[0];
		msg->FiA=*(float*)params[1];
		msg->TC=*(float*)params[2];
		msg->Wm_c=*(float*)params[3];
		msg->Wa=*(float*)params[4];
	    }
	break;                       
	
	case _get_tare_value: // params length is 10 * sizeof(float) + 2 * sizeof (DWORD)
	    if ((params==0) || (*params==0)) result=err_params_decode;
	    else {
		msg->typeinf=8;
		msg->binning=*(uint32_t*)params[0];
		msg->Niter=*(uint32_t*)params[1];
		msg->FiH=*(float*)params[2];
		msg->FiA=*(float*)params[3];
		msg->TC=*(float*)params[4];
		msg->Wm_c=*(float*)params[5];
		msg->Wa=*(float*)params[6];
		msg->X=*(float*)params[7];
		msg->Y=*(float*)params[8];
		msg->Z=*(float*)params[9];
		msg->Fix=*(float*)params[10];
		msg->Fiy=*(float*)params[11];
	    }
	break;
	
	case _tare_stop: // params length is 5 * sizeof(float);
	    if ((params==0) || (*params==0)) result=err_params_decode;
	    else {
		msg->typeinf=9;
		msg->FiH=*(float*)params[0];
		msg->FiA=*(float*)params[1];
		msg->TC=*(float*)params[2];
		msg->Wm_c=*(float*)params[3];
		msg->Wa=*(float*)params[4];
	    }

	break;
	
	default:
	    result=err_not_init;
	break;
    }
    return result;
}

errType comm_SASC::checkAnswer(SASC_answer_mod *typeinf)
{
    errType result=err_result_ok;
    switch (msg->typeinf)
    {
	case 0:
	    *typeinf=_awaiting;
	    break;
	case 51: // 0x33
	    *typeinf=_ready;
	    break;
	case 52: // 0x34
	    if (msg->init==1) *typeinf=_zero_ready;
	    else *typeinf=_measure_ready;
	    break;
	case 53: // 0x35
	    *typeinf=_off;
	    break;
	case 54: // 0x36
	    *typeinf=_link_norm;
	    break;
	case 55: // 0x37
	    *typeinf=_db_record;
	    break;
	case 50: // 0x32
	    *typeinf=_db_last_record;
	    break;
	case 56: // 0x38
	    *typeinf=_db_empty;
	    break;
	case 57: // 0x39
	    *typeinf= _tare_on;
	    break;
	case 58: // 0x3A
	    *typeinf=_tare_params;
	    break;
	case 59: // 0x3B
	    *typeinf=_tare_off;
	    break;
	case 99: // 0x63
	    *typeinf=_busy;
	    break;
	default:
	    result=err_not_init;
	    break;
    }
    return result;
}

errType comm_SASC::decode(BYTE* array)
{
        memcpy(array,msg,comm_SASC::kSASCMsgSize);
}

errType comm_SASC::encode(BYTE* array, DWORD size)
{
        clear_msg();
	memcpy(msg,array, size);
}

void comm_SASC::dbgPrint()
{
    uint64_t time_val=msg->time;
    printf("\ttypeinf=%d\n", msg->typeinf);                                                               
    printf("\tinit=%d\n", msg->init);                                                                  
    printf("\tbinning=%d\n", msg->binning);                                                               
    printf("\tNiter=%d\n", msg->Niter);                                                                 
    printf("\ttime=%s", ctime((time_t*)&time_val));                                                                  
    printf("\tFiH=%f\n",msg->FiH);                                                                      
    printf("\tFiA=%f\n",msg->FiA);                                                                      
    printf("\tTC=%f\n",msg->TC);                                                                       
    printf("\tWm_c=%f\n",msg->Wm_c);                                                                     
    printf("\tWa=%f\n",msg->Wa);                                                                       
    printf("\tX=%f\n",msg->X);                                                                        
    printf("\tY=%f\n",msg->Y);                                                                        
    printf("\tZ=%f\n",msg->Z);                                                                        
    printf("\tFix=%f\n",msg->Fix);                                                                      
    printf("\tFiy=%f\n",msg->Fiy);                                                                      
}
