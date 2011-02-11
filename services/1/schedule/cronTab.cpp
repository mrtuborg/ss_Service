/*
 * cronTab.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: tuborg
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

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

	//printf("cronTest.txt opened");
}

cronTab::~cronTab() {


}

errType cronTab::setCommand(struct tm *ts, DWORD objID, DWORD nextObjID, DWORD finishTime, char* newCommand){

	sprintf(minute,"%d",ts->tm_min);
	sprintf(hour, "%d", ts->tm_hour);
	sprintf(day_of_month, "%d", ts->tm_mday);
	sprintf(month, "%d", ts->tm_mon+1);
	sprintf(day_of_week, "%d", ts->tm_wday);


	sprintf(command, "ssProxy %s #objID=%d,nextObjID=%d,finishTime=%d", newCommand, objID, nextObjID, finishTime);

	return err_result_ok;
}

errType cronTab::addToCronFile()
{
	errType result=err_not_init;
	cronFile=fopen("cronTest.txt","a");
	if (cronFile) {
		printf("want to save: %s %s %s %s * %s\n", minute, hour, day_of_month, month, command);
		fprintf(cronFile, "%s\t%s\t%s\t%s\t*\t%s\n", minute, hour, day_of_month,month, command);
		fclose(cronFile);
		result=err_result_ok;
	}
	printf("want to exec: %s/%s (%s) %s:%s command: %s\n", day_of_month, month, day_of_week, hour, minute, command);

	return result;
}
