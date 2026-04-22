// <copyright file="utf8_converter.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "utf8_converter.h"


/*
 * LYRA_DETAIL_NAMESPACE::utf8_converter::utf8_converter
 */
LYRA_DETAIL_NAMESPACE::utf8_converter::utf8_converter(void)
        : _converter(nullptr) {
    auto status = U_ZERO_ERROR;
    this->_converter = ::ucnv_open("UTF-8", &status);
    if (U_FAILURE(status)) {
        this->_converter = nullptr;
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::utf8_converter::~utf8_converter
 */
LYRA_DETAIL_NAMESPACE::utf8_converter::~utf8_converter(void) {
    if (this->_converter != nullptr) {
        ::ucnv_close(this->_converter);
    }
}
