/*
 * valueRegistry.h
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */


#ifndef VALUESREGISTRY_H_
#define VALUESREGISTRY_H_

using namespace std;
#include <list>

typedef struct registry_record{
        size_t index;
        const char *name;
        const void* valuePtr;
        OrtsType type;
} registry_record;

class valuesRegistry{
      list <registry_record*> registry;

public:
    valuesRegistry();
    ~valuesRegistry();

    errType addToRegistry(const void*, OrtsType, const char*, size_t);
    errType removeFromRegistry(WORD index);
    WORD searchIndexByName(const char*, size_t);

    const void* getValuePtr(WORD index);
    OrtsType getType(WORD index);
    const char* getName (WORD index);
    size_t quantity();


};

#endif /* VALUESREGISTRY_H_ */
