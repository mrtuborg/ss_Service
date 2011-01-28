#include <stdio.h>
#include <string.h>
#include <time.h>
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include "cronTab.h"
#include "job.h"


job::job()
{
	cronJob=new cronTab();
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


DWORD job::get_dwObjId()   { return jobReference->objId;    }
DWORD job::get_dwNextObjId()   { return jobReference->nextObjId;}
WORD job::get_wTimeStart() { return jobReference->timeStart; }
WORD job::get_wTimeEnd()   { return jobReference->timeEnd;   }
BYTE job::get_btServId()   { return jobReference->service_id;}
BYTE job::get_btFuncId()   { return jobEntity->get_func_id();}
const void* job::get_paramsPtr(DWORD offset) { return jobEntity->get_func_paramsPtr(offset);}
DWORD job::get_paramsLength(){return jobEntity->get_func_paramsLength();}
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
	double seconds = jobReference->timeStart;

	time_t t1;
	struct tm t2;


//	double minutes = seconds / 60;
//	double hours = minutes / 60;
//	double days = hours / 24;
//	double weeks = days / 7;
//	double months = weeks / 52;
//	double years = months / 12;

//	cronJob->setCommand(btMinute, btHour, btDayM, btMonth, btDayW, cmd);
	cronJob->addToCronFile();
}
