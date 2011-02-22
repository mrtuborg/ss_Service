#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
              
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include "cronTab.h"
#include <schedule/job/job.h>
#include "schedule.h"

schedule::schedule()
{
	cronJob=new cronTab();
}

schedule::~schedule()
{
	delete cronJob;
}

errType schedule::addJob(job* jEntity){
    list <job*>::iterator iter, prev_iter, tmp_iter;
    rcsCmd *iter_cmd, *param_cmd;

    /// TODO: Make simpler
    iter=job_list.begin();
    //if (!(*iter)) return err_abort;
    int quantity=job_list.size();
    if (!quantity) job_list.push_back(jEntity);
    else {
    		for (iter=job_list.begin(); iter==job_list.end(); ++iter) {
    			if ((*iter)->get_dwTimeStart()<jEntity->get_dwTimeStart()) {
    				prev_iter=iter;
    			} else {
    				if ((*iter)->get_dwTimeStart()==jEntity->get_dwTimeStart()){
    					iter_cmd=(*iter)->cmd();
    					param_cmd=jEntity->cmd();
    					if (((*iter)->get_btServId()==jEntity->get_btServId()) && (iter_cmd->get_func_id()==param_cmd->get_func_id())){
    						tmp_iter=iter;
    						job_list.erase(tmp_iter);
    						prev_iter=++iter;
    					}
    					job_list.insert(prev_iter,jEntity);
    					iter=job_list.end();
    				}
    			}
    		}
    }

    return err_result_ok;
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

errType schedule::execJob()
{
		return err_not_init;
}

errType schedule::checkAlarm()
{
    removeAllJobsBefore(0);
    execJob();
    return err_result_ok;
}


errType schedule::mappingOpen(BYTE* mapping)
{
    errType result=err_not_init;
    char sched_filename[255];
    struct stat info;

    sprintf(sched_filename,"schedule_%d.dat",id);
    
    fd = open(sched_filename, O_RDWR);
    if (fd == -1) {
	//error_message(__FILE__, __LINE__, "'open' failed ");
	return err_result_error;
    }

    // find out the size of the file in bytes
    // we need the size, because we cannot write past the end of the file
    // so, we need to make sure not to write at an offset larger than 'info.st_size'
    if (fstat(fd, &info) == -1) {
	//error_message(__FILE__, __LINE__, "'fstat' failed ");
	return err_result_error;
    }

    printf("The file has %d bytes\n", (DWORD) info.st_size);
    if (info.st_size == 0) {
	fprintf(stderr, "We cannot map a file with size 0\n");
	return err_result_error;
    }
    // TODO
	mapping = (BYTE*) mmap(NULL, info.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (mapping == MAP_FAILED) {
		perror("mmap:");
		return err_result_error;
	}
	else fprintf(stderr, "Successful mapping at the address %p\n", mapping);
	
	memcpy(mapping, "hello", 5);
	
	if (msync(mapping, info.st_size, MS_SYNC) == -1) {
		perror("msync:");
		return err_result_error;
	}

    return result;
}


errType schedule::mappingClose()
{
    errType result=err_not_init;
    
    // close the file
	if (close(fd) == -1) {
		//error_message(__FILE__, __LINE__, "'close' failed ");
		return err_result_error;
	}

    
    return result;
}



errType schedule::update()
{
	list <job*>::iterator iter;
	for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
	{
		writeCronTab(*iter);
	}
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

errType schedule::writeCronTab(job* newJob)
{
	errType result=err_result_ok;
	struct tm  *ts;
	time_t timeStart = newJob->get_dwTimeStart();
	/// 1. Define start time for cron task
	ts = localtime(&timeStart);
	/// 2. Filling cron task
	WORD cmdLen=newJob->cmd()->getCmdLength();
	char *ipaddr;//[255];
	char uport[255];

	struct in_addr in;
	in.s_addr=newJob->get_dwServiceIPaddr();
	ipaddr=inet_ntoa(in);

	sprintf(uport, "%d", newJob->get_wServiceUdpPort());

	char *cmd, *tmp_cmd; // string for writing to cron
	cmd=new char[cmdLen*3+strlen(ipaddr)+strlen(uport)+3];
	sprintf(cmd,"-u %s:%s ", ipaddr, uport);
	tmp_cmd=cmd;
	cmd=cmd+strlen(ipaddr)+strlen(uport);
	BYTE* array; // temporary array for decoding cmd
	array=new BYTE[cmdLen];
	newJob->cmd()->decode(array);
	for (int i=0; i<cmdLen; i++) {
		sprintf(cmd+i*3,"%.2X ",array[i]);
	}
	cmd[cmdLen*3]=0;
	cmd=tmp_cmd;
	cronJob->setCommand(ts, newJob->get_dwObjId(), newJob->get_dwNextObjId(), newJob->get_dwTimeEnd(), cmd);
	cronJob->addToCronFile();

	int pos=0;

	do {
		pos=cronJob->getFromCronFile();
	} while (pos>0);

	delete []cmd;
	delete []array;
	return result;
}

