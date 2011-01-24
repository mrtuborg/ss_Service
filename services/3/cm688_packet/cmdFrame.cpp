#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "extra/ortsTypes/ortsTypes.h"
#include "math/crc/crc16.h"
#include "cmdFrame.h"

cmdFrame::cmdFrame()
{
    memset(&frame,0,sizeof(cmdFrame_t));
    frame.header=0xAA55;
    
}

cmdFrame::~cmdFrame()
{
}

void cmdFrame::setHydroCyl_correct()
{
    frame.hydroCyl_corr=1;
}

void cmdFrame::setHydroStationStart()
{
    frame.hydroStation.cmd.start=1;
    frame.hydroStation.cmd.stop=0;
}

void cmdFrame::setHydroStationStop()
{
    frame.hydroStation.cmd.start=0;
    frame.hydroStation.cmd.stop=1;
}

void cmdFrame::setSchieldClose()
{
    frame.schield.cmd.close=1;
    frame.schield.cmd.open=0;
    frame.schield.cmd.stop=0;
}

void cmdFrame::setSchieldOpen()
{
    frame.schield.cmd.close=0;
    frame.schield.cmd.open=1;
    frame.schield.cmd.stop=0;
}

void cmdFrame::setSchieldStop()
{
    frame.schield.cmd.close=0;
    frame.schield.cmd.open=0;
    frame.schield.cmd.stop=1;
}

void cmdFrame::setCPControl()
{
    frame.ctrlSwitch.cmd.remoteControl=0;
    frame.ctrlSwitch.cmd.localControl=0;
}

void cmdFrame::setRemoteControl()
{
    frame.ctrlSwitch.cmd.remoteControl=1;
    frame.ctrlSwitch.cmd.localControl=0;
}

void cmdFrame::setLocalControl()
{
    frame.ctrlSwitch.cmd.remoteControl=0;
    frame.ctrlSwitch.cmd.localControl=1;
}

void cmdFrame::setFoldOpen(BYTE num)
{
    frame.foldCmd.id[num].fields.value_0=0; //close
    frame.foldCmd.id[num].fields.value_1=1; //open
    frame.foldStop.cmd.id[num].value=0;
}

void cmdFrame::setFoldClose(BYTE num)
{
    frame.foldCmd.id[num].fields.value_0=1; //close
    frame.foldCmd.id[num].fields.value_1=0; //open
    frame.foldStop.cmd.id[num].value=0;
}

void cmdFrame::setFoldStop(BYTE num)
{
    frame.foldCmd.id[num].fields.value_0=0; //close
    frame.foldCmd.id[num].fields.value_1=0; //open
    frame.foldStop.cmd.id[num].value=1;
}

void cmdFrame::setStartFuncControl()
{
    frame.funcCtrl=1;
}

WORD cmdFrame::setCheckSumm()
{
    frame.checkSumm=CRC16_eval((BYTE*)&frame, sizeof(cmdFrame_t)-sizeof(frame.checkSumm));
    return frame.checkSumm;
}


void cmdFrame::decode(BYTE** array)
{
    *array=new BYTE[sizeof(cmdFrame_t)];
    memcpy(*array, &frame, sizeof(cmdFrame_t));
}

void cmdFrame::encode(BYTE* array, size_t size)
{
    memcpy(&frame,array,size);
}

void cmdFrame::dbgPrint()
{
	printf("Коррекция ГЦ=%d\n\n",frame.hydroCyl_corr);
	
	printf("Укрытие закрыть=%d\n",frame.schield.cmd.close);
	printf("Укрытие открыть=%d\n",frame.schield.cmd.open);
	printf("Укрытие стоп=%d\n\n",frame.schield.cmd.stop);
	
	printf("ПУСК гидронасосной станции=%d\n",frame.hydroStation.cmd.start);
	printf("СТОП гидронасосной станции=%d\n\n",frame.hydroStation.cmd.stop);

	printf("Управление от ПДУ=%d\n",frame.ctrlSwitch.cmd.remoteControl);
	printf("Управление от ПМУ=%d\n\n",frame.ctrlSwitch.cmd.localControl);

	printf("Створка верхняя, закрыть=%d\n",frame.foldCmd.id[0].fields.value_0);
	printf("Створка верхняя, открыть=%d\n",frame.foldCmd.id[0].fields.value_1);
	printf("Створка нижняя Б, закрыть=%d\n",frame.foldCmd.id[1].fields.value_0);
	printf("Створка нижняя Б, открыть=%d\n",frame.foldCmd.id[1].fields.value_1);
	printf("Створка нижняя А, закрыть=%d\n",frame.foldCmd.id[2].fields.value_0);
	printf("Створка нижняя А, открыть=%d\n\n",frame.foldCmd.id[2].fields.value_1);
	
	printf("Створка верхняя, СТОП=%d\n",frame.foldStop.cmd.id[0].value);
	printf("Створка нижняя Б, СТОП=%d\n",frame.foldStop.cmd.id[1].value);
	printf("Створка нижняя А, СТОП=%d\n\n",frame.foldStop.cmd.id[2].value);
	
	printf("Контроль функционирования=%d\n",frame.funcCtrl);
	printf("Контрольная сумма=%.4X\n\n",frame.checkSumm);
}