// <copyright file="autodoc.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/autodoc.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <system_error>

#include <nlohmann/json.hpp>

#include "visus/autodoc/raw.h"
#include "visus/autodoc/trace.h"

#include "processes.h"
#include "results.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Implements the collector for <see cref="autodoc" />.
/// </summary>
struct collect final {
    /// <summary>
    /// Stores the path where the collected data should be stored.
    /// </summary>
    std::filesystem::path path;

    /// <summary>
    /// Stores the collection flags.
    /// </summary>
    collection_flags flags;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    template<class TChar> inline collect(
            _In_ const TChar *path,
            _In_ const collection_flags flags)
        : path(path), flags(flags) { }

    /// <summary>
    /// Performs the collection and stores the data.
    /// </summary>
    inline void operator ()(_In_ const collection_flags next) noexcept {
        try {
            const auto data = raw::get(this->flags);

            std::ofstream stream(this->path.c_str(), std::ios::trunc);
            stream << data.json();
            stream.flush();

            // Prepare the flags and the path for the call in the destructor.
            this->flags = next;

            auto ext = this->path.extension();
            ext = ".exit" + ext.string();
            this->path.replace_extension(ext);
        } catch (const std::exception& ex) {
            LYRA_TRACE("Uncaught exception: %s", ex.what());
        }
    }
};

LYRA_NAMESPACE_END


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Gets the fallback path for the output file if none was specified.
/// </summary>
static std::filesystem::path get_default_path(void) {
    std::filesystem::path retval(LYRA_DETAIL_NAMESPACE::get_executable_path());
    return retval.replace_extension(".json");
}


/// <summary>
/// Creates a collector, runs it and returns <see langword="nullptr" />.
/// </summary>
template<class TChar> _Ret_maybenull_ collect *make_collect(
        _In_opt_z_ const TChar *path,
        _In_ const collection_flags flags) {
    std::unique_ptr<collect> collector;

    if (path != nullptr) {
        collector = std::make_unique<collect>(path, flags);
    } else {
        auto p = detail::get_default_path();
        collector = std::make_unique<collect>(p.c_str(), flags);
    }

    (*collector)(collection_flags::no_immutable);

    return nullptr;
}


/// <summary>
/// Creates a collector, runs it and returns it with exit flags set. The
/// returned collector must be released with <see langword="delete" />.
/// </summary>
template<class TChar> _Ret_maybenull_ collect *make_collect(
        _In_opt_z_ const TChar *path,
        _In_ const collection_flags enter_flags,
        _In_ const collection_flags exit_flags) {
    std::unique_ptr<collect> collector;

    if (path != nullptr) {
        collector = std::make_unique<collect>(path, enter_flags);
    } else {
        auto p = detail::get_default_path();
        collector = std::make_unique<collect>(p.c_str(), enter_flags);
    }

    (*collector)(exit_flags);

    return collector.release();
}


/// <summary>
/// Implementation of <see cref="autodoc_write_raw_a" /> and
/// <see cref="autodoc_write_raw_w" />. Note that wide characters are only
/// supported on Windows.
/// </summary>
template<class TChar>
int write_raw(_In_z_ const TChar *path, _In_ const collection_flags flags) {
    auto p = (path != nullptr)
        ? std::filesystem::path(path)
        : detail::get_default_path();

    try {
        const auto data = LYRA_NAMESPACE::raw::get(flags);
        std::ofstream stream(p, std::ios::trunc);
        stream << data.json();
        return 0;
    } catch (std::system_error& ex) {
        LYRA_TRACE("Uncaught exception: %s", ex.what());
        return ex.code().value();
    } catch (const std::exception& ex) {
        LYRA_TRACE("Uncaught exception: %s", ex.what());
        return -1;
    } catch (...) {
        LYRA_TRACE("Uncaught exception.");
        return -2;
    }
}

LYRA_DETAIL_NAMESPACE_END


/*
 * LYRA_NAMESPACE::autodoc::autodoc
 */
LYRA_NAMESPACE::autodoc::autodoc(_In_opt_z_ const char *path,
        _In_ const collection_flags flags) 
    : _collect(detail::make_collect(path, flags)) { }


/*
 * LYRA_NAMESPACE::autodoc::autodoc
 */
LYRA_NAMESPACE::autodoc::autodoc(_In_opt_z_ const char *path,
        _In_ const collection_flags enter_flags,
        _In_ const collection_flags exit_flags)
    : _collect(detail::make_collect(path, enter_flags, exit_flags)) { }


/*
 * LYRA_NAMESPACE::autodoc::autodoc
 */
LYRA_NAMESPACE::autodoc::autodoc(_In_opt_z_ const wchar_t *path,
        _In_ const collection_flags flags)
    : _collect(detail::make_collect(path, flags)) { }


/*
 * LYRA_NAMESPACE::autodoc::autodoc
 */
LYRA_NAMESPACE::autodoc::autodoc(_In_opt_z_ const wchar_t *path,
        _In_ const collection_flags enter_flags,
        _In_ const collection_flags exit_flags)
    : _collect(detail::make_collect(path, enter_flags, exit_flags)) { }


/*
 * LYRA_NAMESPACE::autodoc::~autodoc
 */
LYRA_NAMESPACE::autodoc::~autodoc(void) noexcept {
    if (this->_collect != nullptr) {
        (*this->_collect)(collection_flags::none);
        delete this->_collect;
    }
}


/*
 * ::autodoc_write_raw_a
 */
int autodoc_write_raw_a(_In_z_ const char *path,
        _In_ const LYRA_NAMESPACE::collection_flags flags) {
    return LYRA_DETAIL_NAMESPACE::write_raw(path, flags);
}


/*
 * ::autodoc_write_raw_w
 */
int autodoc_write_raw_w(_In_z_ const wchar_t *path,
        _In_ const LYRA_NAMESPACE::collection_flags flags) {
    return LYRA_DETAIL_NAMESPACE::write_raw(path, flags);
}
