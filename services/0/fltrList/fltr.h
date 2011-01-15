/*
 * fltr.h
 *
 *  Created on: Dec 2, 2009
 *      Author: tuborg
 */

#ifndef FLTR_H_
#define FLTR_H_




class fltr {
private:
		WORD id;
		
		char* cmdline;
		int cpu_usage;
		DWORD mem_usage;
		
		BYTE _action;
public:

		fltr();
		~fltr();
		
		errType set_filter_id(WORD id);
		errType set_filter_cpu(int cpu);
		errType set_filter_mem(DWORD sz);
		
		errType set_filter_cmdline(char* cmdline, size_t size);

		char* get_cmdline();
		WORD get_filter_id();
		BYTE get_filter_cpu();
		DWORD get_filter_mem();

		void dbgPrint();
		
		errType decode(BYTE* array, DWORD* size);
		errType encode(BYTE* array);
		
		DWORD getSize();
		
		BYTE action();
		errType set_filter_action(BYTE);
};


#endif /* FLTR_H_ */
