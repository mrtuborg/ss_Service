#ifndef CMDFRAME_H
#define CMDFRAME_H

typedef struct cmdFrame_t {
	WORD header;
	
	BYTE hydroCyl_corr: 1;
	union {
	    struct {
		BYTE close:1;
		BYTE open: 1;
		BYTE stop: 1;
	    } cmd;
	    BYTE cmdValue: 3;
	} schield;

	union {
	    struct {
		BYTE start: 1;
		BYTE stop: 1;
	    } cmd;
	    BYTE cmdValue: 2;
	} hydroStation;

	union {
	    struct {
		BYTE remoteControl: 1;
		BYTE null:1;
		BYTE localControl: 1;
	    }  cmd;
	    BYTE cmdValue: 3;
	} ctrlSwitch;

	union {
	    boolean2Bits id[3]; // 0 - upper, 1 - B, 2 - 0
	    BYTE cmdValue :6;
	} foldCmd;

	
	union {
	    struct {
		BYTE null: 1;
		booleanBit id[3];
	    } cmd;
		BYTE cmdValue: 4;
	} foldStop;
	
	BYTE funcCtrl: 1;
	WORD reserve: 12;
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
