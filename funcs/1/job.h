typedef struct job_type{
        DWORD objId;
        WORD timeStart;
        WORD timeEnd;
        BYTE service_id;
        rcsCmd_type* cmd;
} __attribute__((packed)) job_type;

class job{
      job_type* jobEntity;
    
    public:
	job();
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);
	WORD getLength();
	
	DWORD get_dwObjId();
	WORD get_wTimeStart();
	WORD get_wTimeEnd();
	BYTE get_btServId();
	rcsCmd_type *cmd();
	
	void dbgPrint();
};
