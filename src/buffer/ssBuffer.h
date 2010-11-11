using namespace std;

typedef struct ssBlock
{
	sockaddr_in addr_in;
	DWORD dataLen;
	BYTE* data;

} ssBlock;

class ssBuffer
{
    deque <ssBlock*> buffer;
    
    public:
    
     ssBuffer();
    ~ssBuffer();
    
    errType pushBlock(sockaddr_in* , BYTE*, DWORD len);
    errType popBlock(sockaddr_in* , BYTE*);
    
    errType getBlockPtrAt(int index, ssBlock* block);
    DWORD getFrontBlockSize();
    
    DWORD size();
    
    void dbgPrint();
};
