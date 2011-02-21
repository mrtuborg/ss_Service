typedef enum {
    _mode_off = 0x01,
    _mode_prg = 0x02,
    _mode_KN = 0x03,
    _mode_srchStart = 0x04,
    _mode_srchStop = 0x05,
    _mode_ZVV = 0x06,
    _corr_AZ = 0x07,
    _corr_EL = 0x08,
    _corr_TM = 0x09
} ps_cmd_id_type;

typedef struct {
    ps_cmd_id_type cmd_id;
    WORD len;
    BYTE *params;
} ps_cmdframe_type;

class PS_CmdFrame {
    ps_cmdframe_type* msg;
    
    public:
    PS_CmdFrame();
    ~PS_CmdFrame();
    
    errType decode(BYTE** );                                                                                        
    //errType encode(BYTE*, DWORD size);
    
    void dbgPrint();
    
    errType prepParams_prg_mode(char* filename, int len, BYTE *prepparams);
    errType prepParams_srch_start(BYTE counts, BYTE step, BYTE stayTime, BYTE *prepparams);
    errType prepParams_new_AZ_corr(int16_t grad, BYTE min, BYTE sec, BYTE *prepparams);
    errType prepParams_new_EL_corr(int16_t grad, BYTE min, BYTE sec, BYTE *prepparams);
    errType prepParams_new_TM_corr(int sign, int16_t tm_sec, uint16_t tm_msec, BYTE *prepparams);
    errType prepMsg(ps_cmd_id_type cmd, BYTE* params=0);

    int lenValue();
};
