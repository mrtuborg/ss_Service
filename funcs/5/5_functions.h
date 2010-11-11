// Pointing Control Service
//---------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType EquipListenProcessing(BYTE *, size_t);

extern errType GetFunctionMode(void*);
extern errType GetCurrentCoordinates(void*);
extern errType GetPointingExtraParams(void*);
extern errType SetCorrection(void*);
extern errType SetProgrammMode(void*);
extern errType SetKNMode(void*);
extern errType SetZVVMode(void*);
extern errType StartSearchingMode(void*);
extern errType StopSearchingMode(void*);

