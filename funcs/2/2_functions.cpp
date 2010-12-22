// Astroclimate Control Service
//------------------------------
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "../myTypes.h"
#include "../ICAppLayer/cmd.h"
#include "../ICAppLayer/FunctionNode/param_desc.h"
#include "../ICAppLayer/FunctionNode/FunctionNode.h"
#include "../buffer/buffer.h"
#include "../udp/udp_port.h"
#include "../global.h"

udp_port* equipment;

typedef struct {
    DWORD azimuth;
    DWORD angle;
} ASTR_COORD;

typedef struct {
         DWORD time_ms;
    ASTR_COORD coord;

} astrTrack_node;

typedef struct {
	DWORD time_us;
	DWORD t_long;
} time_term;

typedef DWORD STAR_ID;

typedef struct {
	STAR_ID star_id;
	DWORD time_us;
	ASTR_COORD coord;
	DWORD brightness;
} brightness_node;

errType srvInit()
{
    equipment=new udp_port(EQ_UDP_PORT);
    equipment->open_port();
    equipAddr.s_addr=inet_addr(EQ_IP_ADDR);
    return err_result_ok;
}

errType srvDeinit()
{
    equipment->close_port();
    delete equipment;
    return err_result_ok;
}

void PollingThreadFunc(void*)
{

}

errType EmergencyShutdown(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** EmergencyShutdown was called! ***\n");
    printf("*************************************\n");
    
    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType ControlModeChange(void* fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** ControlModeChange was called! ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();
    
    return result;
}

errType GetStateVector(void* fn)
{
    errType result=err_result_ok;
    functionNode* func=(functionNode*)fn;
    func->printParams();
    func->setResult(1,&app->getStateVector());
    func->printResults();

    return result;

}

errType InstallStarsSet(void *fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** InstallStarsSet was called!   ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    func->dbgPrint();
    
    // Request to "Balistic Center" for stars brihtnesses set by:
    // 1) Astrotrajectory
    // 2) TimeTerm
    
    WORD star_brs_quantity=*((WORD*)(func->getParamPtr(0)));
//    WORD stars_quantity=100;
    brightness_node *star_brs[100];
    BYTE *buffer;
    WORD offset=0;
    buffer=new BYTE [sizeof(brightness_node)];
    printf("Stars brightnesses catalog:\n");
    printf("===========================\n");
    for (int i=0; i<star_brs_quantity; i++) {
	star_brs[i]=(brightness_node*)(func->getParamPtr(1)+offset);
	
	printf("star_id: %lu\n",star_brs[i]->star_id);
	printf("time: %lu\n",star_brs[i]->time_us);
	printf("azimuth: %lu\n",star_brs[i]->coord.azimuth);
	printf("angle: %lu\n",star_brs[i]->coord.angle);
	printf("brightness: %lu\n\n",star_brs[i]->brightness);
	
	offset+=sizeof(brightness_node);
    }
    
    for (int i=0; i<star_brs_quantity; i++) {
	memcpy(buffer,&star_brs[i], sizeof(brightness_node));
	equipment->sendData(equipAddr, buffer, sizeof(brightness_node));

    }
    
    delete buffer;

    return result;
}

errType SetMeasuringTerm(void *fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** SetMeasuringTerm was called!  ***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    func->dbgPrint();
    
    time_term *period;
    period=(time_term*)func->getParamPtr(0);
    
    printf("Measuring will be starts at %lu us\n", period->time_us);
    printf("Measuring will be lasts  %lu us\n", period->t_long);
    equipment->sendData(equipAddr, (BYTE*) period, sizeof(time_term));

    return result;
}

errType GetMeasuringResult(void *fn)
{
    errType result=err_not_init;
    printf("*************************************\n");
    printf("*** GetMeasuringResult was called!***\n");
    printf("*************************************\n");

    functionNode* func=(functionNode*)fn;
    
    func->dbgPrint();

    return result;
}