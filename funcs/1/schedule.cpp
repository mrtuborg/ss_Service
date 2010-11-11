#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
              
#include "../myTypes.h"
#include "cmd.h"
#include "job.h"
#include "schedule.h"

schedule::schedule()
{
}

schedule::~schedule()
{
}

errType schedule::addJob(job* jEntity)
{
    list <job*>::iterator iter, prev_iter;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
	if ((*iter)->get_dwTime()<jEntity->get_dwTime())
	{
	    prev_iter=iter;
	} else {
	    if ((*iter)->get_dwTime()==jEntity->get_dwTime())
	    {
		if (((*iter)->get_btServId()==jEntity->get_btServId()) && ((*iter)->get_btFuncId()==jEntity->get_btFuncId()))
		    {
			job_list.erase(iter);
			prev_iter=++iter;
		    }
	    } 
	    job_list.insert(prev_iter,jEntity);
	    iter=job_list.end();
	}
	
	
    }
}

errType schedule::removeAllJobsBefore(DWORD dwTime)
{
    list <job*>::iterator iter, prev_iter;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
	if ((*iter)->get_dwTime()<=dwTime)
	{  
	 job_list.erase(iter);
	}
	
	
    }
}

errType schedule::execJob()
{
    
}

errType schedule::checkAlarm()
{
    removeAllJobsBefore(0);
    execJob();
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

    printf("The file has %ld bytes\n", info.st_size);
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
    list <job*> tmp_jobs_list;
    BYTE* mappingArea;
    mappingOpen(mappingArea);
}

errType schedule::encode(BYTE* array)
{
    errType result=err_not_init;
    list <job*>::iterator iter;

    int offset=0;
    job* tmp_job;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
	tmp_job=new job;
	tmp_job->encode(array+offset);
	addJob(tmp_job);
	offset+=(*iter)->getLength();
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

