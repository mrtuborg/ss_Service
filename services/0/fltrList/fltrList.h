using namespace std;

class fltrList 
{
    list<fltr*> procFilters;
    DWORD filters_size;
    
    public:
	fltrList();
	~fltrList();
	
	errType applyFilters(prcList* );
	
	errType addProcFilter(fltr*);
	errType removeProcFilter(WORD id);
	fltr* getProcFilter(WORD id);
	fltr* getProcFilterByIndex(WORD index);
	fltr* searchProcFilterByCmdline(const char*, size_t size);
	
	WORD quantity();
	DWORD getFilterListSize();
	
	errType decode (BYTE* dst_array);//, DWORD* dst_array_len);
	errType encode (BYTE* src_array, DWORD src_array_len);
	errType clear();
	void dbgPrint();
};

