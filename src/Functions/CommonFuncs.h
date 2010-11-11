extern void* EquipListenPolling(void*);

class CommonFuncs
{
    ICAppLayer* appLayer;
    
    public:
	CommonFuncs(ICAppLayer *appl);
	~CommonFuncs();
	
	errType StartCommonFuncs();
	errType StopCommonFuncs();
};
