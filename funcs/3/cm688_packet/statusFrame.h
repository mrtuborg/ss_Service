#ifndef STATUSFRAME_H
#define STATUSFRAME_H

typedef struct statusFrame_t {
    	BYTE  schieldStopped :1;
	
	union {
	    struct {
		BYTE alert:1;
		BYTE norm: 1;
		BYTE ready: 1;
		BYTE poweron: 1;
	    } fields;
	            
	    BYTE bValue: 4;
	} hydroSystem_status;
	
	union {
	    struct {
	    BYTE localControl: 1;
	    BYTE remoteControl: 1;
	    BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue: 3;
	} controlSwitch;

	union {
	    struct {
		BYTE pressure_norm: 1;
		BYTE level_norm: 1;
		BYTE temp_norm: 1;
	    } fields;
	    
	    BYTE bValue:3;
	} hydroSystem_values;

	union {
	    struct {
		BYTE BUZ_link: 1;
		BYTE AUGS_link: 1;
		BYTE localPanel_link: 1;
		BYTE KEGP_link: 1;
		BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue:5;
	} systemsLink_status;

	booleanBit hydroSystemFilters_norm;
	
	union{
	    struct {
		boolean2Bits id[3]; // bit0 - open, bit1 -close
		BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue:7;
	} foldStatus;
	
	BYTE schieldOpened: 1;
	BYTE funcControl_busy: 1;
	BYTE linkingStatus: 1;
	BYTE remotePanel_busy: 1;

	union {
	    struct {
		booleanBit UZ[3];
		BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue: 4;
	}UZ_locked;
	
	BYTE schieldClosed: 1;

	union {
	    struct {
		booleanBit UZ[3];
	    } fields;
	    
	    BYTE bValue: 3;
	}UZ_unlocked;
	
	union {
	    struct {
		booleanBit UZ[3];
		BYTE nullBit: 1;
		BYTE reserve: 1;
	    } fields;
	    
	    BYTE bValue: 5;
	}UZ_busy;
	
	union {
	    struct {
		booleanBit UZ[3];
	    } fields;
	    
	    BYTE bValue: 3;
	} UZ_manual;

        union {
	    struct {
		booleanBit UZ[3];
		BYTE nullBit: 1;
		BYTE reserve: 3;
	    } fields;
	    
	    BYTE bValue: 7;
	} UZ_alert;
	
        union {
	    struct {
		BYTE reseting: 1;
		booleanBit invalid_fold[3];
		BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue: 5;
	} hydroCyl_setting;
	
	union {
	    struct {
		booleanBit fold[3];
		BYTE reserve: 1;
	    } fields;
	    
	    BYTE bValue:4;
	} fold_val_p8;
	
	union {
	    struct {
		booleanBit fold[3];
		BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue:4;
	} fold_val_p0;

	BYTE foldA_value: 7;
	BYTE _A_nullbit: 1;

	BYTE foldB_value: 7;
	BYTE _B_nullbit: 1;

	BYTE foldU_value: 7;
	BYTE _U_nullbit: 1;

	union {
	    struct {
	      BYTE reserve: 1;
	      boolean2Bits fold[3];
	      BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue;
	    
	} psa_sensor;
	
	union {
	    struct {
	      BYTE reserve: 1;
	      boolean2Bits fold[3];
	      BYTE nullBit: 1;
	    } fields;
	    
	    BYTE bValue;
	} esa_sensor;

	BYTE reserve;
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
	BYTE getFoldState(BYTE num);
	
	bool getFuncControlStatus();
	bool getLinkingStatus();
        bool getRemotePanelStatus();
        
	BYTE isUZ_locked(BYTE num);
	BYTE isUZ_unlocked(BYTE num);
	BYTE isUZ_busy(BYTE num);
	BYTE isUZ_manual(BYTE num);
	BYTE isUZ_alert(BYTE num);
        
        BYTE getHydroCylStatus(BYTE num); // setting, resetting
        WORD getFoldPosition(BYTE num);
        BYTE get_psa_sensors(BYTE num);
        BYTE get_esa_sensors(BYTE num);
        
        bool testCheckSumm();
        
        void decode(BYTE**);                                                
        void encode(BYTE*, size_t);                                         
        void dbgPrint();
        
} __attribute__ ((packed));

#endif
