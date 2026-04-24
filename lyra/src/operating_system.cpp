// <copyright file="operating_system.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/operating_system.h"

#include <cstring>
#include <regex>
#include <string>
#include <system_error>

#ifdef _WIN32
#include <lmcons.h>
#include <tchar.h>
#include <Windows.h>
#else /* _WIN32 */
#include <unistd.h>

#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#endif /* _WIN32 */

#include "visus/lyra/trace.h"
#include "visus/lyra/version.h"

#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::operating_system::get_version
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::operating_system::get_version(
        void) {
    detail::property_set_impl ps;
    property_set retval;

#if defined(_WIN32)
    typedef NTSTATUS (WINAPI *gv_t)(PRTL_OSVERSIONINFOW);
    gv_t gv = nullptr;

    auto hModule = ::GetModuleHandleW(L"ntdll.dll");
    if (hModule != nullptr) {
        gv = reinterpret_cast<gv_t>(::GetProcAddress(hModule, "RtlGetVersion"));
    }

    // First, try calling the runtime library directly, which will give us the
    // real OS version rather than the SDK version we use.
    if (gv != nullptr) {
        LYRA_TRACE(_T("Using RtlGetVersion to determine the Windows version."));
        RTL_OSVERSIONINFOW vi = { 0 };
        vi.dwOSVersionInfoSize = sizeof(vi);

        if (gv(&vi) == 0) {
            ps.add<LYRA_NAMESPACE::version::major>(vi.dwMajorVersion);
            ps.add<LYRA_NAMESPACE::version::minor>(vi.dwMinorVersion);
            ps.add<LYRA_NAMESPACE::version::patch>(vi.dwBuildNumber);

        } else {
            LYRA_TRACE(_T("RtlGetVersion failed."));
            gv = nullptr;
        }
    }

    // Use public API (returns SDK version on Windows 10). Furthermore, the
    // public API tells us whether we are on a server or workstation.
    {
        OSVERSIONINFOEXW vi { 0 };
        vi.dwOSVersionInfoSize = sizeof(vi);

#pragma warning(push)
#pragma warning(disable: 4996)
        if (::GetVersionExW(reinterpret_cast<LPOSVERSIONINFOW>(&vi))) {
            if (gv == nullptr) {
                ps.add<LYRA_NAMESPACE::version::major>(vi.dwMajorVersion);
                ps.add<LYRA_NAMESPACE::version::minor>(vi.dwMinorVersion);
                ps.add<LYRA_NAMESPACE::version::patch>(vi.dwBuildNumber);
            }

            ps.add(u8"service_pack_major", vi.wServicePackMajor);
            ps.add(u8"service_pack_minor", vi.wServicePackMinor);

            switch (vi.wProductType) {
                case VER_NT_WORKSTATION:
                    ps.add(u8"product_type", u8"Workstation");
                    break;

                case VER_NT_DOMAIN_CONTROLLER:
                    ps.add(u8"product_type", u8"Domain Controller");
                    break;

                case VER_NT_SERVER:
                    ps.add(u8"product_type", u8"Server");
                    break;
            }
        }
#pragma warning(pop)
    }

#else /* defined(_WIN32) */
    static const std::regex rx("(\\d+)\\.(\\d+)\\.(\\d+)");
    std::cmatch m;
    utsname vi;

    // http://stackoverflow.com/questions/6315666/c-get-linux-distribution-name-version
    if (::uname(&vi) >= 0) {
        if (std::regex_match(vi.release, m, rx)) {
            ps.add<LYRA_NAMESPACE::version::major>(std::stoul(m[1].str()));
            ps.add<LYRA_NAMESPACE::version::minor>(std::stoul(m[2].str()));
            ps.add<LYRA_NAMESPACE::version::patch>(std::stoul(m[3].str()));
        }

        ps.add(u8"release", vi.release);
        ps.add(u8"version", vi.version);
    }

    //this->name = vi.sysname;
    //this->version = vi.release;
    //this->word_size = (::strstr(vi.machine, "64") == nullptr) ? 32 : 64;

#endif /* defined(_WIN32) */

    return detail::move(retval, std::move(ps));
}
