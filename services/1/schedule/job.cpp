#include <stdio.h>
#include <string.h>
#include <time.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include "cronTab.h"
#include "job.h"


job::job(DWORD id)
{
	jobReference = new job_type();
	jobReference->objId=id;
	cronJob=new cronTab();
	jobEntity=new rcsCmd();
}

job::~job()
{
	delete cronJob;

}

void job::encode(BYTE *data)
{
    WORD jobReferenceSize=sizeof(job_type);

    memcpy(jobReference, data, jobReferenceSize);
    jobEntity->encode(data+jobReferenceSize);
}

void job::decode(BYTE *data)
{

    WORD jobReferenceSize=sizeof(job_type);
    memcpy(data,jobReference, jobReferenceSize);
    jobEntity->decode(data+jobReferenceSize);
}


WORD job::getLength()
{
    return sizeof(job_type)+jobEntity->getCmdLength();
}

void job::dbgPrint()
{                       
    printf("object Id = %d\n", jobReference->objId);
    printf("next object Id = %d\n", jobReference->nextObjId);
    printf("start time = %d\n", jobReference->timeStart);
    printf("finish time = %d\n", jobReference->timeEnd);
    printf("calling service = %d\n", jobReference->service_id);
    printf("command:\n");
    jobEntity->dbgPrint();
}

errType job::set_dwNextJobID(DWORD id)
{
		jobReference->nextObjId=id;
		return err_result_ok;
}

errType job::set_dwStartTime(DWORD time)
{
		jobReference->timeStart=time;
		return err_result_ok;
}

errType job::set_dwFinishTime(DWORD time)
{
		jobReference->timeEnd=time;
		return err_result_ok;
}

errType job::set_btServiceId(BYTE id)
{
		jobReference->service_id=id;
		return err_result_ok;
}

errType	job::setJobCmd(BYTE func_id, WORD param_len, void* args)
{
		errType result=err_result_ok;
		jobEntity->encode(func_id, param_len, args);
		jobEntity->makeSign();
		return result;
}

errType	job::setJobCmd(BYTE* cmd)
{
		errType result=err_result_ok;

		jobEntity->encode(cmd);
		jobEntity->makeSign();

		return result;
}


DWORD job::get_dwObjId()   { return jobReference->objId;    }
DWORD job::get_dwNextObjId()   { return jobReference->nextObjId;}
WORD job::get_dwTimeStart() { return jobReference->timeStart; }
WORD job::get_dwTimeEnd()   { return jobReference->timeEnd;   }
BYTE job::get_btServId()   { return jobReference->service_id;}
BYTE job::get_btFuncId()   { return jobEntity->get_func_id();}
const void* job::get_paramsPtr(DWORD offset) { return jobEntity->get_func_paramsPtr(offset);}
WORD job::get_paramsLength(){return jobEntity->get_func_paramsLength();}
rcsCmd *job::cmd()    { return jobEntity;       }

//const char*job::cronTabString()
//{
  //const char* string[]={"uninitilalized"};
  // timeStart - seconds starting from current month
  // 3600 seconds per hour
  // 86400 seconds per day
  //
 /* time_t now;
  struct tm *d;
  d=localtime(&now)
  int8_t currMonth=d->tm_mon;
  minutes=timeStart%60
  hours=minutes%60;
  dayMonth=hours%24;
  month=dayMonth%30;
*/
  //int8_t minutes;
  //int8_t hours;
  //int8_t dayMonths;
  //int8_t month;

  //sprintf(string, "%d\t%d\t%d\t%d\t*\t%s",minutes,hours,dayMonth,month,cmdString);

  //return 0;//string;
//}
/*
BYTE* get_strTime()
{
    double day=(double)time_ms / 86400000); // 86400000 ms in 1 day
    double hh = day
}
*/

errType job::writeCronTab()
{
	errType result=err_result_ok;
	struct tm  *ts;
	time_t timeStart = jobReference->timeStart;
	/// 1. Define start time for cron task
	ts = localtime(&timeStart);
	/// 2. Filling cron task
	WORD cmdLen=jobEntity->getCmdLength();
	char* cmd; // string for writing to cron
	cmd=new char[cmdLen];

	BYTE* array; // temporary array for decoding cmd
	array=new BYTE[cmdLen];
	jobEntity->decode(array);
	for (int i=0; i<cmdLen; i++) {
		sprintf(cmd+i*3,"%.2X ",array[i]);
	}
	cmd[cmdLen*3]=0;

	cronJob->setCommand(ts->tm_min, ts->tm_hour, ts->tm_mday, ts->tm_mon, ts->tm_wday, jobReference->objId, jobReference->nextObjId, jobReference->timeEnd, cmd);
	cronJob->addToCronFile();

	delete []cmd;
	delete []array;
	return result;
}

