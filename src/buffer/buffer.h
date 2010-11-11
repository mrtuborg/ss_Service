class buffer {
	BYTE* buff;
	DWORD WrRef, RdRef, buffSize;
	
	bool writingLock;

	public:
	buffer(DWORD size);
	~buffer();
	
	errType write(BYTE* addr, DWORD len);
	WORD read (BYTE* addr, DWORD len=0);
	DWORD length();
	
	BYTE* lockBufferChunkForExternWriting();
	errType unlockBufferChunkForExternWriting(DWORD offset);
	errType removeBufferChunk(DWORD backward_offset, DWORD len);
	errType copyBufferChunkTo(BYTE* addr, DWORD offset=0, DWORD len=0);
	//errType copyBufferChunkTo(BYTE* addr, WORD offset=0, BYTE len=0);
	
	errType dbgPrint();
};
