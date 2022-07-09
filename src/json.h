
#ifndef  _EOKAS_BASE_JSON_H_
#define  _EOKAS_BASE_JSON_H_

#include "hom.h"

_BeginNamespace(eokas)

struct Json
{
    static String stringify(const HomValue::Ref& json);

    static HomValue::Ref parse(const String& source);
};

_EndNamespace(eokas)

#endif//_EOKAS_BASE_JSON_H_
