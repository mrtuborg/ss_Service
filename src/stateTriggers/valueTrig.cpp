#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../../ortsTypes/ortsTypes.h"
#include "../global.h"
#include "valueTrig.h"

valueTrig::valueTrig(){
	trigDef=new TrigDef_type;
	trigDef->oldValue=0;
}

valueTrig::~valueTrig(){
	delete trigDef->oldValue;
	delete trigDef;
}

errType valueTrig::installTrigValuePtr(void* valuePtr, OrtsType type){
		errType result=err_not_init;
		BYTE size=lenOrtsTypes[type];
		if ((valuePtr==0) || (size=0))  result=err_result_error;
		else {
			delete trigDef->oldValue;
			trigDef->valType=type;
			trigDef->value=valuePtr;
			trigDef->oldValue=new BYTE[size];
		    memcpy(trigDef->oldValue, trigDef->value, size);
		    result=err_result_ok;
		}
		return result;
}

const void* valueTrig::getTrigValuePtr(){ return trigDef->value; }
const void* valueTrig::getTrigOldValuePtr(){ return trigDef->oldValue; }

void valueTrig::dbgPrint(){
		printf("\n\ttrigger old value: %lu\n", trigDef->oldValue);
		printf("\t trigger curr value: %lu\n",trigDef->value);
		printf("\t trigger value type: %s\n",strOrtsTypes[trigDef->type]);
		printf("\t recipient addr:\n");
		printf("\t recipient port:\n");
}


DWORD valueTrig::getSize()
{
    return sizeof(OrtsType)+2*lenOrtsType[typeDef->type]+sizeof(inaddr_in);
}

errType valueTrig::decode(BYTE* array, DWORD* size)
{
    errType result=err_not_init;

    
    return result;
}
errType valueTrig::encode(BYTE* array)
{
    errType result=err_not_init;
    
    return result;
}

bool valueTrig::triggerSet()
{
    bool result=false;

    switch (type)
    {
      case type_BYTE:
           result=((BYTE*) typeDef->oldValue == ((BYTE*) typeDef->Value));
           break;
      case type_WORD:
           result=((WORD*) typeDef->oldValue == ((WORD*) typeDef->Value));
           break;
      case type_DWORD:
           result=((DWORD*) typeDef->oldValue == ((DWORD*) typeDef->Value));
           break;
    }
    return result;
}

void valueTrig::triggerReset()
{
  delete trigDef->oldValue;
  trigDef->oldValue=new BYTE[size];
  memcpy(trigDef->oldValue, trigDef->value, size);
}
