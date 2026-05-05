// <copyright file="sysfs_device.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::from_path
 */
template<class TIterator > 
std::size_t LYRA_DETAIL_NAMESPACE::sysfs_device::from_path(
        _In_ TIterator oit,
        _In_ const std::filesystem::path& path,
        _In_ const bool recursive) {
    if (!std::filesystem::exists(path)) {
        return 0;
    }

    // Note: the following enumeration should be safe with recursive iterators,
    // too, as long as we do not follow symlinks, which might cause an infinite
    // loop.
    const auto enumerate = [&oit](const auto& iterator) {
        std::size_t retval = 0;

        for (auto& p : iterator) {
            const auto subsystem = p.path() / "subsystem";
            LYRA_TRACE("Checking for subsystem directory \"%s\".",
                subsystem.c_str());

            if (std::filesystem::exists(subsystem)) {
                LYRA_TRACE("Found device at\"%s\".", p.path().c_str());
                *oit++ = sysfs_device(p);
                ++retval;
            }
        }

        return retval;
    };

    return (recursive)
        ? enumerate(std::filesystem::recursive_directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied))
        : enumerate(std::filesystem::directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied));
}
