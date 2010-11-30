/*
 * valueRegistry.cpp
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */
#include "valueRegistry.h"

valueRegistry::valueRegistry(){ }
valueRegistry::~valueRegistry(){ }

errType valueRegistry::addToRegistry(const void* valuePtr, OrtsType type, const char* name)
{
        errType result=err_not_init;
        registry_type record;

        record.name=name;
        record.type=type;
        record.valuePtr=valuePtr;
        record.index=registry.size;

        if (getRegistryValue(name)==0)
        {
            registry.push_back(record);
        } else {
            printf("same record exists!\n");
            removeRegistryValue(name);
            registry.push_back(record);
        }
        return result;
}

errType valueRegistry::removeFromRegistry(WORD index)
{
        errType result=err_not_init;

        return result;
}

WORD valueRegistry::searchIndexByName(const char*)
{
     errType result=err_not_init;

     return result;
}

void* valueRegistry::getValuePtr(WORD index)
{
      errType result=err_not_init;

      return result;
}

OrtsType valueRegistry::getType(WORD index)
{

}

const char* valueRegistry::getName (WORD index)
{

}
