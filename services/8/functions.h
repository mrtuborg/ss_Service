// Angular coordinates measuring system
//--------------------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType equipListenProcessing(BYTE *writingBuffer, size_t sz);

extern errType StartMeasuringProcess(void*); //  1-> answer 2: start successfull
extern errType FinishMeasuringProcess(void*); // 3-> answer 4: finish successfull
