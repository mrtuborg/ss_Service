#include <arpa/inet.h>
#include <deque>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "../buffer/ssBuffer.h"
#include "../../../udp_port/udp_port.h"
#include "deqUdp.h"


//deqUdp::deqUdp(WORD portNum, const char* ip_str):udp_port(portNum, ip_str)
//{
//
//}

deqUdp::~deqUdp()
{

}

errType deqUdp::sendData(ssBuffer *buf)                                                                                                                                 
{                                                                                                                                                                           
//    if (verbose_level) printf("udp_port::sendData()\n");                                                                                                                    
    int flags=0;                                                                                                                                                            
    errType result=err_result_ok;                                                                                                                                           
    sockaddr_in host_ip;                                                                                                                                                    
    WORD sock=get_sock();
    BYTE *sendData;                                                                                                                                                         
    DWORD len=buf->getFrontBlockSize();                                                                                                                                     
    memset(&host_ip,0,sizeof(host_ip));                                                                                                                                     
    sendData=new BYTE[len];                                                                                                                                                 
    buf->popBlock(&host_ip, sendData);                                                                                                                                      
    printf("Отправка пакета: содержимое [");                                                                                                                                
    for (size_t i=0; i<len; i++) printf("%.2X ", sendData[i]);                                                                                                              
    printf("]\n\t\t\t адресат: %s, сокет %d/udp\n\n",inet_ntoa(host_ip.sin_addr),ntohs(host_ip.sin_port));                                                                  
    ssize_t sended=0;                                                                                                                                                       
    sended=sendto(sock, sendData, len, flags,(sockaddr*) &host_ip, sizeof(host_ip));                                                                                        
//    if (verbose_level) printf("data length=%d want to be sent via udp port %d, sended=%d\n",(int)len,ntohs(ip_addr.sin_port),(int)sended);                                  
    if (sended==-1)                                                                                                                                                         
    {                                                                                                                                                                       
//	if (verbose_level) printf("errno=%d\n",errno);                                                                                                                      
	    result=err_result_error;                                                                                                                                            
    }                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    delete sendData;                                                                                                                                                    
    return result;                                                                                                                                                          
}


errType deqUdp::readData(ssBuffer* addr, size_t* len, in_addr *ipaddr,bool peek)
{
	//    if (verbose_level) printf("udp_port::readData(buffer)\n");
    errType result=err_result_ok;
    int numbytes=0, i=0;
    socklen_t socklen=0;
    sockaddr_in sfrom;//=new sockaddr_in;
    bool repeat=false;
    WORD sock=get_sock();
    
    
    BYTE* tmp_rcvData=new BYTE[*len];
    BYTE* rcvData;
        
    socklen=sizeof(sockaddr_in);
    memset(&sfrom,0,sizeof(sockaddr_in));
    sfrom.sin_family=AF_INET;
    
    
//    if (verbose_level) printf("listening %lu bytes on %d port\n",*len,ntohs(ip_addr.sin_port));
    do {
		if (peek) numbytes = recvfrom(sock, tmp_rcvData, *len , MSG_PEEK,(struct sockaddr *)&sfrom, &socklen);
		else numbytes = recvfrom(sock, tmp_rcvData, *len , 0 ,(struct sockaddr *)&sfrom, &socklen);
		
		
		//printf("packet from %s:%d\n",inet_ntoa(sfrom.sin_addr),ntohs(sfrom.sin_port));
		if (((sfrom.sin_addr.s_addr)&0xFF000000)==0) repeat=true;
    } while (repeat); 
    
    if ( numbytes == -1) {
        perror("recvfrom");
        result=err_result_error;
		
    } else if (numbytes>0) {
		rcvData=new BYTE[numbytes];
		memcpy(rcvData, tmp_rcvData, numbytes);
		
		*len=numbytes;
		
		delete tmp_rcvData;
		
		addr->pushBlock(&sfrom, rcvData, numbytes);
//		if (verbose_level) addr->dbgPrint();
//		
    }
    return result;
}
