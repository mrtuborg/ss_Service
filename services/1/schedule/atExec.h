/*
 * atExec.h
 *
 *  Created on: Jan 23, 2011
 *      Author: tuborg
 */

#ifndef ATEXEC_H_
#define ATEXEC_H_

class atExec {
	int year;
	int month;
	int monthDay;
	int hour;
	int minute;
	int second;
	char* command;
public:
	atExec();
	addJob(int,int,int,int,int,int, char*);
	getJobsList();
	removeJob();
	getJobInfo();
	virtual ~atExec();
};

#endif /* ATEXEC_H_ */
