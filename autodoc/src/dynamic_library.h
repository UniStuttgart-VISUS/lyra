// <copyright file="dynamic_library.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2021 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_DYNAMIC_LIBRARY_H)
#define _LYRA_DYNAMIC_LIBRARY_H

#pragma once

#include <array>
#include <system_error>
#include <unordered_map>

#if defined(_WIN32)
#include <windows.h>
#else /* defined(_WIN32) */
#include <dlfcn.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A (base) class for managing libraries that are lazily loaded.
/// </summary>
class LYRA_TEST_API dynamic_library {

public:

#if defined(_WIN32)
    typedef FARPROC function_type;
#else /* defined(_WIN32) */
    typedef void *function_type;
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    typedef HMODULE handle_type;
#else /* defined(_WIN32) */
    typedef void *handle_type;
#endif /* defined(_WIN32) */

    static constexpr handle_type invalid_handle
#if defined(_WIN32)
        = NULL;
#else /* defined(_WIN32) */
        = nullptr;
#endif /* defined(_WIN32) */

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="handle">A native library handle.</param>
    explicit dynamic_library(handle_type&& handle);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <typeparam name="TPath">The character type used for the path. On Linux,
    /// this must be <see langword="char" />. On Windows,
    /// <see langword="wchar_t" /> is supported as well.</typeparam>
    /// <typeparam name="TPaths">The character types used for the fallback
    /// paths. This must be <typeparamref name="TPath" />.</typeparam>
    /// <param name="path">The preferred path to the library.</param>
    /// <param name="paths">Additional fallback paths to check for, in order
    /// of preference.</param>
    template<class TPath, class... TPaths>
    dynamic_library(_In_z_ const TPath *path, TPaths&&... paths);

    dynamic_library(_Inout_ dynamic_library&& rhs) noexcept;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~dynamic_library(void) noexcept;

    dynamic_library& operator =(_Inout_ dynamic_library&& rhs) noexcept;

    /// <summary>
    /// Gets the function with the specified name from the library.
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    _Must_inspect_result_ function_type get_function(const char *name) noexcept;

    /// <summary>
    /// Gets the function with the specified name from the library and casts it
    /// to the specified function pointer type.
    /// </summary>
    /// <typeparam name="TFunction"></typeparam>
    /// <param name="name"></param>
    /// <returns></returns>
    template<class TFunction>
    inline TFunction get_function(const char *name) noexcept {
        return reinterpret_cast<TFunction>(this->get_function(name));
    }

    /// <summary>
    /// Indicates whether the library has successfully been loaded.
    /// </summary>
    /// <returns></returns>
    inline operator bool(void) const noexcept {
        return (this->_handle != invalid_handle);
    }

private:

    std::unordered_map<std::string, function_type> _cache;
    handle_type _handle;
};

LYRA_DETAIL_NAMESPACE_END


/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library
 */
template<class TPath, class... TPaths>
LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library(
        _In_z_ const TPath *path,
        TPaths&&... paths)
        : _handle(invalid_handle) {
    std::array<const TPath *, sizeof...(TPaths)> ps = { paths... };

    for (auto& p : ps) {
#if defined(_WIN32)
        if constexpr (std::is_same_v<TPath, wchar_t>) {
            this->_handle = ::LoadLibraryW(p);
        } else {
            this->_handle = ::LoadLibraryA(p);
        }
#else /* defined(_WIN32) */
        this->_handle = ::dlopen(p, RTLD_LAZY);
#endif /* defined(_WIN32) */
        if (this->_handle != invalid_handle) {
            break;
        }
    }

    if (this->_handle == invalid_handle) {
#if defined(_WIN32)
        throw std::system_error(::GetLastError(), std::system_category());
#else /* defined(_WIN32) */
        throw std::runtime_error(::dlerror());
#endif /* defined(_WIN32) */
    }
}

#endif /* !defined(_LYRA_DYNAMIC_LIBRARY_H) */
