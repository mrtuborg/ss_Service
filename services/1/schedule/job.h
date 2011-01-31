typedef struct job_type{
        DWORD objId;
        DWORD nextObjId;
        WORD timeStart;
        WORD timeEnd;
        BYTE service_id;
} __attribute__((packed)) job_type;

class job{
      job_type* jobReference;
        rcsCmd* jobEntity;
       cronTab* cronJob;

	public:
	 job(DWORD id);
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);

	errType set_dwNextJobID(DWORD id);
	errType set_wStartTime(WORD time);
	errType set_wFinishTime(WORD time);
	errType set_btServiceId(BYTE id);
	errType	setJobCmd(BYTE func_id, DWORD param_len, void* args);


	WORD getLength();
	
	DWORD get_dwObjId();
	DWORD get_dwNextObjId();
	WORD get_wTimeStart();
	WORD get_wTimeEnd();
	BYTE get_btServId();
	BYTE get_btFuncId();
	const void* get_paramsPtr(DWORD offset=0);
	DWORD get_paramsLength();
	errType writeCronTab();

	rcsCmd* cmd();
	
	void dbgPrint();
};
