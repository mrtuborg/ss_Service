using namespace std;

class trigList
{
    list<valueTrig*> valueTriggers;
    DWORD triggersQuantity;
    
    public:
	trigList();
	~trigList();
	
	//errType (prcList* );
	
	errType addValueTrigger(valueTrig*);
	errType removeValueTrigger(WORD id);
	valueTrig* getValueTrigger(WORD id);
	valueTrig* searchValueTriggerByValuePtr(const void*);
	
	errType triggersClock(WORD index, OrtsType currType, const void* currValue);

	WORD quantity();
	DWORD getFilterListSize();
	
	errType decode (BYTE* dst_array);//, DWORD* dst_array_len);
	errType encode (BYTE* src_array, DWORD src_array_len);
	errType clear();
	void dbgPrint();
};

