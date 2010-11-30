/*
 * valueRegistry.h
 *
 *  Created on: Nov 29, 2010
 *      Author: tuborg
 */

#ifndef VALUEREGISTRY_H_
#define VALUEREGISTRY_H_

typedef struct registry_type{
        WORD index;
        char *name;
        const void* valuePtr;
        OrtsType type;
} registry_type;

class valueRegistry{
      list <registry_type*> registry;

public:
    valueRegistry();
    ~valueRegistry();

    errType addToRegistry(const void*, OrtsType, const char*);
    errType removeFromRegistry(WORD index);
    WORD searchIndexByName(const char*);

    void* getValuePtr(WORD index);
    OrtsType getType(WORD index);
    const char* getName (WORD index);
};

#endif /* VALUEREGISTRY_H_ */
