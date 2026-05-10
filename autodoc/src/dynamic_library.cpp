// <copyright file="dynamic_library.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2021 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "dynamic_library.h"

#include <memory>

#include "visus/autodoc/trace.h"


/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library
 */
LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library(
        _Inout_ dynamic_library&& rhs) noexcept
    : _cache(std::move(rhs._cache)),
        _handle(rhs._handle) {
    rhs._handle = invalid_handle;
}


/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::~dynamic_library
 */
LYRA_DETAIL_NAMESPACE::dynamic_library::~dynamic_library(void) noexcept {
    if (this->_handle != invalid_handle) {
#if defined(_WIN32)
        ::FreeLibrary(this->_handle);
#else /* defined(_WIN32) */
        ::dlclose(this->_handle);
#endif /* defined(_WIN32) */
    }
}



/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::get_function
 */
_Must_inspect_result_ LYRA_DETAIL_NAMESPACE::dynamic_library::function_type
LYRA_DETAIL_NAMESPACE::dynamic_library::get_function(
        const char *name) noexcept {
    if (name == nullptr) {
        LYRA_TRACE(_T("Invalid function name provided."));
        return nullptr;
    }

    const std::string key(name);
    auto it = this->_cache.find(key);
    if (it != this->_cache.end()) {
        LYRA_TRACE(_T("Reusing cached function pointer."));
        return it->second;
    }

#if defined(_WIN32)
    auto retval = ::GetProcAddress(this->_handle, name);
#else /* defined(_WIN32) */
    auto retval = ::dlsym(this->_handle, name);
#endif /* defined(_WIN32) */

    if (retval != nullptr) {
        LYRA_TRACE(_T("Caching function pointer."));
        this->_cache.emplace(key, retval);
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::operator =
 */
LYRA_DETAIL_NAMESPACE::dynamic_library&
LYRA_DETAIL_NAMESPACE::dynamic_library::operator =(
        _Inout_ dynamic_library&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->_cache = std::move(rhs._cache);
        this->_handle = rhs._handle;
        rhs._handle = invalid_handle;
    }

    return *this;
}


/*
 * LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library
 */
LYRA_DETAIL_NAMESPACE::dynamic_library::dynamic_library(
        _Inout_ handle_type&& handle) : _handle(handle) {
    if (this->_handle == invalid_handle) {
        throw new std::invalid_argument("A valid library handle must be "
            "provided.");
    }

    handle = invalid_handle;
}
