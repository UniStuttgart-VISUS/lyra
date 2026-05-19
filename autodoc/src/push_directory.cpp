// <copyright file="push_directory.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "push_directory.h"


/*
 * LYRA_DETAIL_NAMESPACE::workdir_scope::workdir_scope
 */
LYRA_DETAIL_NAMESPACE::workdir_scope::workdir_scope(
        _In_ const std::filesystem::path& directory)
        : _directory(std::filesystem::current_path()) {
    std::filesystem::current_path(directory);
}


/*
 * LYRA_DETAIL_NAMESPACE::workdir_scope::~workdir_scope
 */
LYRA_DETAIL_NAMESPACE::workdir_scope::~workdir_scope(void) noexcept {
    if (!this->_directory.empty()) {
        std::filesystem::current_path(this->_directory);
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::workdir_scope::operator =
 */
LYRA_DETAIL_NAMESPACE::workdir_scope&
LYRA_DETAIL_NAMESPACE::workdir_scope::operator =(_Inout_ workdir_scope& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->_directory = std::move(rhs._directory);
        assert(rhs._directory.empty());
    }

    return *this;
}
