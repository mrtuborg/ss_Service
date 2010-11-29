#include <string.h>
#include <stdio.h>
#include "../../../ortsTypes/ortsTypes.h"
#include "buffer.h"
buffer::buffer(DWORD size)
{
    buff=new BYTE[size];
    WrRef=0;
    RdRef=0;
    buffSize=size;
    writingLock=false;
}

buffer::~buffer()
{
    WrRef=0;
    RdRef=0;
    writingLock=false;
    delete buff;
}

DWORD buffer::length()
{
//    printf("WrRef=%d\n",WrRef);
//    printf("RdRef=%d\n",RdRef);
    return WrRef-RdRef;
}

errType buffer::write(BYTE* addr, DWORD len)
{
    errType result=err_result_ok;
    if (WrRef+len>buffSize)
	{
	    result=err_result_error;
	    len=buffSize-len;
	}	
	WrRef+=len;
	memcpy(buff+WrRef-len,addr,len);
	
    return result;
}

DWORD buffer::read (BYTE* addr, DWORD len)
{
    DWORD length=WrRef-RdRef; //errType result=err_result_ok;
    //printf("buffer::read()\n");
    //printf("RdRef=%lu\n",RdRef);
    //printf("WrRef=%lu\n",WrRef);
    //printf("len=%lu\n",len);
    if (RdRef+len>WrRef)
    {
	len=WrRef-RdRef;
	//result=err_result_error;
    }
    
    if (len==0)
    {
	len = WrRef-RdRef;
    }
    
    memcpy(addr,buff+RdRef,len);
    RdRef+=len;

    //for (int i=0; i<len; i++) printf("%.2X ",addr[i]);
    //printf("\n");
    if ((!writingLock)&&(WrRef==RdRef)) {
//	printf("flush!\n");
	WrRef=RdRef=0;
    }
    
    return length;
}

BYTE* buffer::lockBufferChunkForExternWriting()
{
     writingLock=true;
     return buff+WrRef;
}

errType buffer::unlockBufferChunkForExternWriting(DWORD offset)
{
    errType result=err_result_ok;
    if ((WrRef+offset)>buffSize) {
	printf("unlockBufferChunkForExternWriting): (WrRef+offset)>buffSize\n");
	printf("WrRef=%d, RdRef=%d, offset=%d, buffSize=%d\n", WrRef, RdRef, offset, buffSize);
	offset=WrRef-offset;
	result=err_result_error;
    }
    writingLock=false;
    WrRef+=offset;
    
    return result;
}

errType buffer::removeBufferChunk(DWORD backward_offset, DWORD len)
{
    errType result=err_result_ok;
    
    if ((WrRef-backward_offset)<RdRef) result=err_result_error;
    else {
	memcpy(buff+(WrRef-backward_offset),buff+(WrRef-backward_offset)+len,len);
	WrRef=WrRef-backward_offset;
    }
    
    return result;
}

errType buffer::copyBufferChunkTo(BYTE* addr, DWORD offset, DWORD len)
{
    errType result=err_result_ok;
    printf("copyBufferChunkTo\n");
    if (len==0) len=WrRef-RdRef;
    if ((RdRef+offset)>WrRef) result=err_result_error;
    else {
	memcpy(addr,buff+RdRef+offset,len);
    }
    return result;
}

/*errType buffer::copyBufferChunkTo(BYTE* addr, WORD offset, BYTE len)
{
    errType result=err_result_ok;
    
    if (len==0) len=WrRef-RdRef;
    if ((RdRef+offset)>WrRef) result=err_result_error;
    else {
	memcpy(addr,buff+RdRef+offset,len);
    }
    return result;
}*/

errType buffer::dbgPrint()
{
    for(int i=RdRef;i<WrRef;i++) printf("%.2X ",buff[i]);
    return err_result_ok;
}
