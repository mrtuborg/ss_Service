#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
              
#include <extra/ortsTypes/ortsTypes.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include "cronTab.h"
#include "job.h"
#include "schedule.h"

schedule::schedule()
{
}

schedule::~schedule()
{
}

errType schedule::addJob(job* jEntity){
    list <job*>::iterator iter, prev_iter, tmp_iter;
    rcsCmd *iter_cmd, *param_cmd;

    printf("pt1\n");
    /// TODO: Make simpler
    iter=job_list.begin();
    //if (!(*iter)) return err_abort;
    int quantity=job_list.size();
    if (!quantity) job_list.push_back(jEntity);
    else {
    		for (iter=job_list.begin(); iter==job_list.end(); ++iter) {
    			if ((*iter)->get_wTimeStart()<jEntity->get_wTimeStart()) {
    				prev_iter=iter;
    			} else {
    				if ((*iter)->get_wTimeStart()==jEntity->get_wTimeStart()){
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
    quantity=job_list.size();
    printf("job_list size=%d\n",quantity);
    return err_result_ok;
}

errType schedule::removeAllJobsBefore(WORD wTime)
{
    list <job*>::iterator iter, prev_iter, tmp_iter;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
		if ((*iter)->get_wTimeStart()<=wTime)
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
		(*iter)->writeCronTab();
	}
    return err_result_ok;
}

errType schedule::encode(BYTE* array)
{
    errType result=err_not_init;
    list <job*>::iterator iter;

    int offset=0;
    job* tmp_job;
    
    for (iter=job_list.begin(); iter!=job_list.end(); ++iter)
    {
	//-tmp_job=new job;
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

