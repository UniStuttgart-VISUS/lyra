// <copyright file="dump.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/raw.h"

#include <iostream>

#include "nlohmann/json.hpp"

#if defined(_WIN32)
#include <tchar.h>
#else /* defined(_WIN32) */
#define _tmain main
#define _TCHAR char
#endif /* defined(_WIN32) */


/// <summary>
/// Entry point of the demo application which obtains all raw data and dumps it
/// to the console.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int _tmain(const int argc, const _TCHAR **argv) {
    const auto props = LYRA_NAMESPACE::raw::get();

    const auto json = props.json();
    if (json == nullptr) {
        return -1;
    }

    std::cout << nlohmann::json::parse(json).dump(4) << std::endl;
    return 0;
}
