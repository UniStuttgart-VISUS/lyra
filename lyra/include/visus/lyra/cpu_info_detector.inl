// <copyright file="cpu_info_detector.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


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
