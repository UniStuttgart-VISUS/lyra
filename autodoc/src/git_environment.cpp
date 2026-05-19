// <copyright file="git_environment.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "git_environment.h"

#include <algorithm>
#include <fstream>
#include <regex>

#include "visus/autodoc/trace.h"

#include "invoke.h"
#include "processes.h"
#include "property_set_impl.h"
#include "push_directory.h"
#include "string_manipulation.h"


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::current
 */
LYRA_DETAIL_NAMESPACE::git_environment
LYRA_DETAIL_NAMESPACE::git_environment::current(void) {
    return git_environment(std::filesystem::current_path());
}


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::git_environment
 */
LYRA_DETAIL_NAMESPACE::git_environment::git_environment(
        const std::filesystem::path& directory)
        : _directory(directory) {
    if (!std::filesystem::exists(_directory)) {
        this->_directory.clear();
    }

    this->_directory = std::filesystem::absolute(this->_directory);
    while (!this->_directory.empty()) {
        auto p = this->_directory / git_directory;
        if (std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
            // Found the root directory of a Git repository.
            break;
        }

        if (this->_directory == this->_directory.root_path()) {
            // The input is definitely not located in a Git repository.
            this->_directory.clear();

        } else {
            // Check the parent directory.
            this->_directory = this->_directory.parent_path();
        }
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::diff
 */
std::string LYRA_DETAIL_NAMESPACE::git_environment::diff(void) const {
    try {
        LYRA_PUSH_DIRECTORY(this->_directory);
        return invoke("git diff");
    } catch (const std::exception& ex) {
        LYRA_TRACE("Invoking Git failed: %s", ex.what());
        return "";
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::head
 */
std::string LYRA_DETAIL_NAMESPACE::git_environment::head(void) const {
    const std::regex rx_ref("^ref:\\s*(.+)$");
    auto path = this->_directory / git_directory / "HEAD";

    std::ifstream f(path);
    std::string line;
    if (f.is_open() && std::getline(f, line)) {
        std::smatch match;

        if (std::regex_match(line, match, rx_ref)) {
            // The HEAD contains a reference to the current branch.
            const auto ref = match[1].str();
            path = this->_directory / git_directory / ref;

            f.close();
            f.open(path);
            if (f.is_open() && std::getline(f, line)) {
                return line;
            }

            f.close();
            f.open(this->_directory / git_directory / "packed-refs");
            if (f.is_open()) {
                while (std::getline(f, line)) {
                    std::string hash, name;
                    while (f >> hash >> name) {
                        if (name == ref) {
                            return hash;
                        }
                    }
                }
            }

        } else {
            // The HEAD contains the hash of the current commit directly.
            return line;
        }
    }

    return "";
}


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::remotes
 */
std::unordered_map<std::string, std::string>
LYRA_DETAIL_NAMESPACE::git_environment::remotes(void) const {
    std::unordered_map<std::string, std::string> retval;
    const std::regex rx_remote("^\\s*\\[remote\\s+\"([^\"]+)\"\\]\\s*$");
    const std::regex rx_url("^\\s*url\\s*=\\s*(.+)\\s*$");
    const std::regex rx_section("^\\s*\\[([^\\]]+)\\]\\s*$");
    auto path = this->_directory / git_directory / "config";

    std::ifstream f(path);
    if (f.is_open()) {
        std::string line;
        std::smatch match;

        while (std::getline(f, line)) {
            if (std::regex_match(line, match, rx_remote)) {
                const auto name = match[1].str();

                while (std::getline(f, line) && !std::regex_match(line,
                    rx_section)) {
                    if (std::regex_match(line, match, rx_url)) {
                        const auto url = match[1].str();
                        retval.emplace(std::move(name), std::move(url));
                        break;
                    }
                } /* while (std::getline(f, line) && !std::regex_match(line, */
            } /* if (std::regex_match(line, match, rx_remote)) */
        } /* while (std::getline(f, line)) */
    } /* if (f.is_open()) */

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::git_environment::status
 */
std::string LYRA_DETAIL_NAMESPACE::git_environment::status(void) const {
    try {
        LYRA_PUSH_DIRECTORY(this->_directory);
        return invoke("git status -s");
    } catch (const std::exception& ex) {
        LYRA_TRACE("Invoking Git failed: %s", ex.what());
        return "";
    }
}
