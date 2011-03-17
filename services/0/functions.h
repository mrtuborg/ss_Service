// Selfdiagnostics system service
//--------------------------------

extern errType equipListenProcessing(BYTE *writingBuffer, size_t *sz);

extern errType srvInit();
extern errType srvDeinit();

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType GetProcessesList(void*);
extern errType GetProcessInfo(void*);
extern errType RebootSystem(void*);
extern errType SearchProcess(void*);
extern errType KillProcess(void*);
extern errType RestartProcess(void*);
extern errType InstallFilter(void*);
