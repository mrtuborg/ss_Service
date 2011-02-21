typedef struct SASC_msg_type
{
uint32_t typeinf;
uint32_t init;
uint32_t binning;
uint32_t Niter;
uint32_t time;
float FiH;
float FiA;
float TC;
float Wm_c;
float Wa;
float X;
float Y;
float Z;
float Fix;
float Fiy;

} SASC_msg_type;

typedef enum { //message command modifications
    _power_on,
    _power_off,
    _measure,
    _zero_measure,
    _link_test,
    _get_db,
    _get_next_record,
    _erase_db,
    _tare_start,
    _get_tare_value,
    _tare_stop
} SASC_cmd_mod;

typedef enum { //message answers modifications
    _ready=0,
    _off=1,
    _measure_ready=2,
    _zero_ready=3,
    _link_norm=4,
    _db_record=55,
    _db_last_record=50,
    _db_empty=7,
    _tare_on=8,
    _tare_off=9,
    _tare_params=10,
    _busy=11,
    _awaiting=12
} SASC_answer_mod;

extern char SASC_answer_str[13][255];
             

class comm_SASC 
{
    static const WORD kSASCMsgSize;
    SASC_msg_type *msg;

public:
    comm_SASC();
    ~comm_SASC();

    errType clear_msg();
    errType apply_mod(SASC_cmd_mod mode, BYTE** params=0);
    errType checkAnswer(SASC_answer_mod *typeinf);

    errType decode(BYTE** );
    errType encode(BYTE*, DWORD size);

    void dbgPrint();
};
