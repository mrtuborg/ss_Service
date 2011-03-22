/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class functionNode interface header.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/**
 * @class functionNode
 * @brief function node interface header
 * @details function node is a service function that will call by srvAppLayer instance in accordance with client request
 */
class functionNode
{
    BYTE  func_id;              ///< function idintifier.
    const char* func_name;      ///< function name. For debug printing purposes.
    WORD  func_paramsQuantity;  ///< function declaration. Quantity of params.
    WORD  func_resultsQuantity; ///< function declaration. Quantity of results.
    
    WORD  func_paramsLength;    ///< Size in bytes of all function parameters.
    WORD  func_resultsLength;   ///< Size in bytes of all function reaults.
    
    param_desc *func_params[32]; ///< function parameters declarations. 32 parameters at maximum.
    param_desc *func_results[32];///< function results declaration. 32 results at maximum.
    
    bool mutator_;               ///< flag describing a purpose of function -
                                 ///< only reading parameters or mutating state of system
                                 ///< mutators will be forbidden for external clients in automatic mode
    funcPtr func;                ///< pointer to a function execution code.
    
    public:
	functionNode(BYTE id, WORD parQnt, WORD resQnt, funcPtr ptr);
	~functionNode();
	
	errType setFuncName (const char *name);
	errType setParamDescriptor (BYTE num, OrtsType type);
	errType setResultDescriptor(BYTE num, OrtsType type);
	errType setParamName (BYTE num, const char *name);
	errType setResultName(BYTE num, const char *name);
        void    setMutatorStatus(bool is_mutator=true)  {  mutator_ = is_mutator;  }
	
        WORD     getParamLength(BYTE num);
	OrtsType getParamType(BYTE num);
        WORD     getParamsQuantity();
        WORD     getAllParamsLength();
        bool     isMutator()  {  return mutator_;  }

        errType  decodeParams(rcsCmd*);
        errType  setParam(BYTE num, void* param);
        void*    getParamPtr(BYTE num);
	
        BYTE     getResultsQuantity();
        DWORD    getAllResultsLength();
        DWORD    getResultLength(BYTE i);
	OrtsType getResultType(BYTE num);
        errType  getResult(BYTE num, void** result, DWORD* length);
        errType  setResult(BYTE num, void* result);
        errType  printParams();
        errType  printResults();
	
	errType callFunction(); //+add timeout?
	
	BYTE id();
	
	void dbgPrint();
};
