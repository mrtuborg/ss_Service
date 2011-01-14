typedef enum {
    _mode_off,
    _mode_prg,
    _mode_KN,
    _mode_srchStart,
    _mode_srchStop,
    _mode_ZVV,
    _corr_AZ,
    _corr_EL,
    _corr_TM
} ps_cmd_id_type;

typedef struct {
    ps_cmd_id_type cmd_id;
    BYTE *params;
    WORD len;
} ps_cmdframe_type;

class PS_CmdFrame {
    ps_cmdframe_type* msg;
    
    public:
    PS_CmdFrame();
    ~PS_CmdFrame();
    
    errType decode(BYTE** );                                                                                        
    errType encode(BYTE*, DWORD size);
    
    void dbgPrint();
    
    errType prepParams_prg_mode(char* filename, BYTE *prepparams);     
    errType prepParams_srch_start(BYTE counts, BYTE step, BYTE stayTime, BYTE *prepparams);
    errType prepParams_new_AZ_corr(int16_t grad, BYTE min, BYTE sec, BYTE *prepparams);
    errType prepParams_new_EL_corr(BYTE grad, BYTE min, BYTE sec, BYTE *prepparams);
    errType prepParams_new_TM_corr(int16_t sec, uint16_t msec, BYTE *prepparams);
    errType prepMsg(ps_cmd_id_type cmd, BYTE* params);
};