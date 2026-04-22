// <copyright file="property_set_ctor.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/property_set.h"


int test_property_set_ctor(int, char *[]) {
    LYRA_NAMESPACE::property_set properties;
    return properties.empty() ? 0 : 1;
}
