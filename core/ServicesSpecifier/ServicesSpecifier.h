/*
 * ServicesSpecifier.h
 *
 *  Created on: May 11, 2011
 *      Author: tuborg
 */

#ifndef SERVICESSPECIFIER_H_
#define SERVICESSPECIFIER_H_

#include <string>
#include <extra/ortsTypes/ortsTypes.h>
#include <parsers/xml/xmlParser.h>

class services_specifier
{
public:
      int load_specs(const char*);
      int install_specs();
      int dbgPrint();

      services_specifier();
      ~services_specifier();
};

#endif /* SERVICESSPECIFIER_H_ */
