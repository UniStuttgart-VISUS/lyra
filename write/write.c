// <copyright file="write.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/autodoc.h"


/// <summary>
/// Entry point of the demo application which uses the one-stop documentation
/// function.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(const int argc, const char **argv) {
    autodoc_write_raw(NULL, collection_flags_none);
    return 0;
}
