// SASC Service
//---------------

extern errType equipListenProcessing(BYTE *writingBuffer, size_t sz);

extern errType srvInit();
extern errType srvDeinit();

extern errType emergencyShutdown(void*);
extern errType controlModeChange(void*);
extern errType getStateVector(void*);

extern errType SASC_PowerON(void*);
extern errType SASC_PowerOFF(void*);
extern errType StartMeasuring(void*);
extern errType ZeroMeasuring(void*);
extern errType GetMeasuringResult(void*);
extern errType ZeroDB(void*);
extern errType TaringStart(void*);
extern errType GetTaringPoint(void*);
extern errType TaringStop(void*);
extern errType linkTest(void*);