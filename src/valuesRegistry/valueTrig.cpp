#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "../global.h"
#include "valueTrig.h"

valueTrig::valueTrig(){
	trigDef=new trigDef_type;
	trigDef->oldValue=0;
}

valueTrig::~valueTrig(){
	delete [](BYTE*)trigDef->oldValue; // deleting void* is undefined
	delete trigDef;
}



//const void* valueTrig::getTrigValuePtr(){ return trigDef->value; }
const void* valueTrig::getTrigOldValuePtr(){ return trigDef->oldValue; }
WORD valueTrig::getTrigIndex() {return trigDef->index; }

void valueTrig::dbgPrint(){
		printf("\n\ttrigger old value: %d\n", *(int*)trigDef->oldValue);
//		printf("\t trigger curr value: %d\n",*(int*)trigDef->value);
		//printf("\t trigger value type: %s\n",strOrtsTypes[trigDef->type]);
		printf("\t recipient addr:\n");
		printf("\t recipient port:\n");
}


DWORD valueTrig::getSize(OrtsType type)
{
    return sizeof(WORD)+2*lenOrtsTypes[type]+sizeof(sockaddr_in);
}

//errType valueTrig::decode(BYTE* array, DWORD* size)
//{
//    errType result=err_not_init;
//
//
//    return result;
//}
//errType valueTrig::encode(BYTE* array)
//{
//    errType result=err_not_init;
//
//    return result;
//}

bool valueTrig::triggerCheck(OrtsType currType, const void *currValue)
{
    bool result=false;

    switch(currType)
            {
            case type_WORD:
              result=(*(WORD*)getTrigOldValuePtr()!=*(WORD*)currValue);
              break;
            case type_DWORD:
              result=(*(DWORD*)getTrigOldValuePtr()!=*(DWORD*)currValue);
              break;
            case type_FLOAT:
              result=(*(float*)getTrigOldValuePtr()!=*(float*)currValue);
              break;
            default: //BYTE, errType
              result=(*(BYTE*)getTrigOldValuePtr()!=*(BYTE*)currValue);
              break;
            }

    return result;
}

errType valueTrig::triggerSet(WORD index, OrtsType type, const void* valuePtr)
{
        errType result=err_not_init;
        trigDef->index=index;
        WORD size=lenOrtsTypes[type];

//      if ((registry==0) || (index=0))  result=err_result_error;
//      else {

        delete [](BYTE*)trigDef->oldValue; // deleting void* is undefined

        trigDef->oldValue=new BYTE[size];
        memcpy(trigDef->oldValue, valuePtr, size);
        result=err_result_ok;
        return result;
}
