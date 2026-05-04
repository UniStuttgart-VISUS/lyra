// <copyright file="autodoc.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/autodoc.h"

#include <fstream>
#include <system_error>

#include <nlohmann/json.hpp>

#include "visus/autodoc/raw.h"
#include "visus/autodoc/trace.h"


/*
 * ::autodoc_write_raw_a
 */
int autodoc_write_raw_a(
        _In_z_ const char *path,
        _In_ const LYRA_NAMESPACE::collection_flags flags) {
    try {
        const auto data = LYRA_NAMESPACE::raw::get(flags);
        std::ofstream stream(path, std::ios::trunc);
        stream << data.json();
        return 0;
    } catch (std::system_error& ex) {
        LYRA_TRACE("System error: %s", ex.what());
        return ex.code().value();
    } catch (const std::exception& ex) {
        LYRA_TRACE("Uncaught exception: %s", ex.what());
        return -1;
    } catch (...) {
        LYRA_TRACE("Uncaught excetpion.");
        return -2;
    }
}


#if defined(_WIN32)
/*
 * ::autodoc_write_raw_w
 */
int autodoc_write_raw_w(
        _In_z_ const wchar_t *path,
        _In_ const LYRA_NAMESPACE::collection_flags flags) {
    try {
        const auto data = LYRA_NAMESPACE::raw::get(flags);
        std::ofstream stream(path, std::ios::trunc);
        stream << data.json();
        return 0;
    } catch (std::system_error& ex) {
        LYRA_TRACE("System error: %s", ex.what());
        return ex.code().value();
    } catch (const std::exception& ex) {
        LYRA_TRACE("Uncaught exception: %s", ex.what());
        return -1;
    } catch (...) {
        LYRA_TRACE("Uncaught excetpion.");
        return -2;
    }
}
#endif /* defined(_WIN32) */
