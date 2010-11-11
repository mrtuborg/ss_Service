class job{
    DWORD time_ms;
    BYTE service_id;
    cmd action;
    
    public:
	job();
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);
	WORD getLength();
	
	DWORD get_dwTime();
	BYTE* get_strTime();
	BYTE get_btServId();
	BYTE get_btFuncId();
	WORD get_paramsLen();
	BYTE* get_paramsPtr();
	
	void dbgPrint();
};
