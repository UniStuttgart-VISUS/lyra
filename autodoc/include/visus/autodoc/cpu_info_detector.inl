// <copyright file="cpu_info_detector.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_NAMESPACE::cpu_info_detector<Fun, Reg, Mask>::cpu_info_any
 */
template<std::size_t Fun, std::size_t Reg>
LYRA_NAMESPACE::cpu_info_any<Fun, Reg>::cpu_info_any(void) : _value(false) {
    cpu_info info;

    if (get_cpu_info(info, Fun)) {
        this->_value = (info.values[Reg] != 0);
    }
}


/*
 * LYRA_NAMESPACE::cpu_info_detector<Fun, Reg, Mask>::cpu_info_detector
 */
template<std::size_t Fun, std::size_t Reg, std::uint32_t Mask>
LYRA_NAMESPACE::cpu_info_detector<Fun, Reg, Mask>::cpu_info_detector(
        void) : _value(false) {
    cpu_info info;

    if (get_cpu_info(info, Fun)) {
        const auto reg = static_cast<std::uint32_t>(info.values[Reg]);
        this->_value = ((reg & Mask) == Mask);
    }
}


/*
 * LYRA_NAMESPACE::cpu_info_selector<Fun, Reg, Mask>::cpu_info_selector
 */
template<std::size_t Fun, std::size_t Reg, std::uint32_t Mask>
LYRA_NAMESPACE::cpu_info_selector<Fun, Reg, Mask>::cpu_info_selector(
        void) : _value(0) {
    cpu_info info;

    if (get_cpu_info(info, Fun)) {
        const auto reg = static_cast<std::uint32_t>(info.values[Reg]);
        this->_value = ((reg & Mask) == Mask);
    }
}


/*
 * ...::cpu_info_vendor_detector<Ebx, Edx, Ecx>::cpu_info_vendor_detector
 */
template<std::uint32_t Ebx, std::uint32_t Edx, std::uint32_t Ecx>
LYRA_NAMESPACE::cpu_info_vendor_detector<Ebx, Edx, Ecx>
::cpu_info_vendor_detector(void) : _value(false) {
    cpu_info info;

    if (get_cpu_info(info, 0)) {
        this->_value = ((info.registers.ebx == Ebx)
            && (info.registers.ecx == Ecx)
            && (info.registers.edx == Edx));
    }
}
