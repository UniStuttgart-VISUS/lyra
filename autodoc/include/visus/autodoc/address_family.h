// <copyright file="address_family.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2022 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for more information.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ADDRESS_FAMILY_H)
#define _LYRA_ADDRESS_FAMILY_H
#pragma once

/*
 * include\the\system\net\address_family.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(_WIN32)
#include <WinSock2.h>
#else /* defined(_WIN32) */
#include <sys/types.h>
#include <sys/socket.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Specifies the addressing scheme of e.g. a socket.
/// </summary>
enum class address_family {

    /// <summary>
    /// Unspecified address.
    /// </summary>
    unspecified = AF_UNSPEC,

    /// <summary>
    /// Unix local to host address.
    /// </summary>
#if !defined(_WIN32)
    // GCCs with C++11 should support this ...
#pragma push_macro("unix")
#undef unix
#endif /* !defined(_WIN32) */
    unix = AF_UNIX,
#if !defined(_WIN32)
#pragma pop_macro("unix")
#endif /* !defined(_WIN32) */

    /// <summary>
    /// IP version 4 adress.
    /// </summary>
    inter_network4 = AF_INET,

#if defined(_WIN32)
    /// <summary>
    /// ARPANET IMP address.
    /// </summary>
    imp_link = AF_IMPLINK,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// PUP protocol address.
    /// </summary>
    pup = AF_PUP,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// MIT CHAOS address.
    /// </summary>
    chaos = AF_CHAOS,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Xerox NS address.
    /// </summary>
    ns = AF_NS,
#endif /* defined(_WIN32) */

    /// <summary>
    /// IPX or SPX address.
    /// </summary>
    ipx = AF_IPX,

#if defined(_WIN32)
    /// <summary>
    /// Address for ISO protocols.
    /// </summary>
    iso = AF_ISO,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Address for OSI protocols.
    /// </summary>
    osi = AF_OSI,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    ///  ECMA address.
    /// </summary>
    ecma = AF_ECMA,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Data kit address.
    /// </summary>
    data_kit = AF_DATAKIT,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// CCITT address (e.g. X.25).
    /// </summary>
    ccitt = AF_CCITT,
#endif /* defined(_WIN32) */

    /// <summary>
    /// IBM SNA address.
    /// </summary>
    sna = AF_SNA,

    /// <summary>
    /// DECnet address.
    /// </summary>
    dec_net = AF_DECnet,

#if defined(_WIN32)
    /// <summary>
    /// Direct data-link interface address.
    /// </summary>
    data_link = AF_DLI,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// LAT address.
    /// </summary>
    lat = AF_LAT,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// NSC Hyperchannel address.
    /// </summary>
    hyper_channel = AF_HYLINK,
#endif /* defined(_WIN32) */

    /// <summary>
    /// AppleTalk address.
    /// </summary>
    apple_talk = AF_APPLETALK,

#if defined(_WIN32)
    /// <summary>
    /// NetBios address.
    /// </summary>
    net_bios = AF_NETBIOS,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// VoiceView address.
    /// </summary>
    voice_view = AF_VOICEVIEW,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Protocols from Firefox.
    /// </summary>
    fire_fox = AF_FIREFOX,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
        /// <summary>
        /// Unknown address.
        /// </summary>
        /// <remarks>
        /// Original documentation: Somebody is using this!
        /// </remarks>
    unknown1 = AF_UNKNOWN1,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Banyan address.
    /// </summary>
    banyan = AF_BAN,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Native ATM services address.
    /// </summary>
    atm = AF_ATM,
#endif /* defined(_WIN32) */

    /// <summary>
    /// IPv6 address.
    /// </summary>
    inter_network6 = AF_INET6,

#if defined(_WIN32)
    /// <summary>
    /// Microsoft cluster product address.
    /// </summary>
    cluster = AF_CLUSTER,
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// IEEE 1284.4 workgroup address.
    /// </summary>
    ieee12844 = AF_12844,
#endif /* defined(_WIN32) */

    /// <summary>
    /// IrDA address.
    /// </summary>
    irda = AF_IRDA,

#if defined(_WIN32)
    /// <summary>
    /// Address for Network Designers OSI gateway-enabled protocols.
    /// </summary>
    network_designers = AF_NETDES,
#endif /* defined(_WIN32) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0501))
    tcn_process = AF_TCNPROCESS,

    tcn_message = AF_TCNMESSAGE,

    iclfxbm = AF_ICLFXBM,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0501)) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0600))
    /// <summary>
    /// Bluetooth RFCOMM/L2CAP protocols.
    /// </summary>
    bth = AF_BTH,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0600)) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0601))
    link = AF_LINK,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0601)) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0604))
    hyperv = AF_HYPERV,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0604)) */

    /// <summary>
    /// Maximum address marker.
    /// </summary>
    max = AF_MAX
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_ADDRESS_FAMILY_H) */
