// Selfdiagnostics system service
//--------------------------------

extern errType EquipListenProcessing(BYTE *writingBuffer, size_t sz);

extern errType srvInit();
extern errType srvDeinit();

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType GetProcessesList(void*);
extern errType GetProcessInfo(void*);
extern errType RebootSystem(void*);
extern errType SearchProcess(void*);
extern errType KillProcess(void*);
extern errType RestartProcess(void*);
extern errType InstallFilter(void*);
