#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "../../../ortsTypes/ortsTypes.h"
#include "global.h"
#include "../../crc/crc16.h"
#include "statusFrame.h"

statusFrame::statusFrame()
{
    memset(&frame,0,sizeof(statusFrame_t));
}

statusFrame::~statusFrame()
{
}

BYTE statusFrame::getShieldState(BYTE num)// + opened
{
    //0000 0sco - stopped, closed, opened
    BYTE result=0;
    switch (num)
    {
	case 0: result=frame.schieldOpened; break;
	case 1: result=frame.schieldClosed; break;
	case 2: result=frame.schieldStopped; break;
    }

    return result;
}

BYTE statusFrame::getHydroSystemState(BYTE num)
{
    // ...1 - alert
    // ..1. - norm
    // .1.. - ready
    // 1... - poweron

     BYTE result=0;
     switch (num)
     {
        case 0:
	    result=frame.hydroSystem_status.fields.alert;
	    break;
        case 1:
	    result=frame.hydroSystem_status.fields.norm;
	    break;
        case 2:
	    result=frame.hydroSystem_status.fields.ready;
	    break;
        case 3:
	    result=frame.hydroSystem_status.fields.poweron;
	    break;
        
     }
     return result;
}

BYTE statusFrame::getHydroSystemValues(BYTE num) // + filters
{
    BYTE result=0;
    
    switch (num)
    {
	case 0: result=frame.hydroSystem_values.fields.pressure_norm; break;
	case 1: result=frame.hydroSystem_values.fields.level_norm; break;
	case 2: result=frame.hydroSystem_values.fields.temp_norm; break;
	case 3: result=frame.hydroSystemFilters_norm.value; break;
    }
    return result;
}

bool statusFrame::isLocalControl()
{
     bool result=frame.controlSwitch.fields.localControl;
     return result;
}

bool statusFrame::isRemoteControl()
{
     bool result=frame.controlSwitch.fields.remoteControl;
     return result;
}

BYTE statusFrame::getSystemLinkStatus(BYTE num)
{
    // BUZ =0
    // AUGS=1
    // localPanel=2
    // KEGP=3
    
    BYTE result=0;
    
    switch (num)
    {
	case 0: result=frame.systemsLink_status.fields.BUZ_link; break;
	case 1: result=frame.systemsLink_status.fields.AUGS_link; break;
	case 2: result=frame.systemsLink_status.fields.localPanel_link; break;
	case 3: result=frame.systemsLink_status.fields.KEGP_link; break;
    };
    
    return result;
}

BYTE statusFrame::getFoldState(BYTE num)
{
    //0 Up opened
    //1 Up closed
    //2 B opened
    //3 B closed
    //4 A opened
    //5 A closed
    
    BYTE result=0;
    //printf("get: num=%d,",num);
    switch (num)
    {
	case 0: result=frame.foldStatus.fields.id[0].fields.value_0; break;
	case 1: result=frame.foldStatus.fields.id[0].fields.value_1; break;
	case 2: result=frame.foldStatus.fields.id[1].fields.value_0; break;
	case 3: result=frame.foldStatus.fields.id[1].fields.value_1; break;
	case 4: result=frame.foldStatus.fields.id[2].fields.value_0; break;
	case 5: result=frame.foldStatus.fields.id[2].fields.value_1; break;
    }
    //printf("result=%d\n",result);

    return result;
}

bool statusFrame::getFuncControlStatus()
{
     return frame.funcControl_busy;
}

bool statusFrame::getLinkingStatus()
{
    return frame.linkingStatus;
}

bool statusFrame::getRemotePanelStatus()
{
    return frame.remotePanel_busy;
}

BYTE statusFrame::isUZ_locked(BYTE num)
{
     return frame.UZ_locked.fields.UZ[num].value;
}

BYTE statusFrame::isUZ_unlocked(BYTE num)
{
     return frame.UZ_unlocked.fields.UZ[num].value;
}

BYTE statusFrame::isUZ_busy(BYTE num)
{
     return frame.UZ_busy.fields.UZ[num].value;
}

BYTE statusFrame::isUZ_manual(BYTE num)
{
     return frame.UZ_manual.fields.UZ[num].value;
}

BYTE statusFrame::isUZ_alert(BYTE num)
{
     return frame.UZ_alert.fields.UZ[num].value;
}


BYTE statusFrame::getHydroCylStatus(BYTE num) // setting, resetting
{
    BYTE result=0;
    if (num>2) result=(frame.hydroCyl_setting.fields.reseting & 0x01);
    else result= (frame.hydroCyl_setting.fields.invalid_fold[num].value & 0x01);
    
    return result;
}

WORD statusFrame::getFoldPosition(BYTE num) // 0 - U, 1 - B, 2 - A
{
    WORD result=0;
    
    switch (num){
	case 0:  // upper fold
	    result = (frame.foldU_value<<1);
	    //printf("\nfoldU_value=%d\n", foldU_value);
	    break;
	case 1:  // B-fold
	    result = (frame.foldB_value<<1);
	    //printf("\nfoldB_value=%d\n", foldB_value);
	    break;
	case 2:  // A-fold
	    result = (frame.foldA_value<<1);
	    //printf("\nfoldA_value=%d\n", foldA_value);
	    break;
    }
    result|= (frame.fold_val_p0.fields.fold[num].value & 0x01);
    result|=((WORD)(frame.fold_val_p8.fields.fold[num].value & 0x01) << 8);
    
    return result;
}

BYTE statusFrame::get_psa_sensors(BYTE num)                // num = 0,1,2 state = 00, 01, 10 ,11 (left.right)
{
    return frame.psa_sensor.fields.fold[num].value & 0x03;
}

BYTE statusFrame::get_esa_sensors(BYTE num)                // num = 0,1,2 state = 00, 01, 10 ,11 (left.right)
{
    return frame.esa_sensor.fields.fold[num].value & 0x03;
}

bool statusFrame::testCheckSumm()
{
     bool result=false;
     WORD chk=CRC16_eval((BYTE*)&frame, sizeof(statusFrame_t)-sizeof(WORD));
     if (chk==frame.checkSumm) result=true;
     
     return result;
}

void statusFrame::decode(BYTE** array)
{
    *array=new BYTE[sizeof(statusFrame_t)];
    memcpy(*array, &frame, sizeof(statusFrame_t));
}

void statusFrame::encode(BYTE* array, size_t size)
{
    memcpy(&frame,array,size);
}

void statusFrame::dbgPrint()
{
	printf("СТОП укрытия=%d\n",frame.schieldStopped);
	printf("Укрытие закрыто=%d\n",frame.schieldClosed);
	printf("Укрытие открыто=%d\n",frame.schieldOpened);
	printf("Контроль функционирования исполняется=%d\n",frame.funcControl_busy);
	printf("Стыковка=%d\n",frame.linkingStatus);
	printf("Работа контроллера ПДУ=%d\n\n",frame.remotePanel_busy);
	
	printf("Створка верхняя  закрыта=%d\n",frame.foldStatus.fields.id[0].fields.value_1); // bit0 - open, bit1 -close
	printf("Створка верхняя  открыта=%d\n",frame.foldStatus.fields.id[0].fields.value_0); // bit0 - open, bit1 -close
	printf("Створка нижняя Б закрыта=%d\n",frame.foldStatus.fields.id[1].fields.value_1); // bit0 - open, bit1 -close
	printf("Створка нижняя Б открыта=%d\n",frame.foldStatus.fields.id[1].fields.value_0); // bit0 - open, bit1 -close
	printf("Створка нижняя А закрыта=%d\n",frame.foldStatus.fields.id[2].fields.value_1); // bit0 - open, bit1 -close
	printf("Створка нижняя А открыта=%d\n\n",frame.foldStatus.fields.id[2].fields.value_0); // bit0 - open, bit1 -close

	printf("Гидросистема, Авария=%d\n",frame.hydroSystem_status.fields.alert);
	printf("Гидросистема, Норма=%d\n",frame.hydroSystem_status.fields.norm);
	printf("Гидросистема, Готова=%d\n",frame.hydroSystem_status.fields.ready);
	printf("Гидросистема, Включена=%d\n\n",frame.hydroSystem_status.fields.poweron);

	printf("Управление от ПМУ=%d\n",frame.controlSwitch.fields.localControl);
	printf("Управление от ПДУ=%d\n\n",frame.controlSwitch.fields.remoteControl);

	printf("Гидросистема, давление, норма=%d\n",frame.hydroSystem_values.fields.pressure_norm);
	printf("Гидросистема, уровень, норма=%d\n",frame.hydroSystem_values.fields.level_norm);
	printf("Гидросистема, температура, норма=%d\n",frame.hydroSystem_values.fields.temp_norm);
	printf("Гидросистема, фильтры, норма=%d\n\n",frame.hydroSystemFilters_norm.value);

	printf("Связь с БУЗ=%d\n",frame.systemsLink_status.fields.BUZ_link);
	printf("Связь с АУГС=%d\n",frame.systemsLink_status.fields.AUGS_link);
	printf("Связь с ПМУ=%d\n",frame.systemsLink_status.fields.localPanel_link);
	printf("Связь с КЭГП=%d\n\n",frame.systemsLink_status.fields.KEGP_link);

	printf("УЗ №3 заштырено=%d\n",frame.UZ_locked.fields.UZ[0].value);
	printf("УЗ №3 отштырено%d\n",frame.UZ_unlocked.fields.UZ[0].value);
	printf("УЗ №3 исполнение=%d\n",frame.UZ_busy.fields.UZ[0].value);
	printf("УЗ №3 ручное=%d\n",frame.UZ_manual.fields.UZ[0].value);
	printf("УЗ №3 неисправность=%d\n\n",frame.UZ_alert.fields.UZ[0].value);
	
	printf("УЗ №2 заштырено=%d\n",frame.UZ_locked.fields.UZ[1].value);
	printf("УЗ №2 отштырено=%d\n",frame.UZ_unlocked.fields.UZ[1].value);
	printf("УЗ №2 исполнение=%d\n",frame.UZ_busy.fields.UZ[1].value);
	printf("УЗ №2 ручное=%d\n",frame.UZ_manual.fields.UZ[1].value);
	printf("УЗ №2 неисправность=%d\n\n",frame.UZ_alert.fields.UZ[1].value);
	
	printf("УЗ №1 заштырено=%d\n",frame.UZ_locked.fields.UZ[2].value);
	printf("УЗ №1 отштырено=%d\n",frame.UZ_unlocked.fields.UZ[2].value);
	printf("УЗ №1 исполнение=%d\n",frame.UZ_busy.fields.UZ[2].value);
	printf("УЗ №1 ручное=%d\n",frame.UZ_manual.fields.UZ[2].value);
	printf("УЗ №1 неисправность=%d\n\n",frame.UZ_alert.fields.UZ[2].value);
	
	printf("Режим согласования ГЦ=%d\n",frame.hydroCyl_setting.fields.reseting);
	printf("Створка нижняя А, рассогласование ГЦ=%d\n",frame.hydroCyl_setting.fields.invalid_fold[0].value);
	printf("Створка нижняя Б, рассогласование ГЦ=%d\n",frame.hydroCyl_setting.fields.invalid_fold[1].value);
	printf("Створка верхня, рассогласование ГЦ=%d\n",frame.hydroCyl_setting.fields.invalid_fold[2].value);
	printf("Створка А, угол =0x%.4X\n",getFoldPosition(0));
	printf("Створка Б, угол =0x%.4X\n",getFoldPosition(1));
	printf("Створка верхняя, угол =0x%.4X\n\n",getFoldPosition(2));
	printf("Верхняя створка, путевой выключатель, левый=%d ",frame.psa_sensor.fields.fold[0].fields.value_1);
	printf("правый=%d\n",frame.psa_sensor.fields.fold[0].fields.value_0);
	printf("Нижняя Б створка, путевой выключатель, левый=%d ",frame.psa_sensor.fields.fold[1].fields.value_1);
	printf("правый=%d\n",frame.psa_sensor.fields.fold[1].fields.value_0);
	printf("Нижняя А створка, путевой выключатель, левый=%d ",frame.psa_sensor.fields.fold[2].fields.value_1);
	printf("правый=%d\n",frame.psa_sensor.fields.fold[2].fields.value_0);
	
	printf("Верхняя створка, концевой выключатель, левый=%d ",frame.esa_sensor.fields.fold[0].fields.value_1);
	printf("правый=%d\n",frame.esa_sensor.fields.fold[0].fields.value_0);
	printf("Нижняя Б створка, концевой выключатель, левый=%d ",frame.esa_sensor.fields.fold[1].fields.value_1);
	printf("правый=%d\n",frame.esa_sensor.fields.fold[1].fields.value_0);
	printf("Нижняя А створка, концевой выключатель, левый=%d ",frame.esa_sensor.fields.fold[2].fields.value_1);
	printf("правый=%d\n\n",frame.esa_sensor.fields.fold[2].fields.value_0);

	printf("Контрольная сумма=0x%.4X\n",frame.checkSumm);
}