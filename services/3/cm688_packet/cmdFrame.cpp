#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "extra/ortsTypes/ortsTypes.h"
#include "math/crc/crc16.h"
#include <shield_types.h>
#include "cmdFrame.h"
#include <stdlib.h> //DEBUG

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
    frame.hydroStation_start=1;
    frame.hydroStation_stop=0;
}

void cmdFrame::setHydroStationStop()
{
    frame.hydroStation_start=0;
    frame.hydroStation_stop=1;
}

void cmdFrame::setSchieldClose()
{
    frame.shield_close=1;
    frame.shield_open=0;
    frame.shield_stop=0;
}

void cmdFrame::setSchieldOpen()
{
    frame.shield_close=0;
    frame.shield_open=1;
    frame.shield_stop=0;
}

void cmdFrame::setSchieldStop()
{
    frame.shield_close=0;
    frame.shield_open=0;
    frame.shield_stop=1;
}

void cmdFrame::setCPControl()
{
    frame.remoteControl=0;
    frame.BYTE_4.fold_status.localControl=0;
}

void cmdFrame::setRemoteControl()
{
    frame.remoteControl=1;
    frame.BYTE_4.fold_status.localControl=0;
}

void cmdFrame::setLocalControl()
{
    frame.remoteControl=0;
    frame.BYTE_4.fold_status.localControl=1;
}

void cmdFrame::setFoldOpen(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A:  {
          frame.BYTE_4.fold_status.fold_lowA_open     = 1;
          frame.BYTE_4.fold_status.fold_lowA_close    = 0;
          frame.BYTE_5.fold_lowA_stop = 0;
          break;
      }
      case LOWER_B:  {
          frame.BYTE_4.fold_status.fold_lowB_open     = 1;
          frame.BYTE_4.fold_status.fold_lowB_close    = 0;
          frame.BYTE_5.fold_lowB_stop = 0;

          break;
      }
      case UPPER:  {
          frame.BYTE_4.fold_status.fold_up_open     = 1;
          frame.BYTE_4.fold_status.fold_up_close    = 0;
          frame.BYTE_5.fold_up_stop = 0;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          void cmdFrame::setFoldOpen(FoldDscr_type)");
          exit (0);
      }
    }

}

void cmdFrame::setFoldClose(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A:  {
          frame.BYTE_4.fold_status.fold_lowA_open     = 0;
          frame.BYTE_4.fold_status.fold_lowA_close    = 1;
          frame.BYTE_5.fold_lowA_stop = 0;
          break;
      }
      case LOWER_B:  {
          frame.BYTE_4.fold_status.fold_lowB_open     = 0;
          frame.BYTE_4.fold_status.fold_lowB_close    = 1;
          frame.BYTE_5.fold_lowB_stop = 0;

          break;
      }
      case UPPER:  {
          frame.BYTE_4.fold_status.fold_up_open     = 0;
          frame.BYTE_4.fold_status.fold_up_close    = 1;
          frame.BYTE_5.fold_up_stop = 0;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          cmdFrame::setFoldClose(FoldDscr_type)");
          exit (0);
      }
    }
}

void cmdFrame::setFoldStop(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A:  {
          frame.BYTE_4.fold_status.fold_lowA_open     = 0;
          frame.BYTE_4.fold_status.fold_lowA_close    = 0;
          frame.BYTE_5.fold_lowA_stop = 1;
          break;
      }
      case LOWER_B:  {
          frame.BYTE_4.fold_status.fold_lowB_open     = 0;
          frame.BYTE_4.fold_status.fold_lowB_close    = 0;
          frame.BYTE_5.fold_lowB_stop = 1;

          break;
      }
      case UPPER:  {
          frame.BYTE_4.fold_status.fold_up_open     = 0;
          frame.BYTE_4.fold_status.fold_up_close    = 0;
          frame.BYTE_5.fold_up_stop = 1;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          cmdFrame::setFoldStop(FoldDscr_type)");
          exit (0);
      }
    }
}

void cmdFrame::setStartFuncControl()
{
    frame.BYTE_5.funcCtrl=1;
}

WORD cmdFrame::setCheckSumm()
{
    frame.checkSumm=CRC16_eval((BYTE*)&frame, sizeof(cmdFrame_t)-sizeof(frame.checkSumm));
    return frame.checkSumm;
}


void cmdFrame::decode(BYTE** array)
{
    *array=new BYTE[sizeof(cmdFrame_t)];
    printf("DEBUG: sizeof(cmdFrame_t) = %lu\n", sizeof(cmdFrame_t));
    exit(0);    //DEBUG
    memcpy(*array, &frame, sizeof(cmdFrame_t));
}

void cmdFrame::encode(BYTE* array, size_t size)
{
    memcpy(&frame,array,size);
}

void cmdFrame::dbgPrint()
{
	printf("Коррекция ГЦ=%d\n\n",frame.hydroCyl_corr);
	
        printf("Укрытие закрыть=%d\n",frame.shield_close);
        printf("Укрытие открыть=%d\n",frame.shield_open);
        printf("Укрытие стоп=%d\n\n",frame.shield_stop);
	
        printf("ПУСК гидронасосной станции=%d\n",frame.hydroStation_start);
        printf("СТОП гидронасосной станции=%d\n\n",frame.hydroStation_stop);

        printf("Управление от ПДУ=%d\n",frame.remoteControl);
        printf("Управление от ПМУ=%d\n\n",frame.BYTE_4.fold_status.localControl);

        printf("Створка верхняя, закрыть=%d\n", frame.BYTE_4.fold_status.fold_up_close);
        printf("Створка верхняя, открыть=%d\n", frame.BYTE_4.fold_status.fold_up_open);
        printf("Створка нижняя Б, закрыть=%d\n", frame.BYTE_4.fold_status.fold_lowB_close);
        printf("Створка нижняя Б, открыть=%d\n", frame.BYTE_4.fold_status.fold_lowB_open);
        printf("Створка нижняя А, закрыть=%d\n", frame.BYTE_4.fold_status.fold_lowA_close);
        printf("Створка нижняя А, открыть=%d\n\n", frame.BYTE_4.fold_status.fold_lowA_open);

        printf("Створка верхняя, СТОП=%d\n",frame.BYTE_5.fold_up_stop);
        printf("Створка нижняя Б, СТОП=%d\n",frame.BYTE_5.fold_lowB_stop);
        printf("Створка нижняя А, СТОП=%d\n\n",frame.BYTE_5.fold_lowA_stop);
	
        printf("Контроль функционирования=%d\n",frame.BYTE_5.funcCtrl);
        printf("Контрольная сумма=%.4X\n\n",frame.checkSumm);
}
