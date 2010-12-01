#include <stdio.h>
#include <string.h>
#include <list>
#include <signal.h>
#include <netinet/in.h>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "valueTrig.h"
#include "trigList.h"
#include "valuesRegistry.h"




trigList::trigList()
{
    triggersQuantity=0;
}

trigList::~trigList()
{

}

errType trigList::addValueTrigger(valueTrig* trigger)
{
    errType result=err_not_init;
    
    if (getValueTrigger(trigger->getTrigIndex())==0)
    {
        valueTriggers.push_back(trigger);
        result=err_result_ok;
    } else {
	printf("same trigger exists!\n");
	removeValueTrigger(trigger->getTrigIndex());
	valueTriggers.push_back(trigger);
	result=err_result_ok;
    }
    
   // filters_size+=filter->getSize();
    return result;
}

errType trigList::removeValueTrigger(WORD id)
{
    errType result=err_not_init;
    list<valueTrig*>::iterator iter;
    
    for(iter=valueTriggers.begin(); iter!=valueTriggers.end(); ++iter)
    {
	if ((*iter)->getTrigIndex()==id)
	{
	    //filters_size-=(*iter)->getSize();
	    valueTriggers.erase(iter);
	    result=err_result_ok;
	    break;
	}
    }
    
    return result;
}

valueTrig* trigList::getValueTrigger(WORD index)
{
    list<valueTrig*>::iterator iter;

    for(iter=valueTriggers.begin(); (*iter)->getTrigIndex()!=index; ++iter);

    return *iter;
}

errType trigList::triggersClock(WORD index, OrtsType currType, const void* currValue)
{
    errType result=err_not_init;
    valueTrig* trigger;

    bool set=false;

    if (quantity()==0) return result;

    trigger=getValueTrigger(index);

    if (trigger->triggerCheck(currType, currValue))
    {
	    // sending data

            trigger->triggerSet(index, currType, currValue);
    }
    
    return result;
}



WORD trigList::quantity()
{
    return valueTriggers.size();
}

void trigList::dbgPrint()
{
//    list<fltr*>::iterator iter;
//    printf ("Process filters have these PIDs: \n");
//    for(iter=procFilters.begin(); iter!= procFilters.end(); ++iter)
//    {
//	(*iter)->dbgPrint();
//	//printf("%d ",(*iter)->get_filter_id());
//	//printf("cmdline: %s\n", (*iter)->get_cmdline());
//    }
//    printf("\n");
}


errType trigList::decode (BYTE* dst_array)//, DWORD* dst_array_length)
{
    errType result=err_not_init;
//    int offset=0;
//    DWORD len;
//    list<fltr*>::iterator iter;
//
//    for(iter=procFilters.begin(); iter!= procFilters.end(); ++iter)
//    {
//	(*iter)->decode(dst_array+offset, &len);
//        offset+=len;
//    }
    
    return result;
}

errType trigList::clear()
{
    valueTriggers.clear();
    return err_result_ok;
}

errType trigList::encode (BYTE* src_array, DWORD src_array_len)
{
    errType result=err_not_init;
//    fltr* flt;
//    int offset=0;
//    valueTriggers.clear();
//
//    do {
//	flt=(fltr*)src_array+offset;
//	filters_size+=flt->getSize();
//	procFilters.push_back(flt);
//	offset+=sizeof(fltr);
//    } while (offset<src_array_len);
//
    return result;

}

