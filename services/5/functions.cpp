// Pointing Control Service
//--------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <ssBuffer.h>
#include <buffer.h>
#include <peripheral/udp_port/udp_port.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>
#include <SrvAppLayer.h>
#include <math/latlong/latlong.h>
#include <global.h>

#include "PS_packet/PS_StatFrame.h"
#include "PS_packet/PS_CmdFrame.h"

udp_port *equip_sending;
PS_StatFrame statFrame;
PS_CmdFrame cmdFrame;


errType equipListenProcessing(BYTE *writingBuffer, size_t sz)
{
    size_t size (*sz);

    if (size>sizeof(PS_StatFrameType)) size=sizeof(PS_StatFrameType);
    statFrame.encode(writingBuffer, size);
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");
    printf("\t[");
    for(size_t k=0; k<size; k++) printf("%.2X ", writingBuffer[k]);
    printf("]\n\n");
    printf("\tРасшифровка:\n");
    statFrame.dbgPrint();


    printf("\t===========================================\n\n");
    return err_result_ok;
}


errType srvInit()
{

    printf("\tСлужба взаимодействия с АРМ \"Система наведения\"\n");
    printf("=============================================================\n\n");

    equip_sending=new udp_port(eq_udp_sending_port);
    equip_sending->open_port();
    equipAddr.s_addr=inet_addr(eq_ip_addr);
    return err_result_ok;
}

errType srvDeinit()
{
    delete equip_sending;
    return err_result_ok;
}

errType emergencyShutdown(void* fn)
{
    errType result=err_not_init;
    functionNode* func=(functionNode*)fn;

    func->printResults();
    return result;
}

errType controlModeChange(void* fn)
{
    errType result=err_not_init;

    functionNode* func=(functionNode*)fn;
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType getStateVector(void* fn)
{
    errType result=err_result_ok;

    functionNode* func=(functionNode*)fn;

    stateVector_type stateVector;
    stateVector=app->getStateVector();

    func->printParams();
    func->setResult(1,&stateVector);
    func->printResults();
    return result;
}

//============================================================


errType GetFunctionMode(void* fn) // func_id=1
{
    errType result=err_result_ok;

    BYTE res=0;

    functionNode* func=(functionNode*)fn;
    func->printParams();

    res=statFrame.currMode();
    func->setResult(1,&res);

    func->printResults();

    return result;
}


errType GetCurrentCoordinates(void* fn) // func_id=2
{
    errType result=err_result_ok;

    functionNode* func=(functionNode*)fn;

    DWORD dwRes=0;
    double dblRes=0;

    func->printParams();

    dwRes=statFrame.currTime();
    func->setResult(1,&dwRes);

    dwRes=statFrame.astralTime();
    func->setResult(2,&dwRes);

    dblRes=statFrame.DOS_Azimuth();
    func->setResult(3,&dblRes);

    dblRes=statFrame.DOS_Elevation();
    func->setResult(4,&dblRes);

    dblRes=statFrame.PRG_Azimuth();
    func->setResult(5,&dblRes);

    dblRes=statFrame.PRG_Elevation();
    func->setResult(6,&dblRes);


    func->printResults();

    return result;
}


errType getPointingCorrectionValues(void* fn) // func_id=3
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;

    DWORD dwRes=0;
    DWORD qwRes=0;
    double dblRes=0;


    func->printParams();

    dwRes=statFrame.currTime();
    func->setResult(1,&dwRes);

    dblRes=statFrame.AzimuthCorrection();
    func->setResult(2,&dblRes);

    dblRes=statFrame.ElevationCorrection();
    func->setResult(3,&dblRes);

    qwRes=statFrame.currTimeCorrection();
    func->setResult(4,&qwRes);

    func->printResults();

    return result;
}

errType getPointingEquipmentParams(void* fn) // func_id=4
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;

    BYTE btRes=0;
    DWORD dwRes=0;


    func->printParams();


    dwRes=statFrame.currTime();
    func->setResult(1,&dwRes);

    btRes=statFrame.equipmentState();
    func->setResult(2,&btRes);

    btRes=statFrame.field();
    func->setResult(3,&btRes);

    btRes=statFrame.discrete();
    func->setResult(4,&btRes);

    func->printResults();

    return result;
}


errType GetPointingExtraParams(void* fn) //func_id=5
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;

    DWORD dwRes=0;
    double dblRes=0;


    func->printParams();

    dwRes=statFrame.currTime();
    func->setResult(1,&dwRes);

    dblRes=statFrame.AzimuthBearing();
    func->setResult(2,&dblRes);

    dblRes=statFrame.ElevationBearing();
    func->setResult(3,&dblRes);

    dblRes=statFrame.PRG_AzimuthVelocity();
    func->setResult(4, &dblRes);

    dblRes=statFrame.PRG_ElevationVelocity();
    func->setResult(5,&dblRes);

    dblRes=statFrame.CurrentAzimuthVelocity();
    func->setResult(6,&dblRes);

    dblRes=statFrame.CurrentElevationVelocity();
    func->setResult(7,&dblRes);

    dblRes=statFrame.AzimuthBinding();
    func->setResult(8,&dblRes);

    dblRes=statFrame.ElevationBinding();
    func->setResult(9,&dblRes);

    func->printResults();

    return result;
}



errType SetProgrammMode(void* fn) //func_id=6
{
    errType result=err_result_ok;
    char *filename;
    int len=0;

    BYTE prepparams[13];
    BYTE *msg=new BYTE[17];

    functionNode* func=(functionNode*)fn;

    func->printParams();

    len = *(WORD*)(func->getParamPtr(0));
    filename = (char*)(func->getParamPtr(0))+2;

    //if (len<1) len=1;
   // filename[len]=0;

    cmdFrame.prepParams_prg_mode(filename, len, prepparams);
    cmdFrame.prepMsg(_mode_prg,prepparams);
    cmdFrame.decode(&msg);

    equip_sending->sendData(equipAddr, msg, sizeof(ps_cmdframe_type));

    func->printResults();
    delete []msg;
    return result;
}


errType SetKNMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    functionNode* func=(functionNode*)fn;

    BYTE* msg=new BYTE[17];

    func->printParams();

    cmdFrame.prepMsg(_mode_KN);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    result=err_result_ok;

    func->printResults();
    delete []msg;
    return result;
}


errType SetZVVMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    BYTE *msg=new BYTE[17];

    functionNode* func=(functionNode*)fn;

    func->printParams();

    result=err_result_ok;

    cmdFrame.prepMsg(_mode_ZVV);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    func->printResults();
    delete []msg;
    return result;
}


errType StopSearchingMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    BYTE *msg=new BYTE[17];

    functionNode* func=(functionNode*)fn;

    func->printParams();


    result=err_result_ok;

    cmdFrame.prepMsg(_mode_srchStop);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    func->printResults();
    delete []msg;
    return result;
}

errType StartSearchingMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    BYTE counts=0, step=0, stayTime=0;
    BYTE prepparams[13];

    //char str[255];
    prepparams[6]=0;

    BYTE *msg=new BYTE[17];

    functionNode* func=(functionNode*)fn;

    func->printParams();

    counts=*(BYTE*)func->getParamPtr(0);
    step=*(BYTE*)func->getParamPtr(1);
    stayTime=*(BYTE*)func->getParamPtr(2);

    result=cmdFrame.prepParams_srch_start(counts,step,stayTime,prepparams);

    if (result==err_result_ok){

    	cmdFrame.prepMsg(_mode_srchStart,prepparams);
    	cmdFrame.decode(&msg);
    	result=equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());
    }

   // result=err_result_ok;

    func->printResults();
    delete []msg;
    return result;
}


errType SetCorrection(void* fn) // func_id=5
{
    errType result=err_result_ok;
    BYTE prepparams[13];
    BYTE *msg = new BYTE[17];

    functionNode* func=(functionNode*)fn;
    func->printParams();

	int16_t msec, tm_sec, AZ_grad, EL_grad;
	uint16_t tm_msec;
	uint8_t AZ_min, EL_min, AZ_sec, EL_sec;

	double azimuth, elevation;


	azimuth=*(double*)func->getParamPtr(0); // Azimuth correction
	elevation=*(double*)func->getParamPtr(1); // Elevation correction
	msec=*(int*)func->getParamPtr(2); // Time correction: DWORD unsigned = int signed;

	int tm_sign=1;
	if (msec<0) tm_sign=-1;

	tm_msec=(msec*tm_sign)%1000;
	tm_sec=msec/1000;
	printf("sec=%d\n",tm_sec);
	printf("msec=%d\n",tm_msec);

	doubleToDegrees(azimuth, &AZ_grad, &AZ_min, &AZ_sec);
	doubleToDegrees(elevation, &EL_grad, &EL_min, &EL_sec);

    cmdFrame.prepParams_new_AZ_corr(AZ_grad, AZ_min, AZ_sec, prepparams);
    cmdFrame.prepMsg(_corr_AZ, prepparams);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    cmdFrame.prepParams_new_EL_corr(EL_grad, EL_min, EL_sec, prepparams);
    cmdFrame.prepMsg(_corr_EL, prepparams);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    cmdFrame.prepParams_new_TM_corr(tm_sign, tm_sec, tm_msec, prepparams);
    cmdFrame.prepMsg(_corr_TM, prepparams);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    func->printResults();
	delete []msg;
    return result;
}

errType SuspendMode(void* fn) // func_id=5
{
    errType result=err_result_ok;
    BYTE *msg = new BYTE[17];

    functionNode* func=(functionNode*)fn;
    func->printParams();

    cmdFrame.prepMsg(_mode_off);
    cmdFrame.decode(&msg);
    equip_sending->sendData(equipAddr, msg, cmdFrame.lenValue());

    func->printResults();
    delete []msg;
    return result;
}



