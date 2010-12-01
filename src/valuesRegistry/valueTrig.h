/*
 * valueTrig.h
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */

#ifndef VALUETRIG_H_
#define VALUETRIG_H_


typedef struct trigDef_type{
                       WORD index;
	               void *oldValue;
	 struct sockaddr_in recepient;
} trigDef_type;

class valueTrig{
private:
		trigDef_type *trigDef;
public:

		valueTrig();
		~valueTrig();
		
		errType setTrigValuePtr(WORD id);
		

		const void* getTrigOldValuePtr();
		WORD getTrigIndex();
		
		void dbgPrint();
		bool triggerCheck(OrtsType currType, const void *currValue);
		errType triggerSet(WORD index, OrtsType type, const void* valuePtr);

		//errType decode(void* array, DWORD* size);
		//errType encode(void* array);
		DWORD getSize(OrtsType type);
		//BYTE action();
		
		//errType set_filter_action(BYTE);
};


#endif /* STTRIG_H_ */
