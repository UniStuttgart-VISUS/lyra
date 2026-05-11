// <copyright file="os_cpu_info.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::enumerate_cpu_info
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enumerate_cpu_info(_In_ TCallback callback) {
    constexpr auto abortable = std::is_invocable_r_v<bool, TCallback,
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>;
    std::vector<std::uint8_t> buffer;
    std::size_t retval = 0;

    auto info = get_logical_processor_info(buffer, RelationAll);

    for (std::size_t o = 0; o < buffer.size();) {
        info = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(
            buffer.data() + o);

        ++retval;
        if constexpr (abortable) {
            if (!callback(*info)) {
                return
            }
        } else {
            callback(*info);
        }

        o += info->Size;
    }

    return retval;
}
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::enumerate_cpu_toplogy
 */
template<class TCallback>
void LYRA_DETAIL_NAMESPACE::enumerate_cpu_toplogy(_In_ TCallback callback) {
    enumerate_cpu_info([&](const auto& info) {
         switch (info.Relationship) {
             case RelationProcessorPackage:
             case RelationProcessorCore:
             case RelationNumaNode:
                 callback(info);
                 break;
         }
    });
}
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
