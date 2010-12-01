/*
 * valueRegistry.cpp
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */
#include <list>
#include "../../../rcsLib/ortsTypes/ortsTypes.h"
#include "valuesRegistry.h"

valuesRegistry::valuesRegistry(){ }
valuesRegistry::~valuesRegistry(){ }

errType valuesRegistry::addToRegistry(const void* valuePtr, OrtsType type, const char* name, size_t nameLen)
{
        errType result=err_not_init;
        registry_record *record=new registry_record;

        record->name=name;
        record->type=type;
        record->valuePtr=valuePtr;
        record->index=registry.size();

        if (getValuePtr(searchIndexByName(name,nameLen))==0)
        {
            registry.push_back(record);
            result=err_result_ok;
        } else {
            printf("same record exists!\n");
            result=removeFromRegistry(searchIndexByName(name,nameLen));
            if (result==err_result_ok) registry.push_back(record);

        }
        return result;
}

errType valuesRegistry::removeFromRegistry(WORD index)
{
        errType result=err_not_init;
        list<registry_record*>::iterator iter;

        for(iter=registry.begin(); iter!= registry.end(); ++iter)
        {
            if ((*iter)->index==index)
            {
                registry.erase(iter);
                result=err_result_ok;
                break;
            }
        }
        return result;
}

WORD valuesRegistry::searchIndexByName(const char* name, size_t size)
{
     WORD result_index=0;


     list<registry_record*>::iterator iter;
     WORD foundCount=0;
     bool founded=false;

     if ((quantity()>0) && (name!=0)) {
            iter=registry.begin();
            iter++;
            do {
                    BYTE res=strncmp((*iter)->name, name, size);
                    //printf("cmd=%s, res=%d\n",(*iter)->get_cmdline(),res);
                    if (res==0) {
                        result_index=(*iter)->index;
                        founded=true;
                        foundCount++;
                    }
                    iter++;
            } while (iter!=registry.end());

            //if (founded) printf("foundCount=%d\n", foundCount);
        }
     return result_index;
}

const void* valuesRegistry::getValuePtr(WORD index)
{
      const void* result=0;
      list<registry_record*>::iterator iter;

      for(iter=registry.begin(); (*iter)->index!= index; ++iter);
      result=(*iter)->valuePtr;
      return result;
}

OrtsType valuesRegistry::getType(WORD index)
{
      OrtsType result;
      list<registry_record*>::iterator iter;

      for(iter=registry.begin(); (*iter)->index!= index; ++iter);
      result=(*iter)->type;
      return result;
}

const char* valuesRegistry::getName (WORD index)
{
        const char* result=0;
        list<registry_record*>::iterator iter;
        for(iter=registry.begin(); (*iter)->index!= index; ++iter);
        result=(*iter)->name;
        return result;
}

size_t valuesRegistry::quantity ()
{
      return registry.size();
}
