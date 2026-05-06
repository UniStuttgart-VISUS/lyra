// <copyright file="property_variant.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "property_variant.h"

#include "visus/autodoc/convert_string.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/property_set.h"


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::make_property_variant
 */
LYRA_DETAIL_NAMESPACE::property_variant
LYRA_DETAIL_NAMESPACE::make_property_variant(_In_ const VARIANT& v) {
    switch (v.vt) {
        //case VT_EMPTY:
        //case VT_NULL:
        case VT_I2: return v.iVal;
        case VT_I4: return v.intVal;
        case VT_R4: return v.fltVal;
        case VT_R8: return v.dblVal;
        case VT_CY: return v.cyVal.int64;
        case VT_DATE: return v.date;
        case VT_BSTR: return multi_sz::for_string(to_utf8(v.bstrVal,
            ::SysStringLen(v.bstrVal)));
        //case VT_DISPATCH:
        case VT_ERROR: return v.scode;
        case VT_BOOL: return (v.boolVal != VARIANT_FALSE);
        case VT_VARIANT: return make_property_variant(*v.pvarVal);
        //case VT_UNKNOWN
        //case VT_DECIMAL: return v.decVal.
        case VT_I1: return v.cVal;
        case VT_UI1: return v.bVal;
        case VT_UI2: return v.uiVal;
        case VT_UI4: return v.uintVal;
        case VT_I8: return v.llVal;
        case VT_UI8: return v.ullVal;
        case VT_INT: return v.intVal;
        case VT_UINT: return v.uintVal;
        case VT_HRESULT: return v.uintVal;
        case VT_LPSTR: return multi_sz::for_string(v.pcVal);
        case VT_LPWSTR: return multi_sz::for_string(to_utf8(v.bstrVal,
            ::SysStringLen(v.bstrVal)));
        case VT_FILETIME: return v.ullVal;
        default: return property_variant();
    }
}
#endif /* defined(_WIN32) */
