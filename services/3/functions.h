// Covering Control Service
//---------------------------
extern errType srvInit();
extern errType srvDeinit();

errType equipListenProcessing(BYTE *writingBuffer, size_t *sz);
extern void* pollingThreadFunc(void*);

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType shieldPowerON(void*);
extern errType shieldPowerOFF(void*);
extern errType hydroSystemPowerON(void*);
extern errType hydroSystemPowerOFF(void*);

extern errType hydroSystemGetParams(void*);
extern errType foldOpen(void*);
extern errType foldClose(void*);
extern errType foldStop(void*);

extern errType shieldOpen(void*);
extern errType shieldClose(void*);
extern errType shieldStop(void*);

extern errType foldGetParams(void*);
extern errType semiaxisSensorsGetState(void*);

extern errType changeControlMode(void*);
extern errType getControlMode(void*);

extern errType getUZstate(void* fn);
extern errType startFuncControl(void*);
extern errType correctHydroCyl(void*);

extern errType allFoldsGetParams(void* fn);
extern errType allSemiaxisSensorsGetState(void* fn); 
extern errType getAllUZstate(void* fn);
