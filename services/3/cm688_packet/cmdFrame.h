#ifndef CMDFRAME_H
#define CMDFRAME_H

typedef struct cmdFrame_t {
    //********************* 1,2 BYTES ********************
    WORD header;

    //********************** 3 BYTE **********************
    BYTE null:          1;
    BYTE remoteControl: 1;

    union {
        struct {
            BYTE stop: 1;
            BYTE start: 1;
        } cmd;
        BYTE cmdValue: 2;
    } hydroStation;

    union {
        struct {
            BYTE stop:  1;
            BYTE open:  1;
            BYTE close: 1;
        } cmd;
        BYTE cmdValue: 3;
    } schield;

    BYTE hydroCyl_corr: 1;

    //********************** 4 BYTE **********************
    union {
        struct  {
            BYTE null: 1;
            boolean2Bits id[3]; // 0 - lower A, 1 - lower B, 2 - upper

        };
        BYTE cmdValue :7;
    } foldCmd;

    BYTE localControl: 1;

    //********************** 5 BYTE **********************
    union {
        struct {
            BYTE null:    1;
            BYTE reserve: 3;
            booleanBit id[3];
        } cmd;
        BYTE cmdValue: 7;
    } foldStop;

    BYTE funcCtrl: 1;

    //********************** 6 BYTE **********************
    WORD reserve:  8;

    //********************* 7,8 BYTES ********************
    WORD checkSumm;
} cmdFrame_t;


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
    void setFoldOpen(BYTE num);
    void setFoldClose(BYTE num);
    void setFoldStop(BYTE num);
    void setStartFuncControl();

    WORD setCheckSumm();

    void decode(BYTE**);
    void encode(BYTE*, size_t);
    void dbgPrint();

} __attribute__ ((packed));

#endif
