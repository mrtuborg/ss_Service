// Andaptive optics  measuring system
//--------------------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType EquipListenProcessing(BYTE *writingBuffer, size_t sz);

extern errType StartMeasuringProcess(void*); //  1-> answer 2: start successfull
extern errType FinishMeasuringProcess(void*); // 3-> answer 4: finish successfull
