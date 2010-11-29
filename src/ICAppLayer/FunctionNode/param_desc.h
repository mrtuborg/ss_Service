class param_desc
{
    void *param;
    char* param_name;
    WORD _length;
    OrtsType _type;
    
    bool _isVector;
    
    public:
        param_desc(OrtsType type, WORD len);
        param_desc(OrtsType type);
       ~param_desc();
       
        errType resize(WORD new_size);
       
       void* value();
       WORD length();
       OrtsType type();
       
       bool isVector();
       
       errType setParam(void* param_val);
       errType setName(const char* name);
       errType printParam();
       
	void dbgPrint();
	
} __attribute__ ((packed));
                  