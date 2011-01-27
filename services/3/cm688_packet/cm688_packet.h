#ifndef CM688_PACKET_H
#define CM688_PACKET_H

typedef union {
    BYTE bValue[2];
    WORD wValue;
} __attribute__ ((packed)) cm688_hdr;

typedef union {
    BYTE bValue_low;
    BYTE bValue_high;
    WORD wValue;
} __attribute__ ((packed)) cm688_chk;


class cm688_packet {

public:
    cm688_packet(bool isCmdPacket=false);
    ~cm688_packet();

    statusFrame* asStatusFrame();
    cmdFrame* asCmdFrame();

    errType decode(BYTE*);// FromArray;
    BYTE* encode();// toArray;
private:
    cm688_hdr  header;
    BYTE *frame;
    cm688_chk  chkSumm;

    bool  packetType; // 0-statusFrame, 1-cmdFrame
} __attribute__ ((packed));

#endif
