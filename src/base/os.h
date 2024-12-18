
#ifndef _EOKAS_BASE_OS_H_
#define _EOKAS_BASE_OS_H_

#include "./header.h"

namespace eokas {
    struct OS {
        static String getSystemName();
        static String getSystemVersion();
        static String getDeviceName();
        static String getDeviceModel();
        static u32_t getCpuCount();
        static String getCpuArch();
        static String getEnv(const String& name);
        static bool setEnv(const String& name, const String& value);
        static String getCurrentUser();
        static String getMacAddress();
    };
}

#endif //_EOKAS_BASE_OS_H_
