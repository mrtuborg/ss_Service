/*
 * cronTab.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: tuborg
 */
#include <stdio.h>
#include <string>
#include <time.h>
#include <fstream>

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
	cronFile.open("cronFile.txt", ios::in);

	cronFile.seekp(0, ios::end);
	writePosition=cronFile.tellp();

	cronFile.seekg(0,ios::beg);
	readPosition=cronFile.tellg();

	printf("cronFile writePosition=%d\n", writePosition);
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

//errType cronTab::getCommand(struct tm *ts, DWORD objID, DWORD nextObjID, DWORD finishTime, char* newCommand){
//
//	//fscanf();
//	sprintf(minute,"%d",ts->tm_min);
//	sprintf(hour, "%d", ts->tm_hour);
//	sprintf(day_of_month, "%d", ts->tm_mday);
//	sprintf(month, "%d", ts->tm_mon+1);
//	sprintf(day_of_week, "%d", ts->tm_wday);
//
//
//	sprintf(command, "ssProxy %s #objID=%d,nextObjID=%d,finishTime=%d", newCommand, objID, nextObjID, finishTime);
//
//	return err_result_ok;
//}

errType cronTab::addToCronFile()
{
	errType result (err_not_init);
	cronFile.open("cronTest.txt",ios::out | ios::app);
	if (cronFile.is_open()) {
		printf("want to save: %s %s %s %s * %s\n", minute, hour, day_of_month, month, command);
		cronFile<<minute<<"\t"<<hour<<"\t"<<day_of_month<<"\t"<<month<<"\t"<<command<<"\r\n";

		//fprintf(cronFile, "%s\t%s\t%s\t%s\t*\t%s\n", minute, hour, day_of_month,month, command);
		writePosition=cronFile.tellp();
		cronFile.close();

		result=err_result_ok;
	}

	printf("want to exec: %s/%s (%s) %s:%s command: %s\n", day_of_month, month, day_of_week, hour, minute, command);
	printf("writePosition = %d\n",writePosition);
	return result;
}

int cronTab::getFromCronFile()
{
	errType result (err_not_init);
	cronFile.open("cronTest.txt", ios::in);
	int pos=0;


	if (cronFile) {

		cronFile.seekg(readPosition, ios::beg);
		//-fscanf(cronFile, "%s\t%s\t%s\t%s\t*\t%s\n", minute, hour, day_of_month,month, command);
		cronFile>>command;
		//fscanf(cronFile,"%s\n", command);
		printf("readed: %s %s %s %s * %s\n", minute, hour, day_of_month, month, command);
		readPosition=cronFile.tellg();

		if (readPosition>=writePosition) {
			readPosition=0;
			pos=-1;
		}
		printf("readPosition=%d\n",readPosition);
		cronFile.close();
		result=err_result_ok;
	}
	pos=readPosition;
	//printf("want to exec: %s/%s (%s) %s:%s command: %s\n", day_of_month, month, day_of_week, hour, minute, command);

	return pos;
}
