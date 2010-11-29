// Covering Control Service
//---------------------------
extern errType srvInit();
extern errType srvDeinit();

errType EquipListenProcessing(BYTE *writingBuffer, size_t sz);
extern void* PollingThreadFunc(void*);

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType ShieldPowerON(void*);
extern errType ShieldPowerOFF(void*);
extern errType HydroSystemPowerON(void*);
extern errType HydroSystemPowerOFF(void*);

extern errType HydroSystemGetParams(void*);
extern errType FoldOpen(void*);
extern errType FoldClose(void*);
extern errType FoldStop(void*);

extern errType ShieldOpen(void*);
extern errType ShieldClose(void*);
extern errType ShieldStop(void*);

extern errType FoldGetParams(void*);
extern errType SemiaxisGetValue(void*);

extern errType ControlMode(void*);

extern errType StartFuncControl(void*);
extern errType CorrectHydroCyl(void*);