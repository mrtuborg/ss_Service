#ifndef STATUSFRAME_H
#define STATUSFRAME_H

typedef struct statusFrame_t {

    //********************** 3 BYTE **********************
    union {

        struct  {
            BYTE nullBit          :1;
            BYTE remoteControl    :1;
            BYTE localControl     :1;
            BYTE hydrosys_poweron :1;
            BYTE hydrosys_ready   :1;
            BYTE hydrosys_norm    :1;
            BYTE hydrosys_alert   :1;
            BYTE shieldStopped    :1;
        };
        BYTE bValue;
    } BYTE_3;

    //********************** 4 BYTE **********************
    union {
        struct {
            BYTE nullBit                :1;
            BYTE link_KEGP              :1;
            BYTE link_localPanel        :1;
            BYTE link_AUGS              :1;
            BYTE link_BUZ               :1;
            BYTE hydrosys_temp_norm     :1;
            BYTE hydrosys_level_norm    :1;
            BYTE hydrosys_pressure_norm :1;
        };
        BYTE bValue;
    } BYTE_4;

    //********************** 5 BYTE **********************
    union{
        struct {
            BYTE nullBit: 1;
            BYTE fold_lowA_open        :1;
            BYTE fold_lowA_close       :1;
            BYTE fold_lowB_open        :1;
            BYTE fold_lowB_close       :1;
            BYTE fold_up_open          :1;
            BYTE fold_up_close         :1;
            BYTE hydrosys_filters_norm :1;
        };
        BYTE bValue;
    } BYTE_5;

    //********************** 6 BYTE **********************
    union {
        struct {
            BYTE nullBit          :1;
            BYTE UZ1_locked       :1;
            BYTE UZ2_locked       :1;
            BYTE UZ3_locked       :1;
            BYTE remotePanel_busy :1;
            BYTE linkingStatus    :1;
            BYTE funcControl_busy :1;
            BYTE schieldOpened    :1;
        };
        BYTE bValue;
    }BYTE_6;

    //********************** 7 BYTE **********************
    union {
        struct {
            BYTE nullBit      :1;
            BYTE UZ1_busy     :1;
            BYTE UZ2_busy     :1;
            BYTE UZ3_busy     :1;
            BYTE UZ1_unlocked :1;
            BYTE UZ2_unlocked :1;
            BYTE UZ3_unlocked :1;
            BYTE shieldClosed :1;
        };
        BYTE bValue;
    }BYTE_7;


    //********************** 8 BYTE **********************
    union {
        struct {
            BYTE nullBit    :1;
            BYTE UZ1_alert  :1;
            BYTE UZ2_alert  :1;
            BYTE UZ3_alert  :1;
            BYTE UZ1_manual :1;
            BYTE UZ2_manual :1;
            BYTE UZ3_manual :1;
            BYTE reserve    :1;
        };
        BYTE bValue;
    } BYTE_8;

    //********************** 9 BYTE **********************
    union {
        struct {
            BYTE nullBit          :1;
            BYTE fold_up_inv_GC   :1;
            BYTE fold_lowB_inv_GC :1;
            BYTE fold_lowA_inv_GC :1;
            BYTE reseting         :1;
            BYTE fold_lowA_stop   :1;
            BYTE fold_lowB_stop   :1;
            BYTE fold_up_stop     :1;
        };
        BYTE bValue;
    } BYTE_9;

    //********************** 10 BYTE **********************
    union {
        struct {
            BYTE nullBit          :1;
            BYTE fold_up_deg_b0   :1;
            BYTE fold_lowB_deg_b0 :1;
            BYTE fold_lowA_deg_b0 :1;
            BYTE reserve          :1;
            BYTE fold_up_deg_b8   :1;
            BYTE fold_lowB_deg_b8 :1;
            BYTE fold_lowA_deg_b8 :1;
        };
        BYTE bValue;
    } BYTE_10;

    //********************** 11 BYTE **********************
    BYTE _A_nullbit:  1;
    BYTE foldA_value: 7;

    //********************** 12 BYTE **********************
    BYTE _B_nullbit:  1;
    BYTE foldB_value: 7;

    //********************** 13 BYTE **********************
    BYTE _U_nullbit:  1;
    BYTE foldU_value: 7;

    //********************** 14 BYTE **********************
    union {
        struct {
            BYTE nullBit: 1;
            BYTE fold_lowA_psa_right :1;
            BYTE fold_lowA_psa_left  :1;
            BYTE fold_lowB_psa_right :1;
            BYTE fold_lowB_psa_left  :1;
            BYTE fold_up_psa_right   :1;
            BYTE fold_up_psa_left    :1;
            BYTE reserve: 1;
        };
        BYTE bValue;
    } BYTE_14;

    //********************** 15 BYTE **********************
    union {
        struct {
            BYTE nullBit :1;
            BYTE fold_lowA_esa_right :1;
            BYTE fold_lowA_esa_left  :1;
            BYTE fold_lowB_esa_right :1;
            BYTE fold_lowB_esa_left  :1;
            BYTE fold_up_esa_right   :1;
            BYTE fold_up_esa_left    :1;
            BYTE reserve             :1;
        } ;
        BYTE bValue;
    } BYTE_15;

    //********************** 16 BYTE **********************
    BYTE reserve;
    //******************** 17,18 BYTES ********************
    WORD checkSumm;

} statusFrame_t;


class statusFrame{
    statusFrame_t frame;
public:
    statusFrame();
    ~statusFrame();

    BYTE getShieldState(BYTE num);// + opened
    BYTE getHydroSystemStatus();
    BYTE getHydroSystemValues(BYTE num); // 0 - pressure, 1 - oil_level, 2 - temp, 3 - filters
    bool isLocalControl();
    bool isRemoteControl();
    bool isCPControl();
    BYTE getSystemLinkStatus(BYTE);
    BYTE getFoldState(FoldDscr_type fold_descriptor);

    bool getFuncControlStatus();
    bool getLinkingStatus();
    bool getRemotePanelStatus();

    BYTE isUZ_locked(FoldDscr_type fold_descriptor);
    BYTE isUZ_unlocked(FoldDscr_type fold_descriptor);
    BYTE isUZ_busy(FoldDscr_type fold_descriptor);
    BYTE isUZ_manual(FoldDscr_type fold_descriptor);
    BYTE isUZ_alert(FoldDscr_type fold_descriptor);

    WORD getFoldPosition(FoldDscr_type fold_descriptor);
    BYTE get_psa_sensors(FoldDscr_type fold_descriptor);
    BYTE get_esa_sensors(FoldDscr_type fold_descriptor);

    bool testCheckSumm();

    void decode(BYTE*);
    void encode(BYTE*, size_t);
    void dbgPrint();

} __attribute__ ((packed));

#endif
