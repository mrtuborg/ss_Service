// Astroclimate Control Service
//------------------------------
extern errType srvInit();
extern errType srvDeinit();

extern errType EmergencyShutdown(void*);
extern errType ControlModeChange(void*);
extern errType GetStateVector(void*);

extern errType InstallStarsSet(void*);
extern errType SetMeasuringTerm(void*);
extern errType GetMeasuringResult(void*);
