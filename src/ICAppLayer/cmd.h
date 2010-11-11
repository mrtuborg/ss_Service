#pragma pack (1)

class cmd {
    BYTE func_id;
    DWORD func_paramsLength;
    BYTE *func_params;
    WORD crc16_signature;

public:
 cmd();
 ~cmd();

 BYTE get_func_id();
 DWORD get_func_paramsLength();
 DWORD getCmdLength();
 WORD get_crc_sign();
 BYTE* get_func_paramsPtr(DWORD offset=0);
 
 
 void dbgPrint();
 errType decode(BYTE* dataBlock);
 errType encode(BYTE* dataBlock);
 errType encode(BYTE, DWORD, BYTE*);
 errType makeSign();

 bool checkSign();
} __attribute__ ((packed));
