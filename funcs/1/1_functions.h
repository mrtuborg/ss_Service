// Scheduling Control Service
//-----------------------------
extern errType srvInit();
extern errType srvDeinit();
extern void* PollingThreadFunc(void*);

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType CreateGeneralSchedule(void*);
extern errType CreateEmergencySchedule(void*);
extern errType ReadGeneralSchedule(void*);
extern errType ReadEmergencySchedule(void*);
extern errType GetCursorPosition(void*);
