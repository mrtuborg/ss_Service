// Scheduling Control Service
//-----------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType equipListenProcessing(BYTE *writingBuffer, size_t sz);
extern void* pollingThread(void* user);

//extern void* PollingThreadFunc(void*);

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType addScheduleJob(void*);
extern errType runSchedule(void*);
extern errType stopSchedule(void*);

extern errType readJobState(void* fn);
extern errType readJobEntity(void* fn);
extern errType getCursorPosition(void* fn);
extern errType getOpsId(void* fn);
extern errType executeJob(void* fn);

