// <copyright file="processes.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CALLING_PROCESS_H)
#define _LYRA_CALLING_PROCESS_H
#pragma once

#include <string>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#else /* defined(_WIN32) */
#include <sys/types.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

#if defined(_WIN32)
/// <summary>
/// The type used to represent a process ID.
/// </summary>
typedef DWORD pid_type;
#else /* defined(_WIN32) */
/// <summary>
/// The type used to represent a process ID.
/// </summary>
typedef pid_t pid_type;
#endif /* defined(_WIN32) */

/// <summary>
/// Gets the command line of the calling process.
/// </summary>
/// <returns>The command line of the calling process.</returns>
LYRA_TEST_API std::string get_command_line(void);

/// <summary>
/// Gets the ID of the calling process.
/// </summary>
/// <returns>The process ID.</returns>
LYRA_TEST_API pid_type get_process_id(void) noexcept;

/// <summary>
/// Gets the full path to the executable of the process with the given ID.
/// </summary>
/// <param name="pid">A process ID.</param>
/// <returns>The path to the executable of the specified process.</returns>
LYRA_TEST_API std::string get_executable_path(_In_ const pid_type pid);

/// <summary>
/// Gets the full path of the executable of the calling process.
/// </summary>
/// <returns>The path to the executable of the calling process.</returns>
LYRA_TEST_API std::string get_executable_path(void);

/// <summary>
/// Gets a list of paths to all DLLs loaded into the calling process at the
/// time of the call.
/// </summary>
/// <returns>A list of paths to the DLLs loaded whan calling the function.
/// </returns>
LYRA_TEST_API std::vector<std::string> get_loaded_library_paths(void);

#if defined(_WIN32)
/// <summary>
/// Gets the full path to the given module.
/// </summary>
/// <param name="handle">An optional handle to the module. If not
/// specified, the module of the executing process is used..</param>
/// <returns>The path to the specified module.</returns>
LYRA_TEST_API std::string get_module_path(_In_opt_ HMODULE handle = NULL);
#endif /* defined(_WIN32) */

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_CALLING_PROCESS_H) */
