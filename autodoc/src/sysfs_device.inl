// <copyright file="sysfs_device.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::from_path
 */
template<class TIterator>
std::size_t LYRA_DETAIL_NAMESPACE::sysfs_device::from_path(
        _In_ TIterator oit,
        _In_ const std::filesystem::path& path) {
    std::size_t retval = 0;

    if (!std::filesystem::exists(path)) {
        return retval;
    }

    for (auto& p : std::filesystem::directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied)) {
        const auto subsystem = p.path() / "subsystem";
        LYRA_TRACE("Checking for subsystem directory \"%s\".",
            subsystem.c_str());

        if (std::filesystem::exists(subsystem)) {
            *oit++ = sysfs_device(p.path());
            ++retval;
        }
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::from_container
 */
template<class TIterator > 
std::size_t LYRA_DETAIL_NAMESPACE::sysfs_device::from_container(
        _In_ TIterator oit,
        _In_ const std::filesystem::path& path) {
    std::size_t retval = 0;

    if (!std::filesystem::exists(path)) {
        return retval;
    }

    // Note: the following should be safe as long as we do not follow symlinks,
    // which might cause an infinite loop.
    for (auto& p : std::filesystem::recursive_directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied)) {
        if (p.is_directory()) {
            retval += from_path(oit, p.path());
        }
    }

    return retval;
}
