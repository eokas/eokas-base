
#include "./os.h"

#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32

#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <iphlpapi.h>
#pragma comment(lib, "wbemuuid.lib")

#else

#include <limits.h>
#include <unistd.h>
#include <sys/utsname.h>

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
        return String(buffer, count);
#elif _EOKAS_OS == _EOKAS_OS_LINUX || _EOKAS_OS == _EOKAS_OS_MACOS
        char buffer[HOST_NAME_MAX] = {0};
        gethostname(buffer, HOST_NAME_MAX);
        return String(buffer);
#else

#endif
    }
    
    String OS::getDeviceModel() {
    
    }
}

#if _EOKAS_OS == _EOKAS_OS_WIN64 || _EOKAS_OS == _EOKAS_OS_WIN32

#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <iphlpapi.h>

#pragma comment(lib, "wbemuuid.lib")

namespace eokas {
    /**
     * WMI: Windows Management Infrastructure
     * https://learn.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer
     * */
    
    class WMIQuery {
        _ForbidCopy(WMIQuery);
        
        _ForbidAssign(WMIQuery);
    
    public:
        WMIQuery(IEnumWbemClassObject* enumerator)
            : m_Enumerator(enumerator) {
            while (m_Enumerator) {
                ULONG uReturn = 0;
                HRESULT hr = m_Enumerator->Next(WBEM_INFINITE, 1, &m_ClassObject, &uReturn);
                if (uReturn == 0) {
                    break;
                }
            }
            
        }
        
        virtual ~WMIQuery() {
            if (m_ClassObject != NULL) {
                m_ClassObject->Release();
                m_ClassObject = NULL;
            }
            if (m_Enumerator != NULL) {
                m_Enumerator->Release();
                m_Enumerator = NULL;
            }
        }
        
        std::string getValue(const std::string& key) {
            if (m_ClassObject == NULL)
                return "";
            
            std::wstring propKey = String::utf8ToUnicode(key, false);
            
            VARIANT propVal;
            VariantInit(&propVal);
            
            // Get the value of the Name property
            HRESULT hr = m_ClassObject->Get(propKey.c_str(), 0, &propVal, 0, 0);
            if (FAILED(hr)) {
                VariantClear(&propVal);
                return "";
            }
            
            std::string ret = String::unicodeToUtf8(propVal.bstrVal, false);
            
            VariantClear(&propVal);
            
            return ret;
        }
    
    private:
        IEnumWbemClassObject* m_Enumerator = NULL;
        IWbemClassObject* m_ClassObject = NULL;
    };
    
    class WMIScope {
        _ForbidCopy(WMIScope);
        
        _ForbidAssign(WMIScope);
    
    public:
        static WMIScope& local() {
            static const wchar_t* server = L"ROOT\\CIMV2";
            static WMIScope scope(server);
            return scope;
        }
    
    public:
        explicit WMIScope(const wchar_t* server) {
            this->init(server);
        }
        
        ~WMIScope() {
            this->quit();
        }
        
        bool init(const wchar_t* server) {
            /**
             * https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-coinitializeex
             * */
            HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
            if (FAILED(hr)) {
                return false;
            }
            
            hr = CoInitializeSecurity(NULL, -1,                             // COM authentication
                NULL,                           // Authentication services
                NULL,                           // Reserved
                RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                NULL,                        // Authentication info
                EOAC_NONE,                   // Additional capabilities
                NULL                         // Reserved
            );
            if (FAILED(hr)) {
                this->quit();
                return false;
            }
            
            hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*) &m_Locator);
            if (FAILED(hr)) {
                this->quit();
                return false;
            }
            
            hr = m_Locator->ConnectServer(_bstr_t(server), // Object path of WMI namespace
                NULL,                    // User name. NULL = current user
                NULL,                    // User password. NULL = current
                0,                       // Locale. NULL indicates current
                NULL,                    // Security flags.
                0,                       // Authority (for example, Kerberos)
                0,                       // Context object
                &m_Services           // pointer to IWbemServices proxy
            );
            if (FAILED(hr)) {
                this->quit();
                return false;
            }
            
            hr = CoSetProxyBlanket(m_Services,                        // Indicates the proxy to set
                RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                NULL,                        // Server principal name
                RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                NULL,                        // client identity
                EOAC_NONE                    // proxy capabilities
            );
            if (FAILED(hr)) {
                this->quit();
                return false;
            }
            
            return true;
        }
        
        void quit() {
            _DeleteList(m_Queries);
            
            if (m_Services != NULL) {
                m_Services->Release();
                m_Services = NULL;
            }
            if (m_Locator != NULL) {
                m_Locator->Release();
                m_Locator = NULL;
            }
            
            CoUninitialize();
        }
        
        WMIQuery* query(const char* wql) {
            if (m_Locator == NULL || m_Services == NULL)
                return NULL;
            
            IEnumWbemClassObject* enumerator = NULL;
            HRESULT hr = m_Services->ExecQuery(bstr_t("WQL"), bstr_t(wql), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);
            if (FAILED(hr)) {
                return NULL;
            }
            
            WMIQuery* q = new WMIQuery(enumerator);
            m_Queries.push_back(q);
            
            return q;
        }
    
    private:
        IWbemLocator* m_Locator = NULL;
        IWbemServices* m_Services = NULL;
        std::vector<WMIQuery*> m_Queries = {};
    };
    
    struct WindowsInfo {
        static String getBaseboardSerialNumber() {
            auto* query = WMIScope::local().query("SELECT * FROM Win32_Baseboard");
            return query->getValue("SerialNumber");
        }
        
        static String getBiosSerialNumber() {
            auto* query = WMIScope::local().query("SELECT * FROM Win32_BIOS");
            return query->getValue("SerialNumber");
        }
        
        static String getProcessorUniqueId() {
            auto* query = WMIScope::local().query("SELECT * FROM Win32_Processor");
            return query->getValue("UniqueId");
        }
        
        static String getDiskDriveSerialNumber() {
            auto* query = WMIScope::local().query("SELECT * FROM Win32_DiskDrive");
            return query->getValue("SerialNumber");
        }
        
        static String getOperatingSystemSerialNumber() {
            auto* query = WMIScope::local().query("SELECT * FROM Win32_OperatingSystem");
            return query->getValue("SerialNumber");
        }
    };
    
    String OS::getSystemVersion() {
        return "";
    }
    
    String OS::getDeviceName() {
    
    };
    
    u32_t OS::getCpuCount() {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        u32_t cpuCount = sysInfo.dwNumberOfProcessors;
        return cpuCount;
    }
    
    CpuState& OS::getCpuState(u32_t index) {
        static CpuState cpu;
        return cpu;
    }
    
    MemoryState& OS::getMemoryState() {
        static MemoryState result;
        if (result.total > 0)
            return result;
        
        MEMORYSTATUSEX memstat;
        memstat.dwLength = sizeof(memstat);
        if (!GlobalMemoryStatusEx(&memstat)) {
            return result;
        }
        
        result.total = memstat.ullTotalPhys;
        result.available = memstat.ullAvailPhys;
        return result;
    }
    
    String OS::getEnv(const String& name) {
        if (name.isEmpty())
            return "";
        
        char buffer[512] = {0};
        u32_t length = _ArrayLength(buffer);
        
        u32_t len = GetEnvironmentVariableA(name.cstr(), buffer, length);
        
        return String(buffer, len);
    }
    
    void OS::setEnv(const String& name, const String& value) {
        if (name.isEmpty())
            return;
        SetEnvironmentVariableA(name.cstr(), value.cstr());
    }
    
    String OS::getCurrentUser() {
        CHAR buffer[256];
        DWORD count = 0;
        if (!GetUserNameA(buffer, &count))
            return "";
        return String(buffer, count);
    }
    
    u32_t OS::getCurrentProcess() {
        return GetCurrentProcessId();
    }
    
    u32_t OS::getCurrentThread() {
        return GetCurrentThreadId();
    }
    
    String OS::getMacAddress() {
        IP_ADAPTER_INFO AdapterInfo[16];
        DWORD dwBufLen = sizeof(AdapterInfo);
        
        DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
        if (dwStatus != ERROR_SUCCESS) {
            return "";
        }
        
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        
        String macAddress;
        while (pAdapterInfo) {
            macAddress += pAdapterInfo->Address[0];
            for (UINT i = 1; i < pAdapterInfo->AddressLength; i++) {
                macAddress += "-" + pAdapterInfo->Address[i];
            }
            pAdapterInfo = pAdapterInfo->Next;
        }
        
        return macAddress;
    }
}

#elif _EOKAS_OS == _EOKAS_OS_MACOS

#elif _EOKAS_OS == _EOKAS_OS_LINUX

#include <unistd.h>
#include <limits.h>

namespace eokas
{
    String OS::getCurrentUser()
    {
        char buffer[LOGIN_NAME_MAX];
        getlogin_r(buffer, LOGIN_NAME_MAX);
        return String(buffer);
    }
    
    String OS::getMacAddress() {
        struct ifaddrs* ifap;
        struct ifaddrs* ifa;
        struct sockaddr_dl* sdl;
    
        if (getifaddrs(&ifap) != 0) {
            return "";
        }
    
        String macAddress;
        for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
            if ((ifa->ifa_addr->sa_family == AF_LINK) && ((sdl = (struct sockaddr_dl *)ifa->ifa_addr)->sdl_type == IFT_ETHER)) {
                char* mac = (char*)ether_ntoa((const struct ether_addr*)LLADDR(sdl));
                macAddress = mac;
                break;
            }
        }
    
        freeifaddrs(ifap);
        
        return macAddress;
    }
}

#elif _EOKAS_OS == _EOKAS_OS_IOS

#elif _EOKAS_OS == _EOKAS_OS_ANDROID

#endif
