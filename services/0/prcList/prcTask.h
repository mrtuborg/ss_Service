/*
 * prc_task.h
 *
 *  Created on: Dec 2, 2009
 *      Author: tuborg
 */

#ifndef PRC_TASK_H_
#define PRC_TASK_H_




class prcTask {
private:
		long pid;
		long ppid;
		
		long euid;
		
		char* cmdline;
		char* environment;
		int cpu_usage;
		char* state;
		long uptime;// lately as timeval (microseconds)
		long vsz;
		long rss;
public:

		prcTask();
		~prcTask();
		
		errType set_prc_ppid(long id);
		errType set_prc_pid(long id);
		errType set_prc_euid(long id);
		errType set_prc_cpu(int cpu);
		errType set_prc_vsz(long sz);
		errType set_prc_rss(long rss);
		errType set_prc_uptime(QWORD tottime);
		
		errType set_prc_pid(char* pid);
		errType set_prc_cpu(char* cpu);
		errType set_prc_state(char* state);
		errType set_prc_vsz(char* sz);
		errType set_prc_rss(char* rss);
		errType set_prc_uptime(char* tottime);

		errType set_prc_cmdline(char* cmdline);
		errType set_prc_environment(char* argv);

		char* get_cmdline();
		WORD get_prc_ppid();
		WORD get_prc_euid();
		WORD get_prc_pid();
		BYTE get_prc_cpu();
		char* get_prc_state();
		QWORD get_prc_uptime();
		DWORD get_prc_vsz();
		DWORD get_prc_rss();
		char* get_prc_environment();

		void dbgPrint();
		
		errType decode(BYTE* array, DWORD* size);
		errType encode(BYTE* array);
		
		DWORD getSize();
};


#endif /* PRC_TASK_H_ */
