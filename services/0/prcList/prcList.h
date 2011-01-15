using namespace std;

class prcList 
{
    list<prcTask*> processes;
    DWORD procs_size;
    
    public:
	prcList();
	~prcList();
	
	errType scanUserProcesses();
	
	errType addUserProcess(prcTask*);
	errType removeUserProcess(long pid);
	prcTask* getUserProcess(long pid);
	prcTask* getUserProcessByIndex(WORD index);
	prcTask* searchUserProcessByCmdline(const char*, size_t size);
	
	WORD quantity();
	DWORD getPrcListSize();
	
	errType decode (BYTE* dst_array);//, DWORD* dst_array_len);
	errType encode (BYTE* src_array, DWORD src_array_len);
	errType clear();
	void dbgPrint();
};

