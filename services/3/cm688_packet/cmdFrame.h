#ifndef CMDFRAME_H
#define CMDFRAME_H

typedef struct cmdFrame_t {
    //********************* 1,2 BYTES ********************
    WORD header;

    //********************** 3 BYTE **********************
    union {
        struct  {
            BYTE null               :1;
            BYTE remoteControl      :1;
            BYTE hydroStation_stop  :1;
            BYTE hydroStation_start :1;
            BYTE shield_stop        :1;
            BYTE shield_open        :1;
            BYTE shield_close       :1;
            BYTE hydroCyl_corr      :1;
        };
        BYTE cmdValue;
    };

    //********************** 4 BYTE **********************
    union {
        struct  {
            BYTE null            :1;
            BYTE fold_lowA_open  :1;
            BYTE fold_lowA_close :1;
            BYTE fold_lowB_open  :1;
            BYTE fold_lowB_close :1;
            BYTE fold_up_open    :1;
            BYTE fold_up_close   :1;
            BYTE localControl    :1;
        } fold_status;
        BYTE cmdValue;
    }BYTE_4;


    //********************** 5 BYTE **********************
    union {
        struct {
            BYTE null           :1;
            BYTE reserve        :3;
            BYTE fold_lowA_stop :1;
            BYTE fold_lowB_stop :1;
            BYTE fold_up_stop   :1;
            BYTE funcCtrl       :1;
        };
        BYTE cmdValue;
    }BYTE_5;


    //********************** 6 BYTE **********************
    BYTE reserve;

    //********************* 7,8 BYTES ********************
    WORD checkSumm;

} __attribute__((packed)) cmdFrame_t;


class cmdFrame {

    cmdFrame_t frame;

public:
    cmdFrame();
    ~cmdFrame();
    void setHydroCyl_correct();
    void setHydroStationStart();
    void setHydroStationStop();
    void setSchieldClose();
    void setSchieldOpen();
    void setSchieldStop();
    void setCPControl();
    void setRemoteControl();
    void setLocalControl();
    void setFoldOpen(FoldDscr_type fold_descriptor);
    void setFoldClose(FoldDscr_type fold_descriptor);
    void setFoldStop(FoldDscr_type fold_descriptor);
    void setStartFuncControl();

    WORD setCheckSumm();

    void decode(BYTE**);
    void encode(BYTE*, size_t);
    void dbgPrint();

} __attribute__ ((packed));

#endif
