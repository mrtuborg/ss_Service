typedef enum {
  _off=0,
  _manual=1,
  _programm=2,
  _KN=3,
  _AS=4,
  _search=5,
  _ZVV=6
} PS_modeType;

    
typedef enum {
    _wide=0,
    _narrow=1,
} fieldType;
    
typedef enum {
    _discrete_1s=0,
    _discrete_5s=1,
    _discrete_15s=2,
    _discrete_1m=3,
    _discrete_2m=4,
    _discrete_5m=5
}   discreteType;

typedef struct PS_StatFrameType {
	PS_modeType currMode;
	uint16_t year, month, day;
	uint32_t currTime;
	uint32_t astralTime;
	double DOS_Azimuth;
	double DOS_Elevation;
	double PRG_Azimuth;
	double PRG_Elevation;
	double AzimuthCorrection;
	double ElevationCorrection;
	double AzimuthBearing;
	double ElevationBearing;
	double PRG_AzimuthVelocity;
	double PRG_ElevationVelocity;
	double CurrentAzimuthVelocity;
	double CurrentElevationVelocity;
	double AzimuthBinding;
	double ElevationBinding;
	uint64_t currTimeCorrection;
	BYTE   equipmentState;
	fieldType	field;
	discreteType	discrete;

} PS_StatFrameType;

class PS_StatFrame{
    PS_StatFrameType *frame;
    public:
    
    PS_StatFrame();
    ~PS_StatFrame();

    errType decode(BYTE** );                                                         
    errType encode(BYTE*, DWORD size);     
    void dbgPrint();
    
    DWORD currTime();
    DWORD astralTime();
    
    PS_modeType currMode();
    
    double DOS_Azimuth();
    double DOS_Elevation();
    double PRG_Azimuth();
    double PRG_Elevation();
    double AzimuthBearing();
    double ElevationBearing();
    double PRG_AzimuthVelocity();
    double PRG_ElevationVelocity();
    double CurrentAzimuthVelocity();
    double CurrentElevationVelocity();
    double AzimuthBinding();
    double ElevationBinding();
    double AzimuthCorrection();
    double ElevationCorrection();
    QWORD currTimeCorrection();
    
    BYTE equipmentState();
    fieldType field();
    discreteType discrete();
};
