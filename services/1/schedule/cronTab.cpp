/*
 * cronTab.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: tuborg
 */
#include <stdio.h>
#include <string.h>

#include <extra/ortsTypes/ortsTypes.h>
#include "cronTab.h"

//  .---------------- minute (0 - 59)
//  |  .------------- hour (0 - 23)
//  |   |   .---------- day of month (1 - 31)
//  |   |   |   .------- month (1 - 12) OR jan,feb,mar,apr ...
//  |   |   |   |  .----- day of week (0 - 6) (Sunday=0 or 7)  OR sun,mon,tue,wed,thu,fri,sat
//  |   |   |   |  |
//  *   *   *   *  *  command to be executed

cronTab::cronTab() {

}

cronTab::~cronTab() {
	// TODO Auto-generated destructor stub
}

errType cronTab::setCommand(int btMinute, int btHour, int btDayM, int btMonth, int btDayW, char* newCommand){
	sprintf(minute,"%d",btMinute);
	sprintf(hour, "%d", btHour);
	sprintf(day_of_month, "%d", btDayM);
	sprintf(month, "%d", btMonth);
	sprintf(day_of_week, "%d", btDayW);

	command[256]=0;
	memcpy(command, newCommand,255);
	return err_result_ok;
}

errType cronTab::getString(char* strOut)
{
		sprintf(strOut, "%s\t%s\t%s\t%s\t%s\t%s", minute, hour, day_of_month,month,day_of_week,command);
		return err_result_ok;
}

errType cronTab::addToCronFile()
{


}
