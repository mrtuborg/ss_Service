// Pointing Control Service
//---------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType equipListenProcessing(BYTE *, size_t *size);

extern errType GetFunctionMode(void*);
extern errType GetCurrentCoordinates(void*);
extern errType GetPointingExtraParams(void*);
extern errType SetCorrection(void*);
extern errType SetProgrammMode(void*);
extern errType SetKNMode(void*);
extern errType SetZVVMode(void*);
extern errType StartSearchingMode(void*);
extern errType StopSearchingMode(void*);
extern errType SuspendMode(void*);

