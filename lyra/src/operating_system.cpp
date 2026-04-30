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
 * LYRA_NAMESPACE::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::operating_system::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    if (detail::check_sensitive<family>(flags)) {
#if defined(_WIN32)
        ps.add<family>(u8"Windows");
#else /* defined(_WIN32) */
        utsname vi;
        if (::uname(&vi) >= 0) {
            ps.add<family>(vi.sysname);
        }
#endif /* defined(_WIN32) */
    }

    if (detail::check_sensitive<version::version>(flags)) {
        ps.add<version::version>(get_version(flags));
    }

    return detail::to_property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::operating_system::get_version
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::operating_system::get_version(
        _In_ const collection_flags flags) {
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

            if (!has_flag(flags, collection_flags::no_undeclared)) {
                ps.add(u8"Service Pack", LYRA_NAMESPACE::version::make(
                    static_cast<std::uint32_t>(vi.wServicePackMajor),
                    static_cast<std::uint32_t>(vi.wServicePackMinor)));

                switch (vi.wProductType) {
                    case VER_NT_WORKSTATION:
                        detail::checked_add(u8"Product Type", ps, flags,
                            u8"Workstation");
                        break;

                    case VER_NT_DOMAIN_CONTROLLER:
                        detail::checked_add(u8"Product Type", ps, flags,
                            u8"Domain Controller");
                        break;

                    case VER_NT_SERVER:
                        detail::checked_add(u8"Product Type", ps, flags,
                            u8"Server");
                        break;
                }
            } /* if (!has_flag(flags, collection_flags::no_undeclared)) */
        } /* if (::GetVersionExW(reinterpret_cast<LPOSVERSIONINFOW>(&vi))) */
#pragma warning(pop)
    }

#else /* defined(_WIN32) */
    static const std::regex rx("(\\d+)\\.(\\d+)\\.(\\d+).*");
    std::cmatch m;
    utsname vi;

    // http://stackoverflow.com/questions/6315666/c-get-linux-distribution-name-version
    if (::uname(&vi) >= 0) {
        if (std::regex_match(vi.release, m, rx)) {
            ps.add<LYRA_NAMESPACE::version::major>(std::stoul(m[1].str()));
            ps.add<LYRA_NAMESPACE::version::minor>(std::stoul(m[2].str()));
            ps.add<LYRA_NAMESPACE::version::patch>(std::stoul(m[3].str()));
        }

        if (!has_flag(flags, collection_flags::no_undeclared)) {
            detail::checked_add(u8"Release", ps, flags, vi.release);
            detail::checked_add(u8"System Name", ps, flags, vi.sysname);
            detail::checked_add(u8"Version", ps, flags, vi.version);
        }
    }
#endif /* defined(_WIN32) */

    detail::realise(retval, std::move(ps));
    return retval;
}
