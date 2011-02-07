#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "extra/ortsTypes/ortsTypes.h"
#include "math/crc/crc16.h"
#include <stdlib.h> //DEBUG
#include <shield_types.h>
#include "statusFrame.h"

const WORD statusFrame::kPacketSize = sizeof(statusFrame_t);

statusFrame::statusFrame()
{
    memset(&frame, 0, statusFrame::kPacketSize);
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
    case 0: result=frame.BYTE_6.schieldOpened; break;
    case 1: result=frame.BYTE_7.shieldClosed; break;
    case 2: result=frame.BYTE_3.shieldStopped; break;
    }

    return result;
}

BYTE statusFrame::getHydroSystemStatus()
{
    // ...1 - poweron
    // ..1. - ready
    // .1.. - norm
    // 1... - alert

    BYTE result = 0;

    result  = frame.BYTE_3.hydrosys_poweron;
    result |= frame.BYTE_3.hydrosys_ready << 1;
    result |= frame.BYTE_3.hydrosys_norm  << 2;
    result |= frame.BYTE_3.hydrosys_alert << 3;

    return result;
}

BYTE statusFrame::getHydroSystemValues(BYTE num) // + filters
{
    BYTE result=0;
    
    switch (num)
    {
    case 0: result=frame.BYTE_4.hydrosys_pressure_norm; break;
    case 1: result=frame.BYTE_4.hydrosys_level_norm;    break;
    case 2: result=frame.BYTE_4.hydrosys_temp_norm;     break;
    case 3: result=frame.BYTE_5.hydrosys_filters_norm;  break;
    }
    return result;
}

bool statusFrame::isLocalControl()
{
    bool result=frame.BYTE_3.localControl;
    return result;
}

bool statusFrame::isRemoteControl()
{
    bool result=frame.BYTE_3.remoteControl;
    return result;
}

bool statusFrame::isCPControl()
{
    bool result = !(frame.BYTE_3.remoteControl|frame.BYTE_3.localControl);
    return result;
}

BYTE statusFrame::getSystemLinkStatus(LinkPoint_type link_point)
{
    // BUZ =0
    // AUGS=1
    // localPanel=2
    // KEGP=3
    
    BYTE result=0;
    
    switch (link_point)
    {
    case BUZ:  result=frame.BYTE_4.link_BUZ;        break;
    case AUGS: result=frame.BYTE_4.link_AUGS;       break;
    case PMU:  result=frame.BYTE_4.link_localPanel; break;
    case KEGP: result=frame.BYTE_4.link_KEGP;       break;
    };
    
    return result;
}

BYTE statusFrame::getFoldState(FoldDscr_type fold_descriptor)
{

    //it's right
    // xxxx xx00 - state is unstable
    // xxxx xx01 - closed
    // xxxx xx10 - opened
    // xxxx x1xx - stopped
    // xxxx x0xx - hydrocyls are coordinated
    // xxxx x1xx - hydrocyls aren't coordinated
    // xxx1 0xxx - hydrocyls are in coordinating state
    BYTE result = 0;
    // 0 - opened
    // 1 - closed
    // 2 - stopped

    //printf("get: num=%d,",num);

    switch (fold_descriptor)  {
      case LOWER_A:  {
          if (frame.BYTE_5.fold_lowA_close == 1)  {
              result = 1;
          }

          if (frame.BYTE_5.fold_lowA_open == 1)  {
              if (result == 0) result = 2;
              else result = 0;
          }

          if (frame.BYTE_9.fold_lowA_stop == 1)  {
              if (!frame.BYTE_5.fold_lowA_close && !frame.BYTE_5.fold_lowA_open)
                  result = 4;
              else result = 0;
          }

          BYTE GC_result = 0;
          if (!frame.BYTE_9.fold_lowA_inv_GC)  {
              GC_result = 1;
          }
          if (frame.BYTE_9.reseting)  {
              GC_result = 2;
          }

          result |= GC_result << 3;
          break;
      }
      case LOWER_B:  {
          if (frame.BYTE_5.fold_lowB_close == 1)  {
              result = 1;
          }

          if (frame.BYTE_5.fold_lowB_open == 1)  {
              if (result == 0) result = 2;
              else result = 0;
          }

          if (frame.BYTE_9.fold_lowB_stop == 1)  {
              if (!frame.BYTE_5.fold_lowB_close && !frame.BYTE_5.fold_lowB_open)
                  result = 4;
              else result = 0;
          }

          BYTE GC_result = 0;
          if (!frame.BYTE_9.fold_lowB_inv_GC)  {
              GC_result = 1;
          }
          if (frame.BYTE_9.reseting)  {
              GC_result = 2;
          }

          result |= GC_result << 3;

          break;
      }
      case UPPER:  {
          if (frame.BYTE_5.fold_up_close == 1)  {
              result = 1;
          }

          if (frame.BYTE_5.fold_up_open == 1)  {
              if (result == 0) result = 2;
              else result = 0;
          }

          if (frame.BYTE_9.fold_up_stop == 1)  {
              if (!frame.BYTE_5.fold_up_close && !frame.BYTE_5.fold_up_open)
                  result = 4;
              else result = 0;
          }

          BYTE GC_result = 0;
          if (!frame.BYTE_9.fold_up_inv_GC)  {
              GC_result = 1;
          }
          if (frame.BYTE_9.reseting)  {
              GC_result = 2;
          }

          result |= GC_result << 3;

          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::getFoldState(FoldDscr_type)/n");
          exit (0);     //DEBUG
      }
    }

    return result;
}

bool statusFrame::getFuncControlStatus()
{
    return frame.BYTE_6.funcControl_busy;
}

bool statusFrame::getLinkingStatus()
{
    return frame.BYTE_6.linkingStatus;
}

bool statusFrame::getRemotePanelStatus()
{
    return frame.BYTE_6.remotePanel_busy;
}

BYTE statusFrame::isUZ_locked(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A: return frame.BYTE_6.UZ1_locked;
      case LOWER_B: return frame.BYTE_6.UZ2_locked;
      case UPPER:   return frame.BYTE_6.UZ3_locked;
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::isUZ_locked(FoldDscr_type)/n");
          exit (0);     //DEBUG
      }
    }
}

BYTE statusFrame::isUZ_unlocked(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A: return frame.BYTE_7.UZ1_unlocked;
      case LOWER_B: return frame.BYTE_7.UZ2_unlocked;
      case UPPER:   return frame.BYTE_7.UZ3_unlocked;
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::isUZ_unlocked(FoldDscr_type)/n");
          exit (0);     //DEBUG
      }
    }
}

BYTE statusFrame::isUZ_busy(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A: return frame.BYTE_7.UZ1_busy; break;
      case LOWER_B: return frame.BYTE_7.UZ2_busy; break;
      case UPPER:   return frame.BYTE_7.UZ3_busy; break;
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::isUZ_busy(FoldDscr_type)/n");
          exit (0);
      }
    }
}

BYTE statusFrame::isUZ_manual(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A: return frame.BYTE_8.UZ1_manual; break;
      case LOWER_B: return frame.BYTE_8.UZ2_manual; break;
      case UPPER:   return frame.BYTE_8.UZ3_manual; break;
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::isUZ_manual(FoldDscr_type)/n");
          exit (0);
      }
    }
}

BYTE statusFrame::isUZ_alert(FoldDscr_type fold_descriptor)
{
    switch (fold_descriptor)  {
      case LOWER_A: return frame.BYTE_8.UZ1_alert; break;
      case LOWER_B: return frame.BYTE_8.UZ2_alert; break;
      case UPPER:   return frame.BYTE_8.UZ3_alert; break;
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::isUZ_alert(FoldDscr_type)/n");
          exit (0);
      }
    }
}


WORD statusFrame::getFoldPosition(FoldDscr_type fold_descriptor)
{
    WORD result=0;

    switch (fold_descriptor)  {
      case LOWER_A:  {
          result =  frame.foldA_value << 1;
          result |= frame.BYTE_10.fold_lowA_deg_b0 & 0x01;
          result |= (WORD)(frame.BYTE_10.fold_lowA_deg_b8 & 0x01) << 8;
          break;
      }
      case LOWER_B:  {
          result =  frame.foldB_value << 1;
          result |= frame.BYTE_10.fold_lowB_deg_b0 & 0x01;
          result |= (WORD)(frame.BYTE_10.fold_lowB_deg_b8 & 0x01) << 8;
          break;
      }
      case UPPER:  {
          result =  frame.foldU_value << 1;
          result |= frame.BYTE_10.fold_up_deg_b0 & 0x01;
          result |= (WORD)(frame.BYTE_10.fold_up_deg_b8 & 0x01) << 8;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          WORD statusFrame::getFoldPosition(FoldDscr_type)/n");
          exit (0);
      }
    }
    
    return result;
}

BYTE statusFrame::get_psa_sensors(FoldDscr_type fold_descriptor)                // num = 0,1,2 state = 00, 01, 10 ,11 (left.right)
{
    BYTE result=0;

    switch (fold_descriptor)  {
      case LOWER_A:  {
          result =  frame.BYTE_14.fold_lowA_psa_right << 1;
          result |= frame.BYTE_14.fold_lowA_psa_left;
          break;
      }
      case LOWER_B:  {
          result =  frame.BYTE_14.fold_lowB_psa_right << 1;
          result |= frame.BYTE_14.fold_lowB_psa_left;
          break;
      }
      case UPPER:  {
          result =  frame.BYTE_14.fold_up_psa_right << 1;
          result |= frame.BYTE_14.fold_up_psa_left;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::get_psa_sensors(FoldDscr_type)/n");
          exit (0);
      }
    }

    return result;
}

BYTE statusFrame::get_esa_sensors(FoldDscr_type fold_descriptor)   // num = 0,1,2 state = 00, 01, 10 ,11 (left.right)
{
    BYTE result=0;

    switch (fold_descriptor)  {
      case LOWER_A:  {
          result =  frame.BYTE_15.fold_lowA_esa_right << 1;
          result |= frame.BYTE_15.fold_lowA_esa_left;
          break;
      }
      case LOWER_B:  {
          result =  frame.BYTE_15.fold_lowA_esa_right << 1;
          result |= frame.BYTE_15.fold_lowA_esa_left;
          break;
      }
      case UPPER:  {
          result =  frame.BYTE_15.fold_lowA_esa_right << 1;
          result |= frame.BYTE_15.fold_lowA_esa_left;
          break;
      }
      default:  {
          printf("ERROR: Unavalible identificator of fold in\
          BYTE statusFrame::get_esa_sensors(FoldDscr_type)/n");
          exit (0);
      }
    }

    return result;
}

bool statusFrame::testCheckSumm()
{
    bool result=false;
    WORD chk=CRC16_eval((BYTE*)&frame, kPacketSize-sizeof(WORD));
    if (chk==frame.checkSumm) result=true;

    return result;
}

void statusFrame::decode(BYTE* array)
{
    memcpy(array, &frame, kPacketSize);
}

void statusFrame::encode(BYTE* array, size_t size)
{
    if (size != kPacketSize)
    memcpy(&frame, array+2, size);
}

void statusFrame::dbgPrint()
{
    printf("Управление от ПДУ......%d\n", frame.BYTE_3.remoteControl);
    printf("Управление от ПМУ......%d\n", frame.BYTE_3.localControl);
    printf("Гидросистема, Включена.%d\n", frame.BYTE_3.hydrosys_poweron);
    printf("Гидросистема, Готова...%d\n", frame.BYTE_3.hydrosys_ready);
    printf("Гидросистема, Норма....%d\n", frame.BYTE_3.hydrosys_norm);
    printf("Гидросистема, Авария...%d\n", frame.BYTE_3.hydrosys_alert);
    printf("СТОП укрытия...........%d\n\n",frame.BYTE_3.shieldStopped);

    printf("Связь с КЭГП.....................%d\n", frame.BYTE_4.link_KEGP);
    printf("Связь с ПМУ......................%d\n", frame.BYTE_4.link_localPanel);
    printf("Связь с АУГС.....................%d\n", frame.BYTE_4.link_AUGS);
    printf("Связь с БУЗ......................%d\n", frame.BYTE_4.link_BUZ);
    printf("Гидросистема, температура, норма.%d\n", frame.BYTE_4.hydrosys_temp_norm);
    printf("Гидросистема, уровень, норма.....%d\n", frame.BYTE_4.hydrosys_level_norm);
    printf("Гидросистема, давление, норма....%d\n\n",frame.BYTE_4.hydrosys_pressure_norm);

    printf("Створка нижняя А открыта.....%d\n", frame.BYTE_5.fold_lowA_open);
    printf("Створка нижняя А закрыта.....%d\n", frame.BYTE_5.fold_lowA_close);
    printf("Створка нижняя Б открыта.....%d\n", frame.BYTE_5.fold_lowB_open);
    printf("Створка нижняя Б закрыта.....%d\n", frame.BYTE_5.fold_lowB_close);
    printf("Створка верхняя  открыта.....%d\n", frame.BYTE_5.fold_up_open);
    printf("Створка верхняя  закрыта.....%d\n", frame.BYTE_5.fold_up_close);
    printf("Гидросистема, фильтры, норма.%d\n\n",frame.BYTE_5.hydrosys_filters_norm);

    printf("УЗ №1 заштырено.......................%d\n", frame.BYTE_6.UZ1_locked);
    printf("УЗ №2 заштырено.......................%d\n", frame.BYTE_6.UZ2_locked);
    printf("УЗ №3 заштырено.......................%d\n", frame.BYTE_6.UZ3_locked);
    printf("Работа контроллера ПДУ................%d\n", frame.BYTE_6.remotePanel_busy);
    printf("Стыковка..............................%d\n", frame.BYTE_6.linkingStatus);
    printf("Контроль функционирования исполняется.%d\n", frame.BYTE_6.funcControl_busy);
    printf("Укрытие открыто.......................%d\n\n", frame.BYTE_6.schieldOpened);

    printf("УЗ №1 исполнение.%d\n", frame.BYTE_7.UZ1_busy);
    printf("УЗ №2 исполнение.%d\n", frame.BYTE_7.UZ2_busy);
    printf("УЗ №3 исполнение.%d\n", frame.BYTE_7.UZ3_busy);
    printf("УЗ №1 отштырено..%d\n", frame.BYTE_7.UZ1_unlocked);
    printf("УЗ №2 отштырено..%d\n", frame.BYTE_7.UZ2_unlocked);
    printf("УЗ №3 отштырено..%d\n", frame.BYTE_7.UZ3_unlocked);
    printf("Укрытие закрыто..%d\n\n", frame.BYTE_7.shieldClosed);

    printf("УЗ №1 неисправность.%d\n", frame.BYTE_8.UZ1_alert);
    printf("УЗ №2 неисправность.%d\n", frame.BYTE_8.UZ2_alert);
    printf("УЗ №3 неисправность.%d\n", frame.BYTE_8.UZ3_alert);
    printf("УЗ №1 ручное........%d\n", frame.BYTE_8.UZ1_manual);
    printf("УЗ №2 ручное........%d\n", frame.BYTE_8.UZ2_manual);
    printf("УЗ №3 ручное........%d\n\n", frame.BYTE_8.UZ3_manual);

    printf("Створка верхня, рассогласование ГЦ...%d\n", frame.BYTE_9.fold_up_inv_GC);
    printf("Створка нижняя Б, рассогласование ГЦ.%d\n", frame.BYTE_9.fold_lowB_inv_GC);
    printf("Створка нижняя А, рассогласование ГЦ.%d\n", frame.BYTE_9.fold_lowA_inv_GC);
    printf("Режим согласования ГЦ................%d\n", frame.BYTE_9.reseting);
    printf("Створка нижняя А, Стоп...............%d\n", frame.BYTE_9.fold_lowA_stop);
    printf("Створка нижняя Б, Стоп...............%d\n", frame.BYTE_9.fold_lowB_stop);
    printf("Створка верхняя,  Стоп...............%d\n\n", frame.BYTE_9.fold_up_stop);

    printf("Створка А, угол = 0x%.4X\n",         getFoldPosition(LOWER_A));
    printf("Створка Б, угол = 0x%.4X\n",         getFoldPosition(LOWER_B));
    printf("Створка верхняя, угол = 0x%.4X\n\n", getFoldPosition(UPPER));

    printf("Нижняя А створка, путевой выключатель, правый =%d ", frame.BYTE_14.fold_lowA_psa_right);
    printf("левый=%d\n",frame.BYTE_14.fold_lowA_psa_left);
    printf("Нижняя Б створка, путевой выключатель, правый =%d ", frame.BYTE_14.fold_lowB_psa_right);
    printf("левый=%d\n",                                         frame.BYTE_14.fold_lowB_psa_left);
    printf("Верхняя створка, путевой выключатель,  левый  =%d ", frame. BYTE_14.fold_up_psa_right);
    printf("правый=%d\n\n",                                      frame.BYTE_14.fold_up_psa_left);


    printf("Нижняя А створка, концевой выключатель, правый =%d ",frame.BYTE_15.fold_lowA_esa_right);
    printf("левый=%d\n",                                         frame.BYTE_15.fold_lowA_esa_left);
    printf("Нижняя Б створка, концевой выключатель, правый =%d ",frame.BYTE_15.fold_lowB_esa_right);
    printf("левый=%d\n",                                         frame.BYTE_15.fold_lowB_esa_left);
    printf("Верхняя створка, концевой выключатель,  правый =%d ",frame.BYTE_15.fold_up_esa_right);
    printf("левый=%d \n\n",                                      frame.BYTE_15.fold_up_esa_left);


    printf("Контрольная сумма=0x%.4X\n",frame.checkSumm);
}                                           
