// <copyright file="cpu_info_value.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_INFO_VALUE_H)
#define _LYRA_CPU_INFO_VALUE_H
#pragma once

#include <cinttypes>
#include <climits>
#include <vector>

#include "visus/autodoc/cpu_vendor.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Creates a mask for the <paramref name="bit" />th bit.
/// </summary>
/// <param name="bit">The zero-based index of the bit to mask.</param>
/// <returns>The mask for the specified bit.</returns>
inline constexpr std::uint32_t bit_mask(_In_ const std::uint32_t bit) noexcept {
    return static_cast<std::size_t>(1) << bit;
}


/// <summary>
/// A condition that unconditionally evaluates to <see langword="true" />.
/// </summary>
struct true_condition final {

    /// <summary>
    /// Converts the object to the condition value.
    /// </summary>
    /// <returns><see langword="true" />.</returns>
    inline constexpr operator bool(void) const noexcept {
        return true;
    }
};


/// <summary>
/// A condition that works on the given <typeparamref name="TVendor" />
/// detector.
/// </summary>
template<class TVendor> struct vendor_condition final {

    /// <summary>
    /// Evaluates the CPUID result and answers whether it identifies the vendor
    /// described by <typeparamref name="TVendor" />.
    /// </summary>
    /// <returns><see langword="true" /> if the vendor matches,
    /// <see langword="false" /> otherwise.</returns>
    inline operator bool(void) const noexcept {
        return TVendor::check();
    }
};

LYRA_DETAIL_NAMESPACE_END


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Provides names for the indices of <see cref="cpu_info.values" /> to improve
/// readability of the <see cref="cpu_info_value" /> and
/// <see cref="cpu_info_selector" />.
/// </summary>
enum cpu_info_register : std::size_t {
    /// <summary>
    /// Identifies the EAX register.
    /// </summary>
    eax = 0,

    /// <summary>
    /// Identifies the EBX register.
    /// </summary>
    ebx = 1,

    /// <summary>
    /// Identifies the ECX register.
    /// </summary>
    ecx = 2,

    /// <summary>
    /// Identifies the EDX register.
    /// </summary>
    edx = 3
};


/// <summary>
/// The base class for all values derived from CPUID.
/// </summary>
/// <typeparam name="Leaf">The leaf to retrieve.</typeparam>
/// <typeparam name="Sub">The (optional) sub-leaf to retrieve. This defaults to
/// zero.</typeparam>
/// <typeparam name="TCond">A condition to be evaluated before getting the
/// value.</typeparam>
template<std::size_t Leaf, std::size_t Sub, std::size_t Reg,
    class TCond = detail::true_condition>
class cpu_info_value_ex {
    static_assert(Reg < 4, "The register must be within [0, 4[.");

public:

    /// <summary>
    /// The type of the condition that must be met for the value to be
    /// retrieved.
    /// </summary>
    typedef TCond condition_type;

    /// <summary>
    /// The type of the value being retrieved.
    /// </summary>
    typedef std::uint32_t value_type;

    /// <summary>
    /// Gets the value of the CPUID instruction for <typeparamref name="Leaf" />
    /// (and optionally the <typeparamref name="Sub" /> leaf) provided that
    /// <typeparamref name="TCond" /> evaluates to <see langword="true" />.
    /// </summary>
    /// <param name="info"></param>
    /// <returns><see langword="true" /> if <paramref name="info" /> has been
    /// set to a valid value, <see langword="false" /> otherwise.</returns>
    static inline bool get(_Out_ cpu_info& info) noexcept {
        return TCond() ? get_cpu_info(info, Leaf, Sub) : false;
    }

    /// <summary>
    /// Invokes the CPUID instruction for <typeparamref name="Leaf" /> (and
    /// optionally the <typeparamref name="Sub" /> leaf) and returns the value
    /// of the specified register if <typeparamref name="TCond" /> evaluates to
    /// <see langword="true" />.
    /// </summary>
    /// <returns>The value of the selected register <typeparamref name="Reg" />
    /// or zero if the info could not be retrieved.</returns>
    static inline value_type get(void) noexcept {
        cpu_info info;
        return get(info) ? info.values[Reg] : 0;
    }
};


/// <summary>
/// Retrieves a the register <typeparamref name="Reg" /> from the CPUID of
/// leaf <typeparamref name="Leaf" />.
/// </summary>
/// <typeparam name="TCond"></typeparam>
/// <typeparam name="Leaf"></typeparam>
/// <typeparam name="Reg"></typeparam>
template<std::size_t Leaf, std::size_t Reg, class TCond = detail::true_condition>
using cpu_info_value = cpu_info_value_ex<Leaf, 0, Reg, TCond>;


/// <summary>
/// Checks whether any of the bits in the specified register of the given CPUID
/// leaf is set.
/// </summary>
/// <typeparam name="Fun"></typeparam>
/// <typeparam name="Sub"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Sub, std::size_t Reg,
    class TCond = detail::true_condition>
class cpu_info_any_ex : private cpu_info_value_ex<Leaf, Sub, Reg, TCond> {
    typedef cpu_info_value_ex<Leaf, Sub, Reg, TCond> base_type;

public:

    /// <summary>
    /// The type of the condition that must be met for the value to be
    /// retrieved.
    /// </summary>
    typedef typename base_type::condition_type condition_type;

    /// <summary>
    /// The type of the value being retrieved.
    /// </summary>
    typedef bool value_type;

    /// <summary>
    /// Check whether any of the bits in <typename="Reg" /> is set in the
    /// <paramref name="info" />.
    /// </summary>
    /// <param name="info"></param>
    /// <returns></returns>
    static inline bool get(_In_ const cpu_info& info) noexcept {
        const auto reg = info.values[Reg];
        assert(!TCond() || ((base_type::get() != 0) == (reg != 0)));
        return (condition_type() && (reg != 0));
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline cpu_info_any_ex(void) noexcept {
        cpu_info info;
        this->_value = base_type::get(info) && cpu_info_any_ex::get(info);
    }

    /// <summary>
    /// Answer whether any bit of <typename="Reg" /> is set.
    /// </summary>
    /// <returns><see langword="true" /> if the register is not zero,
    /// <see langword="false" /> otherwise.</returns>
    inline operator value_type(void) const noexcept {
        return this->_value;
    }

private:

    value_type _value;
};


/// <summary>
/// Checks whether any of the bits in the specified register of the given CPUID
/// leaf is set.
/// </summary>
/// <typeparam name="Fun"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Reg,
    class TCond = detail::true_condition>
using cpu_info_any = cpu_info_any_ex<Leaf, 0, Reg, TCond>;


/// <summary>
/// Base implementation for detecting the presence of a bit in the CPU info
/// of x86 CPUs.
/// </summary>
/// <typeparam name="Leaf"></typeparam>
/// <typeparam name="Sub"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="Bit"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Sub, std::size_t Reg, std::uint32_t Bit,
    class TCond = detail::true_condition>
class cpu_info_bit_ex : private cpu_info_value_ex<Leaf, Sub, Reg, TCond> {
    typedef cpu_info_value_ex<Leaf, Sub, Reg, TCond> base_type;
    static_assert(Bit < sizeof(cpu_info::registers.eax) * CHAR_BIT, "The "
        "selected bit must not exceed 32 bits.");

public:

    /// <summary>
    /// The type of the condition that must be met for the value to be
    /// retrieved.
    /// </summary>
    typedef typename base_type::condition_type condition_type;

    /// <summary>
    /// The type of the value being retrieved.
    /// </summary>
    typedef bool value_type;

    /// <summary>
    /// Check whether the specified <paramref name="Bit" /> is set in the
    /// <paramref name="Reg" /> of the <paramref name="info" />.
    /// </summary>
    /// <param name="info"></param>
    /// <returns></returns>
    static inline bool get(_In_ const cpu_info& info) noexcept {
        static constexpr auto mask = detail::bit_mask(Bit);
        const auto reg = info.values[Reg];
        assert(!TCond() || ((base_type::get() & mask) == (reg & mask)));
        return (TCond() && ((reg & mask) == mask));
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline cpu_info_bit_ex(void) noexcept {
        cpu_info info;
        this->_value = base_type::get(info) && cpu_info_bit_ex::get(info);
    }

    /// <summary>
    /// Answer whether the specified <typeparamref name="Bit" /> is set in 
    /// <typename="Reg" />.
    /// </summary>
    /// <returns><see langword="true" /> if the bit is set,
    /// <see langword="false" /> otherwise.</returns>
    inline operator value_type(void) const noexcept {
        return this->_value;
    }

private:

    value_type _value;
};


/// <summary>
/// Base implementation for detecting the presence of a bit in the CPU info
/// of x86 CPUs.
/// </summary>
/// <typeparam name="Leaf"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="Bit"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Reg, std::uint32_t Bit,
    class TCond = detail::true_condition>
using cpu_info_bit = cpu_info_bit_ex<Leaf, 0, Reg, Bit, TCond>;


/// <summary>
/// Extracts the range of bits <typeparamref name="From" /> to
/// <typeparamref name="To" /> from register <typename="Reg" /> of the CPUID
/// leaf <typeparamref name="Leaf" />.
/// </summary>
/// <typeparam name="Leaf"></typeparam>
/// <typeparam name="Sub"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="From"></typeparam>
/// <typeparam name="To"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Sub, std::size_t Reg, std::uint32_t From,
    std::uint32_t To, class TCond = detail::true_condition>
class cpu_info_bits_ex : private cpu_info_value_ex<Leaf, Sub, Reg, TCond> {
    typedef cpu_info_value_ex<Leaf, Sub, Reg, TCond> base_type;
    static_assert(From < To, "The index of the first bit must be smaller than "
        "the index of the last bit.");
    static_assert(To < sizeof(cpu_info::registers.eax) * CHAR_BIT, "The "
        "selected range must not exceed 32 bits.");

public:

    /// <summary>
    /// The type of the condition that must be met for the value to be
    /// retrieved.
    /// </summary>
    typedef typename base_type::condition_type condition_type;

    /// <summary>
    /// The type of the value being retrieved.
    /// </summary>
    typedef std::uint32_t value_type;

    /// <summary>
    /// Extracts bits <typeparamref name="From" /> to <typeparamref name="To" />
    /// from the specified register <paramref name="Reg" /> of
    /// <paramref name="info" />.
    /// </summary>
    /// <param name="info"></param>
    /// <returns></returns>
    static inline std::uint32_t get(_In_ const cpu_info& info) noexcept {
        constexpr auto mask = (detail::bit_mask(To - From + 1) - 1);
        const auto reg = info.values[Reg];
        assert(!TCond() || (((base_type::get() >> From) & mask)
            == ((reg >> From) & mask)));
        return TCond() ? ((reg >> From) & mask) : 0;
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline cpu_info_bits_ex(void) noexcept {
        cpu_info info;
        this->_value = base_type::get(info) ? cpu_info_bits_ex::get(info) : 0;
    }

    /// <summary>
    /// Answer the selected bits of <typename="Reg" />.
    /// </summary>
    /// <returns>The value selected by the mask.</returns>
    inline operator value_type(void) const noexcept {
        return this->_value;
    }

private:

    value_type _value;
};


/// <summary>
/// Extracts the range of bits <typeparamref name="From" /> to
/// <typeparamref name="To" /> from register <typename="Reg" /> of the CPUID
/// leaf <typeparamref name="Leaf" />.
/// </summary>
/// <typeparam name="Leaf"></typeparam>
/// <typeparam name="Reg"></typeparam>
/// <typeparam name="From"></typeparam>
/// <typeparam name="To"></typeparam>
/// <typeparam name="TCond"></typeparam>
template<std::size_t Leaf, std::size_t Reg, std::uint32_t From,
    std::uint32_t To, class TCond = detail::true_condition>
using cpu_info_bits = cpu_info_bits_ex<Leaf, 0, Reg, From, To, TCond>;

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_INFO_VALUE_H) */
