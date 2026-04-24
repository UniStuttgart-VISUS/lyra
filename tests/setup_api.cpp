// <copyright file="setup_api.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include <gtest/gtest.h>

#include "setup_api.h"

TEST(setup_api, enumerate_gpus) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const auto class_id = GUID_DEVCLASS_DISPLAY;

    const auto cnt = enum_class_devices(&class_id,
        [](HDEVINFO handle, SP_DEVINFO_DATA& data) {
            const auto desc = get_device_registry_property(handle, data, SPDRP_DEVICEDESC);
            EXPECT_GT(desc.size(), 0);

            const auto hwid = get_device_registry_property(handle, data, SPDRP_HARDWAREID);
            EXPECT_GT(hwid.size(), 0);

            add_device_install_flags(handle, &data, 0, DI_FLAGSEX_INSTALLEDDRIVER);

            enum_driver_info(handle, &data, SPDIT_COMPATDRIVER,
                [](HDEVINFO h, PSP_DEVINFO_DATA d, SP_DRVINFO_DATA_W& drv) { return false; });

            return true;
        }, DIGCF_PRESENT | DIGCF_PROFILE);
    EXPECT_GT(cnt, 0);
}

#endif /* defined(_WIN32) */
