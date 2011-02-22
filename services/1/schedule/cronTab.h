/*
 * cronTab.h
 *
 *  Created on: Jan 22, 2011
 *      Author: tuborg
 */

#ifndef CRONTAB_H_
#define CRONTAB_H_


//  .---------------- minute (0 - 59)
//  |  .------------- hour (0 - 23)
//  |   |   .---------- day of month (1 - 31)
//  |   |   |   .------- month (1 - 12) OR jan,feb,mar,apr ...
//  |   |   |   |  .----- day of week (0 - 6) (Sunday=0 or 7)  OR sun,mon,tue,wed,thu,fri,sat
//  |   |   |   |  |
//  *   *   *   *  *  command to be executed

using namespace std;

class cronTab {
	char minute[3];
	char hour[3];
	char day_of_month[3];
	char month[3];
	char day_of_week[3];
	char command[255];

	fstream cronFile;

	int writePosition, readPosition;

public:
	errType setCommand(struct tm *ts, DWORD objID, DWORD nextObjID, DWORD finishTime, char* command);
	errType addToCronFile();
	int getFromCronFile();
	errType eraseEarlier();
	errType loadFromCronFileAt(int minute, int hour, int dayM, int month, int dayW);
	errType clearCronFile();

	cronTab();
	virtual ~cronTab();
};

#endif /* CRONTAB_H_ */
