/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class param_desc implementation.
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
#include "extra/ortsTypes/ortsTypes.h"
#include "param_desc.h"

/**********************************************************************************************//**
 * Constructs parameter description with some type and information about storing value length.
 * @param type  - type (or simplified reference for vector) of storing value
 * @param param_len - length in bytes of storing value
 * @return
 *************************************************************************************************/
param_desc::param_desc(OrtsType type, WORD param_len)
{
	//printf("type=%d, len=%d\n",(BYTE)type, param_len);
	param=new BYTE[param_len];
	_length=param_len;
	_type=type;
	_isVector=false;
}

/**********************************************************************************************//**
 * Overloaded constructor that constructs parameter description of a vector type.
 * @param type  - type of storing value
 * @return
 *************************************************************************************************/
param_desc::param_desc(OrtsType type)
{
	//printf("type=%d, len=%d\n",(BYTE)type, param_len);
	//param=new BYTE[param_len];
	_length=0;
	_type=type;
	_isVector=true;
}

param_desc::~param_desc()
{
	
	delete [] param_name;
	delete [] (BYTE*) param;
}

/**********************************************************************************//**
 * Function calling if need to resize of storage for new value or something else...
 * @param new_size - size in bytes of new storage
 * @return err_result_ok - memory successfully reallocated
 * @return err_mem_alloc - error of allocating memory for new size
 * @return err_not_init - new size does not differ from old size
 **************************************************************************************/
errType param_desc::resize(WORD new_size) // need only for vectors type
{
	errType result=err_not_init;
	if (_length!=new_size){
	    delete [] (BYTE*)param;

	    _length=new_size;

	    param=new BYTE[_length];
	    
	    if (param==0) result=err_mem_alloc;
	    else result=err_result_ok;
	}
	return result;
}

/****************************************//**
 * Getter for length of storing value
 * @return length - storage size in bytes
 *******************************************/
WORD param_desc::length()
{
    return _length;
}

/****************************************//**
 * Getter for pointer of storing value
 * @return param - pointer to storing value
 *******************************************/
void* param_desc::value()
{
    return param;
}

/******************************************//**
 * Getter for type of storing value
 * @return type - pointer to storing value type
 **********************************************/
OrtsType param_desc::type()
{
    return _type;
}

/*******************************************************//**
 * Function checks if type of storing value is a vector
 * @return true - storing value is a vector
 * @return false - storing value is a scalar
 ***********************************************************/
bool param_desc::isVector()
{
    return (_type & 0xF0)!=0;
}

/*******************************************************************//**
 * Setter function for setting param value.
 * @param param_val[in]   - pointer to parameter value
 * @return err_result_ok - value was successfully set
 * @return err_params_value - null-pointer error
 ***********************************************************************/
errType param_desc::setParam(void* param_val)
{
    errType result = err_not_init;
    	
    if (param_val == 0) result = err_params_value;
    else {
        if (_isVector) resize(2 + *(WORD*)param_val);
     //   printf("*(WORD*)param_val=%d, _lenth=%d\n", *(WORD*)param_val, _length);
        memcpy(param, param_val, _length); // Need to copy for caller safety
        result = err_result_ok;
    }
    return result;
}
/****************************************//**
 * Setter for parameter string name.
 * @param name - name of a parameter.
 * @return err_result_ok
 ********************************************/
errType param_desc::setName(const char* name)
{
    errType result=err_not_init;
    param_name=new char[strlen(name)+1];
    strcpy(param_name,name);
    return result;
}

/*************************************************************//**
 * Print parameter value  for debug purposes
 * @return err_result_ok - value was printed successfully
 *****************************************************************/
errType param_desc::printParam()
{
    errType result (err_result_ok);
    float quantity (0);
    //printf("ptr: %p, ",param_name);
    //printf("%s\n", param_name);

    switch (_type)
    {
	case type_ERRTYPE:
	    printf("\t%s: %s\n", param_name, strErrTypes[*(BYTE*)param]);
	    break;
	
	case type_BYTE:
	    printf("\t%s: %d\t(0x%.2X)\n", param_name, *(BYTE*)param, *(BYTE*)param);
	    break;
	
	case type_WORD:
	    printf("\t%s: %d\t(0x%.4X)\n", param_name, *(WORD*)param, *(WORD*)param);
	    break;
	
	case type_CHAR:
	    printf("\t%s: %c\n", param_name, *(char*)param);
	    break;

	case type_DWORD:
	    printf("\t%s: %d\t(0x%.8X)\n", param_name, *(DWORD*)param, *(DWORD*)param);
	    break;
	
	case type_QWORD:
	    printf("\t%s: %llu\n", param_name, *(QWORD*)param);
	    break;
	
	case type_FLOAT:
	    printf("\t%s: %f\n", param_name, *(float*)param);
	    break;
	
	case type_DOUBLE:
	    printf("\t%s: %f\n", param_name, *(double*)param);
	    break;
	    
	case type_CHARVECTOR:
	    printf("\t%s: ", param_name);
	    for (int i=0; i<*(WORD*)param; i++) printf("%c",*((char*)param+2+i));
	    printf("\n");
	    break;

	case type_BYTEVECTOR:
		printf("\t%s: ", param_name);
		for (int i=0; i<*(WORD*)param; i++) printf(" %.2X",*((BYTE*)param+2+i));
		printf("\n");
	    break;

	case type_WORDVECTOR:
		printf("\t%s: ", param_name);
		for (int i=0; i<*(WORD*)param; i++) printf(" %.4X",*((WORD*)param+2+i));
		printf("\n");
	    break;

	case type_DWORDVECTOR:
		printf("\t%s: \n", param_name);
		quantity = (*(WORD*)param) / 4;
		//for (WORD i=0; i<quantity; i++) printf(" %.8X",*((DWORD*)param+2+i*4));
                for (int i=0; i<*(WORD*)param; i++) printf(" %.2X",*((BYTE*)param+2+i));

		printf("\n");
		break;

    }

    return result;
}


/***********************************************************************************//**
 * @brief Print for debug purposes contents of function node (in a hexadecimal notation)
 ***************************************************************************************/
void param_desc::dbgPrint()
{
    printf("param_desc ptr: %p\n",this);
    printf("param ptr: %p\n",param);
}
