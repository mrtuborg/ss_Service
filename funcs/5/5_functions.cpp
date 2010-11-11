// Pointing Control Service
//--------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include "../myTypes.h"
#include "../buffer/ssBuffer.h"
#include "../buffer/buffer.h"
#include "../ICAppLayer/cmd.h"
#include "../udp/udp_port.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../ICAppLayer/ICAppLayer.h"
#include "../global.h"

#include "../PS_packet/PS_StatFrame.h"
#include "../PS_packet/PS_CmdFrame.h"

udp_port *equip_sending;
PS_StatFrame statFrame;
PS_CmdFrame cmdFrame;


errType EquipListenProcessing(BYTE *writingBuffer, size_t sz)                   
{                                                                               
    if (sz>sizeof(PS_StatFrameType)) sz=sizeof(PS_StatFrameType);                     
    statFrame.encode(writingBuffer, sz);                                          
    printf("\n\tС иерархии нижнего уровня получен пакет (hex):\n");             
    printf("\t[");                                                              
    for(int k=0; k<sz; k++) printf("%.2X ", writingBuffer[k]);                  
    printf("]\n\n");                                                            
    printf("\tРасшифровка:\n");                                                 
    statFrame.dbgPrint();                                                     
    printf("\t===========================================\n\n");                
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

errType EmergencyShutdown(void* fn)
{
    errType result=err_not_init;
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printResults();
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_not_init;
    
    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
    //func->dbgPrint();
    func->printResults();
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    
    type_StateVector stateVector;
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

    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
    
//    func->setResult(0,&result);
    
    res=statFrame.currMode();
    func->setResult(1,&res);
        
    func->printResults();
    
    return result;
}


errType GetCurrentCoordinates(void* fn) // func_id=2
{
    errType result=err_result_ok;

    FunctionNode* func=(FunctionNode*)fn;
    BYTE btRes=0;
    DWORD dwRes=0;
    double dblRes=0;
    
    func->printParams();
    
        
//    func->setResult(0,&result,lenMyTypes[type_ERRTYPE]);

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
    FunctionNode* func=(FunctionNode*)fn;
    
    BYTE btRes=0;
    DWORD dwRes=0;
    DWORD qwRes=0;
    double dblRes=0;
    
    
    func->printParams();
    
//    func->setResult(0,&result,lenMyTypes[type_ERRTYPE]);
    
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
    FunctionNode* func=(FunctionNode*)fn;
    
    BYTE btRes=0;
    DWORD dwRes=0;
    DWORD qwRes=0;
    double dblRes=0;
    
    
    func->printParams();
    
//    func->setResult(0,&result);
    
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
    FunctionNode* func=(FunctionNode*)fn;
    
    BYTE btRes=0;
    DWORD dwRes=0;
    DWORD qwRes=0;
    double dblRes=0;
    
        
    func->printParams();
    
//    func->setResult(0,&result,type_ERRTYPE);
    
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
    BYTE *prepparams, *msg;
    
    prepparams=new BYTE[13];
    msg=new BYTE[17];
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    filename=(char*)(func->getParamPtr(0));

    cmdFrame.prepParams_prg_mode(filename, prepparams);
    cmdFrame.prepMsg(_mode_prg,prepparams);
    cmdFrame.decode(&msg);
    
//    equip_sending->sendData(&msg);

//    func->setResult(0, &result,type_ERRTYPE);
    func->printResults();

    return result;
}


errType SetKNMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    	    
    result=err_result_ok;
	
    
  
//    func->setResult(0, (BYTE*)&result,1);
    func->printResults();

    return result;
}


errType SetZVVMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
	    
    result=err_result_ok;
	
    

//    func->setResult(0, (BYTE*)&result,1);
    func->printResults();

    return result;
}


errType StopSearchingMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
	    
    result=err_result_ok;
	
    

//    func->setResult(0, (BYTE*)&result,1);
    func->printResults();

    return result;
}

errType StartSearchingMode(void* fn) //func_id=4
{
    errType result=err_not_init;
    BYTE *par=0;
    char str[255];
    str[6]=0;
    
    FunctionNode* func=(FunctionNode*)fn;
    
    func->printParams();
    
    par=(BYTE*)func->getParamPtr(0);
    //printf("val=%d str=",*par);
    //sprintf(str,"%s",*par);
    //    str[6]=0;

    //printf("%s\n",str);
    
    par=(BYTE*)func->getParamPtr(1);
    //printf("val=%d str=",*par);
    //sprintf(str+2,"%s",*par);
    //    str[6]=0;

    //printf("%s\n",str);
    
    par=(BYTE*)func->getParamPtr(2);
    //printf("val=%d str=",*par);
    //sprintf(str+4,"%s",*par);
    //    str[6]=0;

    //printf("%s\n",str);
    
  //  dgram.cmd_id=4;
//    dgram.len=7;//new BYTE[dgram.len-1];
//    dgram.params=(BYTE*)str;
	    
    result=err_result_ok;
	
    
    if (result==err_result_ok) {
  //      psControlSend(&dgram);
    }

//    func->setResult(0, (BYTE*)&result,1);
    func->printResults();

    return result;
}


errType SetCorrection(void* fn) // func_id=5
{
    errType result=err_result_ok;
    
    BYTE *AZ_adj, *ANG_adj, *TIME_adj;
//    ps_control_type dgram;
    
    FunctionNode* func=(FunctionNode*)fn;
    func->printParams();
	
    //func->dbgPrint();
    
//    AZ_adj=(func->getParamPtr(0));    // Azimuth correction
//    ANG_adj=(func->getParamPtr(1));   // Angle correction
//    TIME_adj=(func->getParamPtr(2));  // Time correction
    
    
//    dgram.len=3;

//    dgram.cmd_id=7;
//    dgram.params=AZ_adj;
//    psControlSend(&dgram);

//    dgram.cmd_id=8;
//    dgram.params=ANG_adj;
//    psControlSend(&dgram);

//    dgram.cmd_id=9;
//    dgram.params=TIME_adj;

  //  psControlSend(&dgram);
    
//    func->setResult(0, (BYTE*)&result,1);
    func->printResults();
	
    return result;
}


