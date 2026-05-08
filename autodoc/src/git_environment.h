// <copyright file="git_environment.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_GIT_ENVIRONMENT_H)
#define _LYRA_GIT_ENVIRONMENT_H
#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "visus/autodoc/property_set.h"

#include "property_set_impl.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Contains some information about the Git enviroment a specific file or folder
/// is located in.
/// </summary>
class LYRA_TEST_API git_environment final {

public:

    static constexpr auto head_property = u8"Head";
    static constexpr auto git_directory = ".git";
    static constexpr auto remotes_property = u8"Remotes";

    /// <summary>
    /// Creates a new instance for the current working directory.
    /// </summary>
    /// <returns></returns>
    static git_environment current(void);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="directory"></param>
    explicit git_environment(const std::filesystem::path& directory);

    /// <summary>
    /// Gets the hash of the current HEAD commit.
    /// </summary>
    /// <returns>The hash of the current HEAD commit or an empty string if
    /// the environment is not valid.</returns>
    std::string head(void) const;

    /// <summary>
    /// Gets, if any, the names and URLs of the remotes.
    /// </summary>
    /// <returns>A map with the names of the remotes as keys and their URLs as
    /// values.</returns>
    std::unordered_map<std::string, std::string> remotes(void) const;

    /// <summary>
    /// Indicates whether the Git environment is valid.
    /// </summary>
    /// <returns><see langword="true" /> if the Git environment is valid,
    /// <see langword="false" /> if the directory provided to the constructor
    /// is not in a Git repository.</returns>
    inline operator bool(void) const noexcept {
        return !this->_directory.empty();
    }

    /// <summary>
    /// Converts the environment to property set.
    /// </summary>
    /// <returns>A property set containing the Git environment
    /// information.</returns>
    operator property_set(void) const {
        // Note: For some bizarre reason I don't understand, MSVC only allows
        // this operator to be defined if it is inline.
        detail::property_set_impl retval;

        if (*this) {
            retval.add(head_property, this->head());

            const auto remotes = this->remotes();
            if (!remotes.empty()) {
                detail::property_set_impl rps;
                for (const auto& r : remotes) {
                    rps.add(r.first.c_str(), r.second.c_str());
                }
                retval.add(remotes_property, property_set(std::move(rps)));
            }
        }

        return property_set(std::move(retval));
    }

private:

    std::filesystem::path _directory;

};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_GIT_ENVIRONMENT_H) */
