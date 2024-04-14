/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "ril-wrapper"

/*
 * We're using RIL_Env, which is only exposed if this is defined.
 */
#define RIL_SHLIB

#include <log/log.h>
#include <telephony/ril.h>

#include <dlfcn.h>
#include <string.h>

#include "radio_hal.h"

#define RIL_LIB_NAME "libril-qc-hal-qmi.so"

#define RIL_UNSOL_LGE_SIGNAL_STRENGTH 1300

static const RIL_RadioFunctions* qmiRilFunctions;
static const struct RIL_Env* qcRilEnv;

static void onUnsolicitedResponseShim(int unsolResponse, const void* data, size_t datalen) {
    void * shimmedResponseData = NULL;
    size_t shimmedResponseDataLen;

    ALOGE("%s: RECEIVED UNSOL %d\n", __func__, unsolResponse);
    switch (unsolResponse) {
        case RIL_UNSOL_LGE_SIGNAL_STRENGTH: {
           ALOGE("%s: RECEIVED RIL_UNSOL_LGE_SIGNAL_STRENGTH\n", __func__);
            // Convert to RIL_UNSOL_SIGNAL_STRENGTH so that AOSP understands
            unsolResponse = RIL_UNSOL_SIGNAL_STRENGTH;
            Lge_Radio_V2_0_LgeSignalStrength* lgeSignalStrength = (Lge_Radio_V2_0_LgeSignalStrength*)data;

            // Create an AOSP-style SignalStrength and insert needed values.
            Radio_V1_4_SignalStrength newSignalStrength = {
                lgeSignalStrength->GW_SignalStrength.GW_SignalStrength,
                lgeSignalStrength->CDMA_SignalStrength,
                lgeSignalStrength->EVDO_SignalStrength,
                lgeSignalStrength->LTE_SignalStrength,
                lgeSignalStrength->TD_SCDMA_SignalStrength,
                lgeSignalStrength->WCDMA_SignalStrength,
                lgeSignalStrength->NR_SignalStrength
            };
            shimmedResponseData = malloc(sizeof(Radio_V1_4_SignalStrength));
            shimmedResponseDataLen = sizeof(Radio_V1_4_SignalStrength);
            __builtin_memcpy(shimmedResponseData, &newSignalStrength, shimmedResponseDataLen);
            break;
        }
        default:
            shimmedResponseData = malloc(datalen);
            __builtin_memcpy(shimmedResponseData, data, datalen);
            shimmedResponseDataLen = datalen;
            break;
    }

    // Hand over to qcril with a potentially modified UNSOL.
    qcRilEnv->OnUnsolicitedResponse(unsolResponse, shimmedResponseData, shimmedResponseDataLen);

    // Clean up our own data pointer
    free(shimmedResponseData);
}

const RIL_RadioFunctions* RIL_Init(const struct RIL_Env* env, int argc, char** argv) {
    RIL_RadioFunctions const* (*qmiRilInit)(const struct RIL_Env* env, int argc, char** argv);
    static struct RIL_Env shimmedRilEnv;
    void* qmiRil;

    ALOGE("%s: STARTING LGE RIL WRAPPER!!!!\n", __func__);
    /*
     * Save the RilEnv passed from rild.
     */
    qcRilEnv = env;

    /*
     * Copy the RilEnv and shim the OnUnsolicitedResponse function.
     */
    shimmedRilEnv = *env;
    shimmedRilEnv.OnUnsolicitedResponse = onUnsolicitedResponseShim;

    /*
     * Open the qmi RIL.
     */
    qmiRil = dlopen(RIL_LIB_NAME, RTLD_LOCAL);
    if (!qmiRil) {
        ALOGE("%s: failed to load %s: %s\n", __func__, RIL_LIB_NAME, dlerror());
        return NULL;
    }

    /*
     * Get a reference to the qmi RIL_Init.
     */
    qmiRilInit = dlsym(qmiRil, "RIL_Init");
    if (!qmiRilInit) {
        ALOGE("%s: failed to find RIL_Init\n", __func__);
        goto fail_after_dlopen;
    }

    /*
     * Init the qmi RIL add pass it the shimmed RilEnv.
     */
    qmiRilFunctions = qmiRilInit(&shimmedRilEnv, argc, argv);
    if (!qmiRilFunctions) {
        ALOGE("%s: failed to get functions from RIL_Init\n", __func__);
        goto fail_after_dlopen;
    }

    return qmiRilFunctions;

fail_after_dlopen:
    dlclose(qmiRil);

    return NULL;
}
