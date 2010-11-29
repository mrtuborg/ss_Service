
class deqUdp: public udp_port
{
    


    public:
    
    deqUdp(WORD portNum, const char* ip_str="127.0.0.1"):udp_port(portNum, ip_str){};
    ~deqUdp();

    errType sendData(ssBuffer *buf);                                                                                                                                      
    errType readData(ssBuffer* addr, size_t* len, in_addr *ip_from=0, bool peek=false);                                                                                   

};