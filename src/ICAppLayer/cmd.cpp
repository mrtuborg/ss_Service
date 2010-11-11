#include <stdio.h>
#include <string.h>
#include "../myTypes.h"
#include "cmd.h"
#include "crc16.h"

cmd::cmd()
{
}

cmd::~cmd()
{
}

BYTE cmd::get_func_id()
{
    return func_id;
}

void cmd::dbgPrint()
{
  printf("cmd_length=%d\n",getCmdLength());
  printf("func_id: %d\n", func_id);
  printf("func_paramsLength:%lu\n",func_paramsLength);
  
  printf("parameter [");
  for (int i=0; i<func_paramsLength; i++){
      printf("%.2X ",func_params[i]);
  }
  printf("]\n");
  printf("crc: %.4X\n", crc16_signature);
}

errType cmd::decode(BYTE* data)
{
	errType result=err_not_init;
	//*data=new BYTE[5+func_paramsLength];
        //1. Copyng static part of cmd type:
	    memcpy(data, this,sizeof(func_id)+sizeof(func_paramsLength));
 	//2. Decoding dynamic part from dataBlock:
	    memcpy(data+sizeof(func_id)+sizeof(func_paramsLength), func_params, func_paramsLength);
	    memcpy(data+sizeof(func_id)+sizeof(func_paramsLength)+func_paramsLength, &crc16_signature, sizeof(crc16_signature));
	//3. Debug print:
	//    this->dbgPrint();
	return result;
}

errType cmd::encode(BYTE* data)
{
	errType result=err_not_init;
	BYTE func_id=data[0];
	func_paramsLength=*(DWORD*)(data+1);
	encode(func_id, func_paramsLength, data+sizeof(func_id)+sizeof(func_paramsLength));
	crc16_signature=*(WORD*)(data+getCmdLength()-2);
        return result;
}


errType cmd::encode(BYTE func_num, DWORD par_length, BYTE* data)
{
	errType result=err_not_init;
	BYTE tmp[255];
	
        //1. Copyng static part of cmd type:
	     func_id=func_num;
	     func_paramsLength=par_length;
	     crc16_signature=0;
	     if (func_paramsLength>0) {
	//2. Creating dynamic part of cmd type:
	        func_params=new BYTE[func_paramsLength];
	//3. Decoding dynamic part from dataBlock:
	        memcpy(func_params, ((BYTE*)data), func_paramsLength);
	        
	//4. Debug print:
	    }
	     
	return result;
}


DWORD cmd::get_func_paramsLength()
{
     return func_paramsLength;
}
  
DWORD cmd::getCmdLength()
{
     return func_paramsLength+sizeof(func_id)+sizeof(crc16_signature)+sizeof(func_paramsLength);
}
  
BYTE* cmd::get_func_paramsPtr(DWORD offset)
{
    return func_params+offset;
}

WORD cmd::get_crc_sign()
{
    return crc16_signature;
}

errType cmd::makeSign()
{
    crc16_signature=0;
    if (func_paramsLength>0) crc16_signature=CRC16_eval((BYTE*)func_params,func_paramsLength);
    crc16_signature=CRC16_eval((BYTE*)&func_paramsLength,4,crc16_signature);
    crc16_signature=CRC16_eval((BYTE*)&func_id,1,crc16_signature);
	     
    return err_result_ok;
}

bool cmd::checkSign()
{
    bool result=false;
    cmd test_cmd;
    WORD test_sign;
    test_cmd.encode(this->func_id, this->func_paramsLength, func_params);
    test_cmd.makeSign();
    test_sign=test_cmd.get_crc_sign();
    //printf("test sign: %.4X\n",test_sign);
    //printf("rcvd sign: %.4X\n",this->get_crc_sign());
    if (test_sign==this->get_crc_sign()) result=true;
    return result;
}