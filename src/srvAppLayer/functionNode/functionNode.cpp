/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class functionNode implementation.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */
#include <stdio.h>
#include <string.h>
#include "rcsLib/ortsTypes/ortsTypes.h"
#include "param_desc.h"
#include "rcsLib/rcsCmd/rcsCmd.h"
#include "functionNode.h"

/**********************************************************************************//**
 * @brief       functionNode constructor.
 * @details     uses to declare service function.
 * @param[in]   id - function identifier.
 * @param[in]   parQnt - quantity of function parameters.
 * @param[in]   resQnt - quantity of function results.
 * @param[in]   ptr - function execution code pointer.
 **************************************************************************************/
functionNode::functionNode(BYTE id, WORD parQnt, WORD resQnt, funcPtr ptr) //+function pointer
{
    func_id=id;
    func_paramsQuantity=parQnt;
    func_resultsQuantity=resQnt;
    func_paramsLength=0;
    func_resultsLength=0;
    
    for (int i=0; i<32; i++) func_params[i]=0;
    
    func=ptr;
}

functionNode::~functionNode()
{
    for (int i=0; i<func_paramsQuantity; i++) delete func_params[i];
    for (int i=0; i<func_resultsQuantity; i++) delete func_results[i];
}

/*******************************************//**
 * Getter for function parameters quantity
 **********************************************/
WORD functionNode::getParamsQuantity ()
{
    return func_paramsQuantity;
}
/*********************************************************************//**
 * Method  declares parameter for function node
 * @param  num - parameter index (zero based index)
 * @param  type - parameter type with respect to OrtsType enumeration
 * @return err_result_ok - declaring successful
 ************************************************************************/
errType functionNode::setParamDescriptor (BYTE num, OrtsType type)
{
	errType result=err_result_ok;
	if (!(type & 0xF0)) func_params[num]=new param_desc(type, lenOrtsTypes[type]);
	else (func_params[num])=new param_desc(type);
	
	if (func_params[num]) result=err_result_ok;
	func_paramsLength=0;
	
	for(int i=0; i<func_paramsQuantity; i++){
	    if (func_params[i]) func_paramsLength+=func_params[i]->length();
	}
	return result;
}

/*********************************************************************//**
 * Method  declares results for function node
 * @param  num - result index (zero based index)
 * @param  type - result type with respect to OrtsType enumeration
 * @return err_result_ok - declaring successful
 *         err_not_init - declaring unsuccessful
 ************************************************************************/
errType functionNode::setResultDescriptor(BYTE num, OrtsType type)
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

/***********************************************//**
 * Method evaluate function node parameter size
 * @param num - parameter index (zero based index)
 * @return size of parameter in bytes
 ***************************************************/
WORD functionNode::getParamLength(BYTE num)
{
    WORD result=0;
    if (num<func_paramsQuantity) result=(BYTE)((func_params[num])->length());
    return result;

}

/*********************************************************//**
 * Method calculate sum of all function node parameters size
 * @return size of all parameters in bytes
 *************************************************************/
WORD functionNode::getAllParamsLength()
{
     func_paramsLength=0;
     for(int i=0; i<func_paramsQuantity; i++){
	    if (func_params[i]) func_paramsLength+=func_params[i]->length();
     }
     return func_paramsLength;
}


/*********************************************************//**
 * Method calculate sum of all function node results size
 * @return size of all results in bytes
 *************************************************************/
DWORD functionNode::getAllResultsLength()
{
     func_resultsLength=0;
     for(int i=0; i<func_resultsQuantity; i++){
	    if (func_results[i]) func_resultsLength+=func_results[i]->length();
     }
     return func_resultsLength;
}

/***********************************************//**
 * Method evaluate function node result size
 * @param num - result index (zero based index)
 * @return size of result in bytes
 ***************************************************/
DWORD functionNode::getResultLength(BYTE res_no)
{
	return func_results[res_no]->length();
}

/*******************************************//**
 * Getter for function results quantity
 **********************************************/
BYTE functionNode::getResultsQuantity()
{
    return func_resultsQuantity;
}


/***************************************************//**
 * Set value for declared parameter
 * @param num - parameter index (zero based index)
 * @param param - pointer to value
 * @return err_result_ok - value setting was successful
 ******************************************************/
errType functionNode::setParam(BYTE num, void* param)
{
	errType result=err_result_ok;
	result=func_params[num]->setParam(param);
	return result;
}

/***************************************************//**
 * Get value pointer of declared parameter
 * @param num - parameter index (zero based index)
 * @return nonzero - value pointer
 * @return 0       - parameter not exist
 ******************************************************/
void* functionNode::getParamPtr(BYTE num)
{
	if (func_params[num]) return func_params[num]->value();
	else return 0;
}

/***************************************************************************************//**
 * Received message parameters decode.
 * @details check parameters quantity and fills parameters pointers with received values
 * @param packet - received rcsCmd message
 * @return err_result_error - received parameters does not match to declaration
 * @return err_result_ok    - received parameters successfully decodes
 *******************************************************************************************/
errType functionNode::decodeParams(rcsCmd* packet)//BYTE* paramsPtr)
{
	errType result=err_not_init;
	WORD len=0, offset=0;
	BYTE *param_tmp;
	
	/// 1. Define length of parametric part by function declaration
	//printf("1. Define length of parametric part by function declaration\n");
	func_paramsLength=packet->get_func_paramsLength();
	//printf("func_paramsLength=%d\n",func_paramsLength);
	/// 2. Count length of parametric part of received request message
	//printf("2. Count length of parametric part of received request message\n");
	int rcvdPacket_length=packet->getCmdLength();
	//printf("rcvdPacket_length=%d\n",rcvdPacket_length);
	int descPacket_length=packet->getCmdLength()-func_paramsLength;
	//printf("descPacket_length=%d\n",descPacket_length);
	
	param_tmp=new BYTE[func_paramsLength];
	
	/// 3. Compare received params quantity with declared params quantity
	//printf("3. Compare received params quantity with declared params quantity\n");
	WORD paramOffset=0;
	if (func_params)
	{
	    //printf("func_params exists!\n");
	   // printf("func_paramsQuantity=%d\n",func_paramsQuantity);
	    for (int i=0; i<func_paramsQuantity; i++){
		if (func_params[i]) {
		   // printf("func_params[%d] exists!\n",i);
		    if  (func_params[i]->isVector()) {
		       // printf("func_params[%d] is Vector!\n",i);
		        WORD vectorSize=*(WORD*)((BYTE*)packet->get_func_paramsPtr()+paramOffset);
		     
		        descPacket_length+=vectorSize+sizeof(WORD);
		      //  printf("descPacket_length=%d\n",descPacket_length);
		        paramOffset+=vectorSize+sizeof(WORD);
		       // printf("paramOffset=%d\n",paramOffset);
		    }
		    else {
		       // printf("func_params[%d] is scalar!\n",i);
			descPacket_length+=func_params[i]->length();
			//printf("descPacket_length=%d\n",descPacket_length);
			paramOffset+=func_params[i]->length();
			//printf("paramOffset=%d\n",paramOffset);

		    }
		}
	    }
	}
	
	/// 4. If received params quantity is equal to declared params quantity - fills parameters values.
	///    otherwise -  return value is err_result_error.
	//printf("4. If received params quantity is equal to declared params quantity - fills parameters values. Otherwise -  return value is err_result_error\n");
	paramOffset=0;
	if (rcvdPacket_length!=descPacket_length) {
	  //  printf("rcvdPacket_length!=descPacket_length\n");
	    result=err_result_error;
	}
	else {
		for (int i=0; i<func_paramsQuantity; i++){
		    if (func_params[i])
		      {
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
		      } else result=err_not_found;
		}
		result=err_result_ok;
	}
	//delete param_tmp; Will deleted in param_desc destructor
	return result;
}

/*******************************************************************//**
 * Setter function for function node result storage.
 * @param num[in]   - number of result in declaration of function node
 * @param res[in]   - pointer to result value
 * @return err_result_ok - value successfully sets
 ***********************************************************************/
errType functionNode::setResult(BYTE num, void* res)
{
	errType result=err_result_ok;
	OrtsType type;
	
	if (func_results[num]) {
	    result=func_results[num]->setParam(res);
	} else printf ("Внутренняя ошибка: обращение к несуществующему параметру %d функции №%d\n",num,func_id);
	
	return result;
}

/********************************************************************//**
 * Getter function for function node result storage.
 * @param num[in]      - number of result in declaration of function node
 * @param out_res[out] - pointer to result value
 * @param length[out]  - length in bytes of stored value
 * @return err_result_ok - value gets successfully
 ************************************************************************/
errType functionNode::getResult(BYTE num, void** out_res, DWORD* length)
{
	errType result=err_result_ok;
	if (func_results[num]){
	    *out_res=func_results[num]->value();
	    *length=func_results[num]->length();
	} else result=err_not_found;
	return result;
}

/*************************************************************//**
 * Prints params values of function node for debug purposes
 * @return err_result_ok - values prints successfully
 *****************************************************************/
errType functionNode::printParams()
{
    errType result=err_result_ok;
    
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

/*************************************************************//**
 * Prints results values of function node for debug purposes
 * @return err_result_ok - values prints successfully
 *****************************************************************/
errType functionNode::printResults()
{
    errType result=err_result_ok;
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

/***********************************************************************//**
 * Call function node pointer to execution code
 * @return result - result code returns from calling code as return value
 ***************************************************************************/
errType functionNode::callFunction() //+add timeout?
{
	errType result=err_result_ok;
//	printf("Call function #%d!\n",func_id);
	printf("\tОбращение к функции \"%s\"\n",func_name);
	result=func(this);  // may be need a separate thread???

	return result;
}

/*****************************************//**
 * Getter for function node identifier
 * @return func_id - function identifier
 *********************************************/
BYTE functionNode::id()
{
	return func_id;
}

/****************************************//**
 * Setter for function node string name.
 * @param name - name of a function node.
 * @return err_result_ok
 ********************************************/
errType functionNode::setFuncName (const char *name)
{
	errType result=err_result_ok;
	
	func_name=name;
	
	return result;
}

/*************************************************//**
 * Setter for function node parameter string name.
 * @param num - number of a parameter
 * @param name - name of a parameter
 * @return err_result_ok
 *****************************************************/
errType functionNode::setParamName (BYTE num, const char *name)
{
	errType result=err_not_init;
	
	if (func_params[num])func_params[num]->setName(name);
	
	return result;
}

/*************************************************//**
 * Setter for function node result string name.
 * @param num - number of a result
 * @param name - name of a result
 * @return err_result_ok
 *****************************************************/
errType functionNode::setResultName(BYTE num, const char *name)
{
	errType result=err_not_init;
	
	if (func_results[num])func_results[num]->setName(name);
	
	return result;
}
                                
/***********************************************************************************//**
 * @brief Print for debug purposes contents of function node (in a hexadecimal notation)
 ***************************************************************************************/
void functionNode::dbgPrint()
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
