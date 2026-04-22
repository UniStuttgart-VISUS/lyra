// <copyright file="icu_converter.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "icu_converter.h"


/*
 * LYRA_DETAIL_NAMESPACE::icu_converter::icu_converter
 */
LYRA_DETAIL_NAMESPACE::icu_converter::icu_converter(
        _In_opt_z_ const char *encoding)
        : _converter(nullptr) {
    auto status = U_ZERO_ERROR;
    this->_converter = ::ucnv_open(encoding, &status);
    if (U_FAILURE(status)) {
        this->_converter = nullptr;
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::icu_converter::~icu_converter
 */
LYRA_DETAIL_NAMESPACE::icu_converter::~icu_converter(void) {
    if (this->_converter != nullptr) {
        ::ucnv_close(this->_converter);
    }
}
