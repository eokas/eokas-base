
#include "../engine/main.h"
using namespace eokas;

_eokas_test_case(hom)
{
    HomObject* obj = new HomObject();
    obj->set("name", "eokas-hom");
    obj->set("version", "1.0.0");

    printf("obj.name: %s\n", obj->getString("name").cstr());
    printf("obj.version: %s\n", obj->getString("version").cstr());

    return 0;
}