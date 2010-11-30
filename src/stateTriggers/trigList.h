using namespace std;

class triggerList
{
    list<valueTrig*> valueTriggers;
    DWORD triggersQuantity;
    
    public:
	triggerList();
	~triggerList();
	
	//errType (prcList* );
	
	errType addValueTrigger(valueTrig*);
	errType removeValueTrigger(WORD id);
	valueTrig* getValueTrigger(WORD id);
	valueTrig* searchValueTriggerByValuePtr(const void*);
	
	WORD quantity();
	DWORD getFilterListSize();
	
	errType decode (BYTE* dst_array);//, DWORD* dst_array_len);
	errType encode (BYTE* src_array, DWORD src_array_len);
	errType clear();
	void dbgPrint();
};

