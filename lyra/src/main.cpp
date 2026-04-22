// <copyright file="main.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/api.h"


#if defined(_WIN32)

/// <summary>
/// Entry point of the DLL.
/// </summary>
/// <param name="hInst"></param>
/// <param name="reason"></param>
/// <param name="reserved"></param>
/// <returns></returns>
BOOL WINAPI DllMain(
        _In_ HINSTANCE hInst,
        _In_ DWORD reason,
        _In_opt_ LPVOID reserved) {

    switch (reason) {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            if (peserved == nullptr) {
            }
            break;
    }

    return TRUE;
}

#endif /* defined(_WIN32) */
