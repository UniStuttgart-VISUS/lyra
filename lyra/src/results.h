// <copyright file="results.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_RESULTS_H)
#define _LYRA_RESULTS_H
#pragma once

#include "visus/lyra/result_type.h"

#if !defined(_WIN32)
#include <cerrno>
#endif /* !defined(_WIN32) */


#define _LYRA_RESULTS_BEGIN LYRA_DETAIL_NAMESPACE_BEGIN namespace results {
#define _LYRA_RESULTS_END } LYRA_DETAIL_NAMESPACE_END

#if defined(_WIN32)
#define _LYRA_RESULT(name, win, lin) constexpr result_type name = (win)
#define _LYRA_W32(code) HRESULT_FROM_WIN32(code)
#else /* defined(_WIN32) */
#define _LYRA_RESULT(name win, lin) constexpr result_type name = (lin)
#define _LYRA_W32(code)
#endif /* defined(_WIN32) */


_LYRA_RESULTS_BEGIN

_LYRA_RESULT(access_denied, _LYRA_W32(ERROR_ACCESS_DENIED), EACCES);
_LYRA_RESULT(catastrophic_failure, E_FAIL, ENOTSUP);
_LYRA_RESULT(insufficient_buffer, _LYRA_W32(ERROR_INSUFFICIENT_BUFFER), ERANGE);
_LYRA_RESULT(invalid_argument, E_INVALIDARG, EINVAL);
_LYRA_RESULT(invalid_state, E_NOT_VALID_STATE, EBADFD);
_LYRA_RESULT(not_found, _LYRA_W32(ERROR_NOT_FOUND), ENOENT);
_LYRA_RESULT(out_of_memory, _LYRA_W32(ERROR_OUTOFMEMORY), ENOMEM);
_LYRA_RESULT(success, S_OK, 0);

_LYRA_RESULTS_END

#endif /* !defined(_LYRA_RESULTS_H) */
