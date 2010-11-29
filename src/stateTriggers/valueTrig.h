/*
 * valueTrig.h
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */

#ifndef VALUETRIG_H_
#define VALUETRIG_H_


typedef struct trigDef_type{
	    void* value;
	    void* oldValue;
	 OrtsType type;
	inaddr_in recepient;
} trigDef_type;

class valueTrig{
private:
		trigDef_type *trigDef;
public:

		valueTrig();
		~valueTrig();
		
		errType setTrigValuePtr(WORD id);
		
		void* getTrigValuePtr();
		void* getTrigOldValuePtr();
		
		void dbgPrint();
		
		//errType decode(void* array, DWORD* size);
		//errType encode(void* array);
		//DWORD getSize();
		BYTE action();
		
		//errType set_filter_action(BYTE);
};


#endif /* STTRIG_H_ */
