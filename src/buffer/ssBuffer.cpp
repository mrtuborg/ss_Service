#include <string.h>
#include <stdio.h>
#include <deque>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../myTypes.h"
#include "ssBuffer.h"

ssBuffer::ssBuffer()
{
}

ssBuffer::~ssBuffer()
{
    buffer.clear();
}

errType ssBuffer::pushBlock(sockaddr_in* addr, BYTE* block, DWORD len)
{
    errType result=err_not_init;
    ssBlock* dataBlock=new ssBlock;
    //printf("Wanna send to IP: %s\n",inet_ntoa(addr->sin_addr));
    //printf("to udp port: %d\n",ntohs(addr->sin_port));
    memcpy(&dataBlock->addr_in,addr,sizeof(sockaddr_in));
    dataBlock->data=new BYTE[len];
    memcpy(dataBlock->data,block,len);
    dataBlock->dataLen=len;
    buffer.push_back(dataBlock);
    //dbgPrint();
    return result;
}

DWORD ssBuffer::getFrontBlockSize()
{
    ssBlock* dataBlock;
    dataBlock=buffer.front();
    return dataBlock->dataLen;
}

errType ssBuffer::popBlock(sockaddr_in* addr, BYTE* block)
{
    errType result=err_not_init;
    DWORD len=0;
    ssBlock* dataBlock;
    dataBlock=buffer.front();
    buffer.pop_front();
    memcpy(addr,&dataBlock->addr_in,sizeof(sockaddr_in));
    len=dataBlock->dataLen;
    memcpy(block, dataBlock->data, len);
    
    return result;
}

errType ssBuffer::getBlockPtrAt(int index, ssBlock* block)
{
    errType result=err_not_init;
    
    block=buffer.at(index);
    
    return result;
}

DWORD ssBuffer::size()
{
    //if (buffer!=0) 
    return buffer.size();
    //else return 0;
}

void ssBuffer::dbgPrint()
{
    ssBlock* dataBlock;
    int i=1, k=0;
    DWORD len=0, q=0;
    
    q=size();
    printf("Blocks quantity=%lu\n", q);
    while (i<=q) {
	dataBlock=buffer[i-1];
	len=dataBlock->dataLen;
	printf("Block #%d\n",i);
	printf("IP address: %s\n",inet_ntoa(dataBlock->addr_in.sin_addr));
	printf("UDP port: %d\n",ntohs(dataBlock->addr_in.sin_port));
	printf("Data length: %lu\n", len);
	printf("Data :[");
	for (k=0; k<len; k++) printf("%.2X ",dataBlock->data[k]);
	printf("]\n");
	i++;
    } ;
    printf("------------\n");
}

