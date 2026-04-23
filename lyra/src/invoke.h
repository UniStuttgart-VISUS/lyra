// <copyright file="invoke.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_INVOKE_H)
#define _LYRA_INVOKE_H
#pragma once

#include <string>

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Invokes the given command and returns it output.
/// </summary>
/// <param name="cmd">The command to be invoked.</param>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::string invoke(_In_z_ const char *cmd);

/// <summary>
/// Invokes the given command and returns it output.
/// </summary>
/// <param name="cmd">The command to be invoked.</param>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::string invoke(_In_z_ const wchar_t *cmd);

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_INVOKE_H) */
