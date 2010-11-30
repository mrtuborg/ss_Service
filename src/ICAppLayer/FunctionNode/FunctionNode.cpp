#include <stdio.h>
#include <string.h>
#include "../../../../rcsLib/ortsTypes/ortsTypes.h"
#include "param_desc.h"
#include "../../../../rcsLib/rcsCmd/rcsCmd.h"
#include "FunctionNode.h"


FunctionNode::FunctionNode(BYTE id, WORD parQnt, WORD resQnt, funcPtr ptr) //+function pointer
{
    func_id=id;
    func_paramsQuantity=parQnt;
    func_resultsQuantity=resQnt;
    func_paramsLength=0;
    func_resultsLength=0;
    
    for (int i=0; i<32; i++) func_params[i]=0;
    
    func=ptr;
}

FunctionNode::~FunctionNode()
{
    for (int i=0; i<func_paramsQuantity; i++) delete func_params[i];
    for (int i=0; i<func_resultsQuantity; i++) delete func_results[i];
}

WORD FunctionNode::getParamsQuantity ()
{
    return func_paramsQuantity;
}
errType FunctionNode::setParamDescriptor (BYTE num, OrtsType type)
{
	errType result=err_not_init;
	if (!(type & 0xF0)) func_params[num]=new param_desc(type, lenOrtsTypes[type]);
	else (func_params[num])=new param_desc(type);
	
	if (func_params[num]) result=err_result_ok;
	func_paramsLength=0;
	
	for(int i=0; i<func_paramsQuantity; i++){
	    if (func_params[i]) func_paramsLength+=func_params[i]->length();
	}
	return result;
}

errType FunctionNode::setResultDescriptor(BYTE num, OrtsType type)
{
	errType result=err_not_init;
	
	// vector types are greater than 0x10
	//printf("func_id=%d, num=%d, is not Vector=%d\n", func_id, num, !(type & 0xF0));
	
	if (!(type & 0xF0)) (func_results[num])=new param_desc(type, lenOrtsTypes[(int)type]);
	else {
	    (func_results[num])=new param_desc(type);
	    //printf("Vector! func_id=%d, num=%d\n",func_id, num);
	}
	
	if (func_results[num]) result=err_result_ok;
	return result;
}

WORD FunctionNode::getParamLength(BYTE num)
{
    WORD result=0;
    if (num<func_paramsQuantity) result=(BYTE)((func_params[num])->length());
    return result;

}

WORD FunctionNode::getAllParamsLength()
{
     func_paramsLength=0;
     for(int i=0; i<func_paramsQuantity; i++){
	    if (func_params[i]) func_paramsLength+=func_params[i]->length();
     }
     return func_paramsLength;
}

DWORD FunctionNode::getAllResultsLength()
{
     func_resultsLength=0;
     for(int i=0; i<func_resultsQuantity; i++){
	    if (func_results[i]) func_resultsLength+=func_results[i]->length();
     }
     return func_resultsLength;
}

DWORD FunctionNode::getResultLength(BYTE res_no)
{
	return func_results[res_no]->length();
}


BYTE FunctionNode::getResultsQuantity()
{
    return func_resultsQuantity;
}



errType FunctionNode::setParam(BYTE num, void* param)
{
	errType result=err_not_init;
	result=func_params[num]->setParam(param);
	return result;
}

void* FunctionNode::getParamPtr(BYTE num)
{
	if (func_params[num]) return func_params[num]->value();
	else return 0;
}

errType FunctionNode::decodeParams(rcsCmd* packet)//BYTE* paramsPtr)
{
	errType result=err_not_init;
	WORD len=0, offset=0;
	BYTE *param_tmp;
	
	
	func_paramsLength=packet->get_func_paramsLength();
	
	
	int rcvdPacket_length=packet->getCmdLength();
	int descPacket_length=packet->getCmdLength()-func_paramsLength;
	
	
	param_tmp=new BYTE[func_paramsLength];
	
	// check received params quantity with params description
	
	WORD paramOffset=0;
	if (func_params)
	{
	    for (int i=0; i<func_paramsQuantity; i++){
		if (func_params[i]) {
		    if  (func_params[i]->isVector()) {
		     WORD vectorSize=*(WORD*)((BYTE*)packet->get_func_paramsPtr()+paramOffset);
		     
		     descPacket_length+=vectorSize+sizeof(WORD);
		     paramOffset+=vectorSize+sizeof(WORD);
		    }
		    else {
			descPacket_length+=func_params[i]->length();
			paramOffset+=func_params[i]->length();
		    }
		}
	    }
	}
	
	//printf("decode received: %d bytes\n",rcvdPacket_length);
	//printf("by description: %d bytes\n",descPacket_length);
	paramOffset=0;
	if (rcvdPacket_length!=descPacket_length) result=err_result_error;
	else {
		for (int i=0; i<func_paramsQuantity; i++){
		    if  (func_params[i]->isVector())
		    {
			WORD vectorSize=*(WORD*)((BYTE*)packet->get_func_paramsPtr()+paramOffset);
			memcpy(param_tmp, (BYTE*)packet->get_func_paramsPtr()+paramOffset, vectorSize+sizeof(WORD));
			func_params[i]->setParam(param_tmp);
			paramOffset+=vectorSize+sizeof(WORD);
		    }
		    else {
			len=func_params[i]->length();
			memcpy(param_tmp, (BYTE*)packet->get_func_paramsPtr()+paramOffset, len);
			func_params[i]->setParam(param_tmp);
			
			paramOffset+=func_params[i]->length();
		    }
		}
		result=err_result_ok;
	}
	//delete param_tmp; Will deleted in param_desc destructor
	return result;
}

errType FunctionNode::setResult(BYTE num, void* res)
{
	errType result=err_not_init;
	OrtsType type;
	
	if (func_results[num]) {
	    result=func_results[num]->setParam(res);
	} else printf ("Внутренняя ошибка: обращение к несущестаующему параметру %d функции №%d\n",num,func_id);
	
	return result;
}

errType FunctionNode::getResult(BYTE num, void** out_res, DWORD* length)
{
	errType result=err_not_init;
	*out_res=func_results[num]->value();
	*length=func_results[num]->length();
	
	return result;
}

errType FunctionNode::printParams()
{
    errType result=err_not_init;
    
    printf("\tВходные параметры функции: "); 
    if (func_paramsQuantity==0) printf("ОТСУТСТВУЮТ");
    else {
	printf("\n");
	for (int num=0; num<func_paramsQuantity; num++){
		printf("\t");
		if (func_params[num]) func_params[num]->printParam();
	}
    }
    
    printf("\n");
    
    return result;
}

errType FunctionNode::printResults()
{
    errType result=err_not_init;
    printf("\tВыходные результаты функции: "); 
    if (func_resultsQuantity==0) printf("ОТСУТСТВУЮТ");
    else {
	printf("\n");
	for (int num=0; num<func_resultsQuantity; num++){
	    if (func_results[num]) {
		printf("\t");
		func_results[num]->printParam();
	    }
	}
    }
    printf("\n");
    return result;
}

errType FunctionNode::callFunction() //+add timeout?
{
	errType result=err_not_init;
//	printf("Call function #%d!\n",func_id);
	printf("\tОбращение к функции \"%s\"\n",func_name);
	result=func(this);  // may be need a separate thread???

	return result;
}

BYTE FunctionNode::id()
{
	return func_id;
}

errType FunctionNode::setFuncName (const char *name)
{
	errType result=err_not_init;
	
	func_name=name;
	
	return result;
}

errType FunctionNode::setParamName (BYTE num, const char *name)
{
	errType result=err_not_init;
	
	if (func_params[num])func_params[num]->setName(name);
	
	return result;
}

errType FunctionNode::setResultName(BYTE num, const char *name)
{
	errType result=err_not_init;
	
	if (func_results[num])func_results[num]->setName(name);
	
	return result;
}
                                

void FunctionNode::dbgPrint()
{
	int len=0;
	BYTE *val=0;
	
	printf("--------------------\n");
	printf(" Function #%d\n", func_id);
	printf("====================\n");
	printf(" paramsQuantity=%d\n",func_paramsQuantity);
	printf(" resultsQuantity=%d\n",func_resultsQuantity);
	printf(" TotalParamsLength=%d\n", func_paramsLength);
	
	for(int i=0; i<func_paramsQuantity; i++) {
	    len=(func_params[i])->length();
	    val=(BYTE*)(func_params[i])->value();
	    //printf("\n param[%d] pointer=%p\n",i,func_params[i]->value());
	    printf(" param[%d] length=%d\n",i, len);
	    printf(" param[%d]: 0x", i);
	    for (int k=0; k<len;k++){
		printf("%.2X", val[k]);
	    }
	    printf("\n");
	    
	}
	printf("\n");
	printf("--------------------\n");
}
