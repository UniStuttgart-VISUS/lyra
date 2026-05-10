// <copyright file="os_cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "os_cpu_info.h"

#if !defined(_WIN32)
#include <unistd.h>
#endif /* !defined(_WIN32) */


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::get_logical_processor_info
 */
SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *
LYRA_DETAIL_NAMESPACE::get_logical_processor_info(
        _Out_ std::vector<std::uint8_t>& buffer,
        _In_ const LOGICAL_PROCESSOR_RELATIONSHIP relationship) {
    if (buffer.empty()) {
        buffer.resize(sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX));
    }

    auto retval = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(
        buffer.data());
    auto size = static_cast<DWORD>(buffer.size());

    while (true) {
        if (::GetLogicalProcessorInformationEx(relationship, retval, &size)) {
            return retval;
        }

        const auto error = ::GetLastError();
        if (error != ERROR_INSUFFICIENT_BUFFER) {
            ::SetLastError(error);
            return nullptr;
        }

        buffer.resize(size);
        retval = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(
            buffer.data());
    }
}
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */


/*
 * LYRA_DETAIL_NAMESPACE::get_os_max_cpus
 */
std::size_t LYRA_DETAIL_NAMESPACE::get_os_max_cpus(void) {
#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    const auto groups = ::GetActiveProcessorGroupCount();

    if (groups > 0){
        std::vector<std::uint8_t> buffer(groups
            * sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX));
        auto info = get_logical_processor_info(buffer, RelationGroup);

        std::size_t retval = 0;
        if ((info != nullptr) && (info->Relationship == RelationGroup)) {
            for (std::size_t i = 0; i < groups; ++i) {
                retval += info->Group.GroupInfo[i].ActiveProcessorCount;
            }

            return retval;
        }
    }
#endif /* (_WIN32_WINNT >= 0x0601) */

    {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
        return info.dwNumberOfProcessors;
    }

#else /* defined(_WIN32) */
    return static_cast<std::size_t>(::sysconf(_SC_NPROCESSORS_CONF));
#endif /* defined(_WIN32) */
}
