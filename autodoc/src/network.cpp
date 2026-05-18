// <copyright file="network.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include <WinSock2.h>
#endif /* defined(_WIN32) */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>

#if defined(_WIN32)
#include <iphlpapi.h>
#include <Windows.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>

#else /* defined(_WIN32) */
#include <arpa/inet.h>

#include <net/if.h>

#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <ifaddrs.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/convert_string.h"
#include "visus/autodoc/network.h"
#include "visus/autodoc/on_exit.h"

#include "file.h"
#include "property_set_impl.h"
#include "string_manipulation.h"


#if !defined(_WIN32)
/// <summary>
/// Combines all information about a single interface in one place.
/// </summary>
struct per_interface_data final {
    LYRA_NAMESPACE::multi_sz anycast_addresses;
    LYRA_NAMESPACE::multi_sz p2p_addresses;
    std::vector<LYRA_DETAIL_NAMESPACE::property_set_impl> unicast_addresses;
};
#endif /* !defined(_WIN32) */


#if defined(_WIN32)
/// <summary>
/// Enumerates the addresses of all network adapters on the local machine.
/// </summary>
template<class TCallback>
static std::size_t enumerate_adapter_addresses(
        _In_ TCallback callback,
        _In_ const ULONG family = AF_UNSPEC,
        _In_ const ULONG flags = 0) {
    constexpr auto retries = 16;
    std::vector<std::uint8_t> addresses;
    std::size_t retval = 0;
    auto status = ERROR_BUFFER_OVERFLOW;

    for (auto i = 0; (i < retries) && (status == ERROR_BUFFER_OVERFLOW); ++i) {
        auto addrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(addresses.data());
        auto size = static_cast<DWORD>(addresses.size());
        status = ::GetAdaptersAddresses(family, flags, nullptr, addrs, &size);
        if (status == ERROR_SUCCESS) {
            for (auto a = addrs; a != nullptr; a = a->Next) {
                ++retval;
                callback(*a);
            }
        } else if ((status == ERROR_BUFFER_OVERFLOW) && (i + 1 < retries)) {
            addresses.resize(size);
        }
    }

    return retval;
}
#endif /* defined(_WIN32) */


/// <summary>
/// Convert the IP address contained in the given socket address to a string.
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
std::string to_string(_In_ const sockaddr *address) {
    const void *addr;
    char buffer[INET6_ADDRSTRLEN];

    if (address == nullptr) {
        *buffer = 0;
        return buffer;
    }

    switch (address->sa_family) {
        case AF_INET:
            addr = &reinterpret_cast<const sockaddr_in *>(address)->sin_addr;
            break;

        case AF_INET6:
            addr = &reinterpret_cast<const sockaddr_in6 *>(address)->sin6_addr;
            break;

        default:
            addr = nullptr;
            break;
    }

    if ((addr == nullptr) || (::inet_ntop(address->sa_family, addr,
            buffer, sizeof(buffer)) == nullptr)) {
        // If lookup fails, make sure that the string is empty.
        *buffer = 0;
    }

    return buffer;
}


/*
 * LYRA_NAMESPACE::network::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::network::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    ::enumerate_adapter_addresses([&](const IP_ADAPTER_ADDRESSES& addr) {
        detail::property_set_impl p;
        detail::checked_add("Adapter Name", p, flags, addr.AdapterName);
        detail::checked_add("Description", p, flags,
            to_utf8(addr.Description));
        detail::checked_add("DNS Suffix", p, flags, to_utf8(addr.DnsSuffix));
        detail::checked_add<interface_index>(p, flags,
            static_cast<interface_index::type>(addr.IfIndex));
        detail::checked_add<interface_name>(p, flags,
            to_utf8(addr.FriendlyName));
        detail::checked_add<receive_speed>(p, flags, addr.ReceiveLinkSpeed);
        detail::checked_add<transmit_speed>(p, flags, addr.TransmitLinkSpeed);
        detail::checked_add<speed>(p, flags, (std::min)(addr.ReceiveLinkSpeed,
            addr.TransmitLinkSpeed));

        std::vector<property_set> uni_addr;
        for (auto a = addr.FirstUnicastAddress; a != nullptr; a = a->Next) {
            auto s = ::to_string(a->Address.lpSockaddr);
            if (s.empty()) {
                continue;
            }

            detail::property_set_impl p;
            detail::checked_add<address>(p, flags, std::move(s));
            detail::checked_add<prefix_length>(p, flags, a->OnLinkPrefixLength);
            uni_addr.emplace_back(std::move(p));
        }
        detail::checked_add<unicast_address>(p, flags, std::move(uni_addr));

        multi_sz any_addr;
        for (auto a = addr.FirstAnycastAddress; a != nullptr; a = a->Next) {
            auto s = ::to_string(a->Address.lpSockaddr);
            if (!s.empty()) {
                any_addr.add(s);
            }
        }
        detail::checked_add<anycast_address>(p, flags, std::move(any_addr));

        multi_sz multi_addr;
        for (auto a = addr.FirstMulticastAddress; a != nullptr; a = a->Next) {
            auto s = ::to_string(a->Address.lpSockaddr);
            if (!s.empty()) {
                multi_addr.add(s);
            }
        }
        detail::checked_add<multicast_address>(p, flags, std::move(multi_addr));

        {
            auto s = to_string(addr.Dhcpv4Server.lpSockaddr);
            if (!s.empty()) {
                detail::checked_add("DHCPv4 Server", p, flags, s.c_str());
            }
        }

        {
            auto s = to_string(addr.Dhcpv6Server.lpSockaddr);
            if (!s.empty()) {
                detail::checked_add("DHCPv6 Server", p, flags, s.c_str());
            }
        }

        {
            auto s = detail::to_hex_string(addr.Dhcpv6ClientDuid,
                addr.Dhcpv6ClientDuidLength);
            detail::checked_add("DHCPv6 Client DUID", p, flags, s.c_str());
        }

        detail::checked_add("DHCPv6 IAID", p, flags, addr.Dhcpv6Iaid);

        {
            auto s = detail::to_hex_string(addr.PhysicalAddress,
                addr.PhysicalAddressLength);
            detail::checked_add<hardware_address>(p, flags,
                multi_sz::for_string(s));
        }

        ps.add(std::to_string(addr.IfIndex), property_set(std::move(p)));
    });

#else /* defined(_WIN32) */
    struct ifaddrs *addresses;
    std::map<std::string, per_interface_data> interfaces;

    // Collect all addresses and netmask per interface.
    if (::getifaddrs(&addresses) == 0) {
        LYRA_ON_EXIT([addresses](void) { ::freeifaddrs(addresses); });

        for (auto a = addresses; a != nullptr; a = a->ifa_next) {
            auto& interface = interfaces[a->ifa_name];

            auto addr_string = ::to_string(a->ifa_addr);
            if (!addr_string.empty()) {
                auto& ps = interface.unicast_addresses.emplace_back();
                detail::checked_add<address>(ps, flags,
                    std::move(addr_string));
                detail::checked_add<netmask>(ps, flags,
                    ::to_string(a->ifa_netmask));
            }

            if ((a->ifa_flags & IFF_BROADCAST) != 0) {
                auto addr_string = ::to_string(a->ifa_broadaddr);
                if (!addr_string.empty()) {
                    interface.anycast_addresses.add(addr_string);
                }

            } else if ((a->ifa_flags & IFF_POINTOPOINT) != 0) {
                auto addr_string = ::to_string(a->ifa_dstaddr);
                if (!addr_string.empty()) {
                    interface.p2p_addresses.add(addr_string);
                }
            }
        }
    }

    // Construct interface property sets for the interfaces.
    for (auto& i : interfaces) {
        detail::property_set_impl p;
        detail::checked_add<interface_name>(p, flags, i.first.c_str());

        if (!i.second.anycast_addresses.empty()) {
            detail::checked_add<anycast_address>(p, flags,
                multi_sz::for_strings(i.second.anycast_addresses.begin(),
                i.second.anycast_addresses.end()));
        }

        if (!i.second.p2p_addresses.empty()) {
            detail::checked_add<p2p_address>(p, flags,
                multi_sz::for_strings(i.second.p2p_addresses.begin(),
                    i.second.p2p_addresses.end()));
        }

        if (!i.second.unicast_addresses.empty()) {
            auto a = detail::make_property_sets(std::move(
                i.second.unicast_addresses));
            detail::checked_add<unicast_address>(p, flags, std::move(a));
        }

        // Get additional stuff from sysfs if possible.
        const std::filesystem::path class_root("/sys/class/net");

        {
            auto path = class_root / i.first / "address";
            if (std::filesystem::exists(path)) {
                std::string s;
                std::ifstream f(path);
                if (std::getline(f, s) && !s.empty()) {
                    detail::checked_add<hardware_address>(p, flags, s);
                }
            }
        }

        {
            auto path = class_root / i.first / "broadcast";
            if (std::filesystem::exists(path)) {
                std::string s;
                std::ifstream f(path);
                if (std::getline(f, s) && !s.empty()) {
                    detail::checked_add<broadcast_address>(p, flags, s);
                }
            }
        }

        {
            auto path = class_root / i.first / "duplex";
            if (std::filesystem::exists(path)) {
                std::string s;
                std::ifstream f(path);
                if (std::getline(f, s) && !s.empty()) {
                    detail::checked_add("Duplex", p, flags, s.c_str());
                }
            }
        }

        {
            auto path = class_root / i.first / "mtu";
            if (std::filesystem::exists(path)) {
                std::string s;
                std::ifstream f(path);
                if (std::getline(f, s) && !s.empty()) {
                    detail::checked_add<mtu>(p, flags, std::stoul(s));
                }
            }
        }

        {
            auto path = class_root / i.first / "speed";
            if (std::filesystem::exists(path)) {
                std::string s;
                std::ifstream f(path);
                if (std::getline(f, s) && !s.empty()) {
                    detail::checked_add<speed>(p, flags, std::stoul(s));
                }
            }
        }

        ps.add(i.first.c_str(), property_set(std::move(p)));
    }
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
}
