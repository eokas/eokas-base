
#include "../engine/main.h"
using namespace eokas;

_eokas_test_case(os)
{
    printf("OS::getEnv: JAVA_HOME=%s\n", OS::getEnv("JAVA_HOME").cstr());
    return 0;
}
