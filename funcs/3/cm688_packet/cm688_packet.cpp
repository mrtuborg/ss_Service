#include <netinet/in.h>

#include "../myTypes.h"
#include "global.h"
#include "cmdFrame.h"
#include "statusFrame.h"
#include "cm688_packet.h"

cm688_packet::cm688_packet(bool isCmdPacket)
{
    if (isCmdPacket) frame=(BYTE*)new cmdFrame;
    else frame=(BYTE*)new statusFrame;
    
    packetType=isCmdPacket;
}

cm688_packet::~cm688_packet()
{
    delete frame;
}

statusFrame* cm688_packet::asStatusFrame()
{
    return (statusFrame*)frame;
}

cmdFrame* cm688_packet::asCmdFrame()
{
    return (cmdFrame*)frame;
}

errType cm688_packet::decode(BYTE* array) //fromArray
{
    errType result=err_not_init;
    frame=array;
    return result;
}

BYTE* cm688_packet::encode() //toArray
{
    return frame;
}
