class param_desc
{
    void *param;
    char* param_name;
    WORD _length;
    MyType _type;
    
    bool _isVector;
    
    public:
        param_desc(MyType type, WORD len);
        param_desc(MyType type);
       ~param_desc();
       
        errType resize(WORD new_size);
       
       void* value();
       WORD length();
       MyType type();
       
       bool isVector();
       
       errType setParam(void* param_val);
       errType setName(const char* name);
       errType printParam();
       
	void dbgPrint();
} __attribute__ ((packed));
