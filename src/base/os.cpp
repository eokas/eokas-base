
#include "./os.h"
#include "./string.h"
#include <cstdlib>

#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
    #include <Windows.h>
    #include <Wbemidl.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "Iphlpapi.lib")
#else
    #include <unistd.h>
    #include <pwd.h>
    #include <ifaddrs.h>
    #include <sys/utsname.h>
    #include <sys/socket.h>
    #include <net/if_types.h>
    #include <net/if_dl.h>
    #include <net/ethernet.h>
#endif

namespace eokas {
    String OS::getSystemName() {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        return "Windows";
#elif _EOKAS_OS == __EOKAS_OS_LINUX
        return "Linux";
#elif _EOKAS_OS == _EOKAS_OS_MACOS
        return "macOS";
#elif _EOKAS_OS == _EOKAS_OS_IOS
        return "iOS";
#elif _EOKAS_OS == _EOKAS_OS_ANDROID
        return "Android";

#else
        return "Unknown";
#endif
    }
    
    String OS::getSystemVersion() {
        return "";
    }
    
    String OS::getDeviceName() {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        CHAR buffer[256] = {0};
        DWORD count = 0;
        if (!GetComputerNameA(buffer, &count))
            return "";
        return String{buffer, count};
#else
        char buffer[256] = {0};
        gethostname(buffer, 256);
        return String{buffer};
#endif
    }
    
    String OS::getDeviceModel() {
        return "";
    }
    
    u32_t OS::getCpuCount() {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        u32_t cpuCount = sysInfo.dwNumberOfProcessors;
        return cpuCount;
#else
        u32_t cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
        return cpuCount;
#endif
    }
    
    String OS::getEnv(const String& name) {
        return std::getenv(name.cstr());
    }
    
    bool OS::setEnv(const String& name, const String& value) {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        return _putenv_s(name.cstr(), value.cstr()) == 0;
#else
        return setenv(name.cstr(), value.cstr(), 1) == 0;
#endif
    }
    
    String OS::getCurrentUser() {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        CHAR buffer[256];
        DWORD count = 0;
        if (!GetUserNameA(buffer, &count))
            return "";
        return String(buffer, count);
#else
        uid_t uid = geteuid();
        struct passwd* pw = getpwuid(uid);
        if (pw == nullptr)
            return "";
        return pw->pw_name;
#endif
    }
    
    String OS::getMacAddress() {
#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32
        IP_ADAPTER_INFO adapterInfo[16];
        DWORD dwBufLen = sizeof(adapterInfo);
        if (GetAdaptersInfo(adapterInfo, &dwBufLen) != ERROR_SUCCESS)
            return "";
        String macAddr;
        PIP_ADAPTER_INFO adapterPtr = adapterInfo;
        while (adapterPtr != nullptr) {
            if(adapterPtr->Type == MIB_IF_TYPE_ETHERNET) {
                for (UINT i = 0; i < adapterPtr->AddressLength; i++) {
                    macAddr += String::format("%02X", adapterPtr->Address[i]);
                }
                break;
            }
            adapterPtr = adapterPtr->Next;
        }
        return macAddr;
#else
        struct ifaddrs* addrs;
        if (getifaddrs(&addrs) != 0) {
            return "";
        }
        
        String macAddress;
        for (struct ifaddrs* addrPtr = addrs; addrPtr != nullptr; addrPtr = addrPtr->ifa_next) {
            if(addrPtr->ifa_addr->sa_family == AF_LINK) {
                sockaddr_dl* sdl = (sockaddr_dl*)(addrPtr->ifa_addr);
                if(sdl->sdl_type == IFT_ETHER) {
                    char* mac = (char*)ether_ntoa((const struct ether_addr*)LLADDR(sdl));
                    macAddress = mac;
                    break;
                }
            }
        }
    
        freeifaddrs(addrs);
        
        return macAddress;
#endif
    }
}
