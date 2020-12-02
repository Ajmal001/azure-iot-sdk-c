// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothub_device_client.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include "iothub_account.h"
#include "iothubtest.h"
#include "../common_longhaul/iothub_client_common_longhaul.h"

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

#define INDEFINITE_TIME ((time_t)-1)

int main(void)
{
    int result;
    IOTHUB_LONGHAUL_RESOURCES_HANDLE iotHubLonghaulRsrcsHandle;

    if ((iotHubLonghaulRsrcsHandle = longhaul_tests_init()) == NULL)
    {
        LogError("Test failed");
        result = MU_FAILURE;
    }
    else
    {
        IOTHUB_DEVICE_CLIENT_HANDLE device_client = longhaul_initialize_device_client(iotHubLonghaulRsrcsHandle, IoTHubAccount_GetSASDevice(longhaul_get_account_info(iotHubLonghaulRsrcsHandle)), MQTT_Protocol);
        if (device_client == NULL)
        {
            LogError("Failed creating the device client");
            result = MU_FAILURE;
        }
        else
        {
#ifdef AZIOT_LINUX
            bool traceOn = true;
            IoTHubDeviceClient_SetOption(device_client, OPTION_LOG_TRACE, &traceOn);
#endif //AZIOT_LINUX
            result = longhaul_run_c2d_tests(iotHubLonghaulRsrcsHandle);
        }

        longhaul_tests_deinit(iotHubLonghaulRsrcsHandle);
    }

    return result;
}
