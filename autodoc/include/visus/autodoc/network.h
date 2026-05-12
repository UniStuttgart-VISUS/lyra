// <copyright file="network.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_NETWORK_H)
#define _LYRA_NETWORK_H
#pragma once

#include <cinttypes>

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace network {

    /// <summary>
    /// Identifies a property that contains an IP address string.
    /// </summary>
    struct address final {
        typedef const char *type;
        static constexpr const char *name = "Address";
    };

    /// <summary>
    /// Identifies a property that contains the anycast address(es) of a
    /// network interface.
    /// </summary>
    struct anycast_address final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr auto is_uncommon = true;
        static constexpr const char *name = "Anycast Address";
    };

    /// <summary>
    /// Identifies a property that contains the broadcast address(es) of a
    /// network interface.
    /// </summary>
    struct broadcast_address final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr auto is_uncommon = true;
        static constexpr const char *name = "Broadcast Address";
    };

    /// <summary>
    /// Identifies a property that contains the hardware (MAC) address of an
    /// interface.
    /// </summary>
    struct hardware_address final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr const char *name = "Hardware Address";
    };

    /// <summary>
    /// Identifies the property holding the index of a network interface.
    /// </summary>
    struct interface_index final {
        typedef std::uint64_t type;
        static constexpr auto is_uncommon = true;
        static constexpr const char *name = "Interface Index";
    };

    /// <summary>
    /// Identifies the property holding the name of a network interface.
    /// </summary>
    struct interface_name final {
        typedef const char *type;
        static constexpr const char *name = "Interface Name";
    };

    /// <summary>
    /// Identifies the property holding the maximum transmission unit (MTU) of
    /// a network interface.
    /// </summary>
    struct mtu final {
        typedef std::uint32_t type;
        static constexpr const char *name = "Maximum Transmission Unit";
    };

    /// <summary>
    /// Identifies a property that contains the multicast address(es) of a
    /// network interface.
    /// </summary>
    struct multicast_address final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr auto is_uncommon = true;
        static constexpr const char *name = "Multicast Address";
    };

    /// <summary>
    /// Identifies a property that contains a net mask.
    /// </summary>
    struct netmask final {
        typedef const char *type;
        static constexpr const char *name = "Netmask";
    };

    /// <summary>
    /// Identifies a property that contains the point-to-point address(es) of a
    /// network interface.
    /// </summary>
    struct p2p_address final {
        typedef const char *type;
        static constexpr auto is_sensitive = true;
        static constexpr auto is_uncommon = true;
        static constexpr const char *name = "Point-to-Point Address";
    };

    /// <summary>
    /// Identifies a property that holds the prefix length.
    /// </summary>
    struct prefix_length final {
        typedef std::uint8_t type;
        static constexpr const char *name = "Prefix Length";
    };

    /// <summary>
    /// Identifies the property holding the receive speed of a network interface.
    /// </summary>
    struct receive_speed final {
        typedef std::uint64_t type;
        static constexpr const char *name = "Receive Speed";
    };

    /// <summary>
    /// Identifies the property holding the link speed of a network interface.
    /// </summary>
    struct speed final {
        typedef std::uint64_t type;
        static constexpr const char *name = "Speed";
    };

    /// <summary>
    /// Identifies the property holding the send speed of a network interface.
    /// </summary>
    struct transmit_speed final {
        typedef std::uint64_t type;
        static constexpr const char *name = "Transmit Speed";
    };

    /// <summary>
    /// Identifies a property that contains the unicast address(es) of a
    /// network interface. The unicast address is a property set with the
    /// actual address and the netmask/prefix length.
    /// </summary>
    struct unicast_address final {
        typedef property_set type;
        static constexpr auto is_sensitive = true;
        static constexpr const char *name = "Unicast Address";
    };

    /// <summary>
    /// Gets all network interfaces we can find on the system.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>The hardware properties of the calling process.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace network */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_NETWORK_H) */
