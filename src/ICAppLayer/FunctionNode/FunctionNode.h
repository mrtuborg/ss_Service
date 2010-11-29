
class FunctionNode
{
    BYTE  func_id;
    const char* func_name;
    WORD  func_paramsQuantity;
    WORD  func_resultsQuantity;
    
    WORD  func_paramsLength;
    WORD  func_resultsLength;
    
    param_desc *func_params[32];
    param_desc *func_results[32]; 
    
    funcPtr func;//function pointer
    
    public:
	FunctionNode(BYTE id, WORD parQnt, WORD resQnt, funcPtr ptr); //+function pointer
	~FunctionNode();
	
	errType setFuncName (const char *name);
	errType setParamDescriptor (BYTE num, OrtsType type);
	errType setResultDescriptor(BYTE num, OrtsType type);
	errType setParamName (BYTE num, const char *name);
	errType setResultName(BYTE num, const char *name);
	
	WORD getParamLength(BYTE num);
	OrtsType getParamType(BYTE num);
	WORD getParamsQuantity();
	WORD getAllParamsLength();
	errType decodeParams(rcsCmd*);
	errType setParam(BYTE num, void* param);
	void* getParamPtr(BYTE num);
	
	BYTE getResultsQuantity();
	DWORD getAllResultsLength();
	DWORD getResultLength(BYTE i);
	OrtsType getResultType(BYTE num);
	errType getResult(BYTE num, void** result, DWORD* length);
	errType setResult(BYTE num, void* result);
	errType printParams();
	errType printResults();
	
	errType callFunction(); //+add timeout?
	
	BYTE id();
	
	void dbgPrint();
};
