// <copyright file="os_properties.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_OS_PROPERTIES_H)
#define _LYRA_OS_PROPERTIES_H
#pragma once

#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN


/// <summary>
/// Represents a group of properties that identify the operating system
/// software.
/// </summary>
class LYRA_API os_properties final : public property_set {

public:

    static constexpr auto property_family = u8"family";
    static constexpr auto property_version = u8"version";

    inline _Ret_maybenull_z_ const char *family(void) const noexcept {
        void *retval;
        std::size_t length;
        property_type type;

        if (get(retval, length, type, property_family)) {
            //assert(type == property_type::string);
            return static_cast<const char *>(retval);
        }

        return nullptr;
    }

};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_OS_PROPERTIES_H) */
