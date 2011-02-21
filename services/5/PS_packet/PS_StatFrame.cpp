#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <extra/ortsTypes/ortsTypes.h>
#include "PS_StatFrame.h"

char strPSmode[7][255]={                                                           
    {"ОТКЛ"},                                                                      
    {"Ручной"},                                                                    
    {"Программный"},                                                               
    {"КН"},                                                                        
    {"АС"},
    {"Поиск"},
    {"ЗВВ"}};                                                                      
                                                                                                               
char strPSfield[2][255]={                                                          
    {"Широкий"},                                                                   
    {"Узкий"}};                                                                             
                                                                                                                                                                                                              
char strPSdiscrete[7][255]={                                                       
    {"1 сек"},                                                                     
    {"5 сек"},                                                                     
    {"15 сек"},                                                                    
    {"1 мин"},                                                                     
    {"2 мин"},                                                                     
    {"5 мин"}};
    
PS_StatFrame::PS_StatFrame()
{
    frame = new PS_StatFrameType;
}

PS_StatFrame::~PS_StatFrame()
{
    delete frame;
}

errType PS_StatFrame::decode(BYTE** array)                                            
{                                                                                  
    memcpy(*array,frame,sizeof(PS_StatFrameType));
    return err_result_ok;
}                                                                                  
                                                                                   
errType PS_StatFrame::encode(BYTE* array, DWORD size)                                 
{                                                                                  
    memcpy(frame,"\x0", sizeof(PS_StatFrameType));
    memcpy(frame,array, size);
    return err_result_ok;
} 

DWORD PS_StatFrame::currTime()
{
    return frame->currTime;
}

DWORD PS_StatFrame::astralTime()
{
    return frame->astralTime;
}

PS_modeType PS_StatFrame::currMode()
{
    return frame->currMode;
}

double PS_StatFrame::DOS_Azimuth()
{
    return frame->DOS_Azimuth;
}

double PS_StatFrame::DOS_Elevation()
{
    return frame->DOS_Elevation;
}

double PS_StatFrame::PRG_Azimuth()
{
    return frame->PRG_Azimuth;
}

double PS_StatFrame::PRG_Elevation()
{
    return frame->PRG_Elevation;
}

double PS_StatFrame::AzimuthBearing()
{
    return frame->AzimuthBearing;
}

double PS_StatFrame::ElevationBearing()
{
    return frame->ElevationBearing;
}

double PS_StatFrame::PRG_AzimuthVelocity()
{
    return frame->PRG_AzimuthVelocity;
}

double PS_StatFrame::PRG_ElevationVelocity()
{
    return frame->PRG_ElevationVelocity;
}

double PS_StatFrame::CurrentAzimuthVelocity()
{
    return frame->CurrentAzimuthVelocity;
}

double PS_StatFrame::CurrentElevationVelocity()
{
    return frame->CurrentElevationVelocity;
}

double PS_StatFrame::AzimuthBinding()
{
    return frame->AzimuthBinding;
}

double PS_StatFrame::ElevationBinding()
{
    return frame->ElevationBinding;
}

double PS_StatFrame::AzimuthCorrection()
{
    return frame->AzimuthCorrection;
}

double PS_StatFrame::ElevationCorrection()
{
    return frame->ElevationCorrection;
}

QWORD PS_StatFrame::currTimeCorrection()
{
    return frame->currTimeCorrection;
}

BYTE PS_StatFrame::equipmentState()
{
    return frame->equipmentState;
}

fieldType PS_StatFrame::field()
{
    return frame->field;
}

discreteType PS_StatFrame::discrete()
{
    return frame->discrete;
}

void PS_StatFrame::dbgPrint()
{
    printf("Режим: %s\n",strPSmode[frame->currMode]);                                                      
    printf("Дата: %d/%d/%d",frame->day,frame->month,frame->year);                                                 
    printf("ДМВ: %d\n",frame->currTime);
    printf("Звёздное время: %d\n",frame->astralTime);
    printf("Азимут в режиме ДОС: %f\n",frame->DOS_Azimuth);                                                        
    printf("Угол места в режиме ДОС: %f\n",frame->DOS_Elevation);                                                      
    printf("Азимут программного режима: %f\n",frame->PRG_Azimuth);                                                        
    printf("Угол места программного режима: %f\n",frame->PRG_Elevation);                                                      
    printf("Поправка по азимуту: %f\n",frame->AzimuthCorrection);                                                  
    printf("Поправка по углу места: %f\n",frame->ElevationCorrection);                                                
    printf("Привязка по азимуту: %f\n",frame->AzimuthBearing);                                                     
    printf("Привязка по углу места: %f\n",frame->ElevationBearing);                                                
    printf("Установившееся прогр. значение скорости изменения по азимуту: %f\n",frame->PRG_AzimuthVelocity);                                                
    printf("Установившееся прогр. значение скорости изменения по углу места: %f\n",frame->PRG_ElevationVelocity);                                              
    printf("Установившееся факт. значение скорости изменения по азимуту: %f\n",frame->CurrentAzimuthVelocity);                                             
    printf("Установившееся факт. значение скорости изменения по углу места: %f\n",frame->CurrentElevationVelocity);                                           
    printf("Используемая привязка по азимуту: %f\n",frame->AzimuthBinding);                                                     
    printf("Используемая привязка по углу места: %f\n",frame->ElevationBinding);                                                   
    printf("Используемая поправка по времени: %llu,сек\n",frame->currTimeCorrection);
    printf("Вектор состояния оборудования: 0x%.2X\n", frame->equipmentState);                                                     
    printf("Используемое поле: %s\n",strPSfield[frame->field]);                                                     
    printf("Дискрета: %s\n", strPSdiscrete[frame->discrete]);
}
