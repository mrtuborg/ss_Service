#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../myTypes.h"
#include "PS_CmdFrame.h"

PS_CmdFrame::PS_CmdFrame()
{
    msg = new ps_cmdframe_type;
}

PS_CmdFrame::~PS_CmdFrame()
{
    delete msg;
}

errType PS_CmdFrame::decode(BYTE** array)                                                                           
{                                                                                                                 
    memcpy(*array,msg,sizeof(ps_cmdframe_type));                                                                 
}                                                                                                                 
                                                                                                                  
errType PS_CmdFrame::encode(BYTE* array, DWORD size)                                                                
{                                                                                                                 
    memset(msg,0,sizeof(ps_cmdframe_type));                                                                                              
    memcpy(msg,array, size);                                                                                  
} 


errType PS_CmdFrame::prepParams_prg_mode(char* filename, BYTE *prepparams)
{
	errType result=err_result_ok;
	
	if (!prepparams) result=err_params_decode;
	if (!filename) result=err_params_decode;
	else if (!strlen(filename)) result=err_params_value;
	
	if (result==err_result_ok)
	{
	    memcpy(prepparams, filename, 12);
	}
	
	return result;
}

errType PS_CmdFrame::prepParams_srch_start(BYTE counts, BYTE step, BYTE stayTime, BYTE *prepparams)
{
	errType result=err_result_ok;
	
	if (!prepparams) result=err_params_decode;
	if (counts>99) result=err_params_value;
	if (step>99) result=err_params_value;
	if (stayTime>99) result=err_params_value;
	
	if (result==err_result_ok){
	    sprintf((char*)prepparams+0, "%.2d", counts);
	    sprintf((char*)prepparams+2, "%.2d", step);
	    sprintf((char*)prepparams+4, "%.2d", stayTime);
	}
	
	return result;
}

errType PS_CmdFrame::prepParams_new_AZ_corr(int16_t grad, BYTE min, BYTE sec, BYTE *prepparams)
{
	errType result=err_result_ok;
	
	if (!prepparams) result=err_params_decode;
	if (abs(grad)>360)   result=err_params_value;
	if (min>60) result=err_params_value;
	if (sec>60) result=err_params_value;
	
	if (result==err_result_ok)
	{
	    sprintf((char*)prepparams+0, "%d", grad);
	    sprintf((char*)prepparams+3, "%d", min);
	    sprintf((char*)prepparams+5, "%d", sec);
	}
	return result;
}

errType PS_CmdFrame::prepParams_new_EL_corr(BYTE grad, BYTE min, BYTE sec, BYTE *prepparams)
{
	errType result=err_result_ok;
	
	if (!prepparams) result=err_params_decode;
	if (grad>90)   result=err_params_value;
	if (min>60) result=err_params_value;
	if (sec>60) result=err_params_value;
	
	if (result==err_result_ok)
	{
	    sprintf((char*)prepparams+0, "%d", grad);
	    sprintf((char*)prepparams+2, "%d", min);
	    sprintf((char*)prepparams+4, "%d", sec);
	}
	
	return result;
}

errType PS_CmdFrame::prepParams_new_TM_corr(int16_t sec, uint16_t msec, BYTE *prepparams)
{
	errType result=err_result_ok;
	
	
	if (!prepparams) result=err_params_decode;
	
	if (abs(sec)>60)   result=err_params_value;
	if (msec>999) result=err_params_value;

	if (result==err_result_ok)
	{
	    sprintf((char*)prepparams+0, "%d", sec);
	    sprintf((char*)prepparams+3, "%d", msec);
	}
	
	return result;
}

errType PS_CmdFrame::prepMsg(ps_cmd_id_type cmd, BYTE* params)
{
    errType result=err_result_ok;
    
    if (!params) result=err_params_decode;
    msg->cmd_id=cmd;
    switch (cmd)
    {
	case _mode_off:
	    msg->len=1;
	    msg->params=params;
	    break;
	case _mode_prg:
	    msg->len=16;
	    msg->params=params;
	    break;
	case _mode_KN:
	    msg->len=1;
	    msg->params=params;
	    break;
	case _mode_srchStart:
	    msg->params=params;
	    msg->len=7;
	    break;
	case _mode_srchStop:
	    msg->len=1;
	    msg->params=params;
	    break;
	case _mode_ZVV:
	    msg->len=1;
	    msg->params=params;
	    break;
	case _corr_AZ:
	    msg->len=9;
	    msg->params=params;
	    break;
	case _corr_EL:
	    msg->len=7;
	    msg->params=params;
	    break;
	case _corr_TM:
	    msg->len=7;
	    msg->params=params;
	    break;
	default:
	    result=err_params_decode;
	    break;
    }
    return result;
}
