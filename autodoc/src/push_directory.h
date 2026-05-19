// <copyright file="push_directory.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PUSH_DIRECTORY_H)
#define _LYRA_PUSH_DIRECTORY_H
#pragma once

#include <cassert>
#include <filesystem>
#include <memory>

#include "visus/autodoc/unique_variable.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// RAII container for temporarily changing the current working directory.
/// </summary>
class LYRA_TEST_API workdir_scope final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="directory">The directory to change to.</param>
    explicit workdir_scope(_In_ const std::filesystem::path& directory);

    workdir_scope(_Inout_ workdir_scope& rhs) noexcept
            : _directory(std::move(rhs._directory)) {
        assert(rhs._directory.empty());
    }

    ~workdir_scope(void) noexcept;

    workdir_scope& operator =(_Inout_ workdir_scope& rhs) noexcept;

private:

    std::filesystem::path _directory;
};


/// <summary>
/// Changes to <paramref name="dir" /> and returns a scope object that will
/// change back to the original directory when it goes out of scope.
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
inline [[nodiscard]] workdir_scope push_directory(
        _In_ const std::filesystem::path& dir) {
    return workdir_scope(dir);
}

LYRA_DETAIL_NAMESPACE_END


/// <summary>
/// Declares a workdir_scope variable that returns from the given path.
/// </summary>
#define LYRA_PUSH_DIRECTORY(dir)\
volatile auto LYRA_UNIQUE_VARIABLE(__lyra_work_dir_guard)\
    = LYRA_DETAIL_NAMESPACE::push_directory(dir)

#endif /* !defined(_LYRA_PUSH_DIRECTORY_H) */
