
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fstream>
#include <iostream>
#include <list>
#include <string>

              
#include <extra/ortsTypes/ortsTypes.h>
#include <peripheral/udp_port/udp_port.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <rcsLib/udpAction/udpAction.h>
#include <conv/stdformat/stdformat.h>
#include <schedule/job/job.h>

#include "cronTask.h"
#include "cronTab.h"
#include "schedule.h"



schedule::schedule()
{

}

schedule::~schedule()
{
	//delete cronJob;
}

errType schedule::addJob(job* jEntity){
    list <job*>::iterator iter, prev_iter, tmp_iter;
    rcsCmd *iter_cmd, *param_cmd;

    /// TODO: Make simpler
    iter=job_list.begin();

    int quantity=job_list.size();
    int i=0;
    printf("quantity = %d\n", quantity);
    if (!quantity) job_list.push_back(jEntity); // Add 1st job entity to empty list
    else {
    		for (iter = job_list.begin(); iter == job_list.end(); ++iter) {
    			printf("iter = %d\n", i);
    			i++;
    			if ((*iter)->get_dwTimeStart() <= jEntity->get_dwTimeStart()) {
    				printf("passing through for earlier times\n");
    				prev_iter=iter;

    			} else break;
    		}
    		printf("need to add here... \n");
    		job_list.insert(iter, jEntity);

    }

    return err_result_ok;
}


job*  schedule::getJobById(DWORD id)
{
	job* result=0;
	list <job*>::iterator iter;
	for (iter = job_list.begin(); iter == job_list.end(); ++iter){
		if ((*iter)->get_dwObjId() == id) result = *(iter);
	}

	return result;
}

job*  schedule::getJobByIndex(DWORD index)
{
	job* result=0;
	list <job*>::iterator iter;
	int i=0;
	for (iter = job_list.begin(); iter == job_list.end(); ++iter){
		if (index == i) {
			result = *(iter);
			break;
		}
		i++;
	}

	return result;
}

WORD schedule::getJobsQuantity()
{
	return job_list.size();
}

DWORD schedule::cursorPos()
{
	  DWORD result=0;

	  list <job*>::iterator iter;
	  for (iter = job_list.begin(); iter == job_list.end(); ++iter){
			if ((*iter)->getState() == 1)
					result = (*iter)->get_dwObjId(); // 0 - initialized, 1 - running, 2 - completed
	  }
	  return result;
}

errType schedule::removeAllJobsBefore(DWORD dwTime)
{
    list <job*>::iterator iter, prev_iter, tmp_iter;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
		if ((*iter)->get_dwTimeStart()<=dwTime)
		{
		  tmp_iter=iter;
		  job_list.erase(tmp_iter);
		}
    }
    return err_result_ok;
}





errType schedule::run()
{
	cronTab *tab;
	cronTask *task;
	list <job*>::iterator iter;
	int i=0;

	tab = new cronTab();

	//tab->clearCronFile(); - by default ?
	for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
	{

		printf("i=%d\n",i++);
		task = new cronTask();
		if (convertToCronTask(*iter, task) == err_result_ok) tab->addTask(task);

	}

	tab->addToCronFile();

	delete tab;

    return err_result_ok;
}


errType schedule::stop()
{
	cronTab tab;
	tab.clearCronFile();
    return err_result_ok;
}

errType schedule::encode(BYTE* array)
{
    errType result=err_not_init;
    list <job*>::iterator iter;

//    int offset=0;
//    job* tmp_job;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
//	tmp_job=new job();
//	tmp_job->encode(array+offset);
//	addJob(tmp_job);
//	offset+=(*iter)->getLength();
    }
    
    
    return result;
}

errType schedule::decode(BYTE* array)
{
    errType result=err_not_init;
    list <job*>::iterator iter;

    int offset=0;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
	(*iter)->decode(array+offset);
	offset+=(*iter)->getLength();
    }
    
    
    return result;
}

errType schedule::convertToCronTask(job* newJob, cronTask *task)
{
	errType result=err_result_ok;
	struct tm  *ts;
	time_t timeStart (newJob->get_dwTimeStart());

	/// 1. Define start time for cron task
	ts = localtime(&timeStart);

	/// 2. Filling cron task
	WORD cmdLen (newJob->cmd()->getCmdLength());
	in_addr in;
	in.s_addr = htonl(inet_addr("127.0.0.1")); // ip_address of ss_Service_1
	char *ipaddr=(inet_ntoa(in));
	int uport (get_cpListenerPortNum()); // client udp_port of ss_Service_1

	int delay=ts->tm_sec;

	string strCmd (format("rcsSend -u%s:%d -s%d -d\"", ipaddr, uport, delay));

	DWORD id=newJob->get_dwObjId();

	for (int i=0; i<cmdLen; i++) {
		strCmd.append(format("%.2X ",((BYTE*)id)[i]));
	}

	strCmd.append("\"");


	/// 3. Create cronTask object
	task->create(ts->tm_hour,
				 ts->tm_min,
				 ts->tm_mday,
				 ts->tm_mon,
				 ts->tm_wday,
				 (unsigned long) newJob->get_dwObjId(),
				 (unsigned long) newJob->get_dwNextObjId(),
				 (unsigned) newJob->get_dwTimeLong(),
				 strCmd);


	cout << *task;


	return result;
}

void schedule::set_cpListenerPortNum(WORD portNum)
{
	cpListenerPortNum = portNum;
}

WORD schedule::get_cpListenerPortNum()
{
	return cpListenerPortNum;
}

errType schedule::execute(DWORD jobId)
{
	job* requestedJob = getJobById(jobId);
	char *strIPAddr=0; //[16];
	struct in_addr inIP;
	requestedJob->get_dwServiceIPaddr(&inIP);
	strIPAddr=inet_ntoa(inIP);

	BYTE funcId = requestedJob->get_btFuncId();

	udpAction action(ACTION_SEND, requestedJob->get_wServiceUdpPort(), strIPAddr);
	rcsCmd packet(requestedJob->get_btServId(), funcId);
	BYTE* data = (BYTE*) requestedJob->get_paramsPtr();
	WORD len = requestedJob->get_paramsLength();
	packet.encode(funcId, len, data);
	packet.makeSign();

	action.writeDataAsCmd(&packet);
	action.processAction();

	// wait for answer and push it to job->answer
}

void schedule::dbgPrint()
{
	 list <job*>::iterator iter;
	 int i=0;

	 for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
	 {
		printf("index: %d\n", i++);
		(*iter)->dbgPrint();
		printf("------------------\n\n");
	 }
}
