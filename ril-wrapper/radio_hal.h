/*
    Copyright (C) 2024 The LineageOS Project

    SPDX-License-Identifier: Apache-2.0
*/

#pragma once

/*
    This file contains needed structs from android.hardware.radio@1.4.
    As this file is written in C, the actual HAL in question cannot be directly linked.
*/


// The following structs are based on android.hardware.radio@1.0.

typedef struct {
    uint32_t signalStrength;              // Valid values are (0-61, 99) as defined in
                                          // TS 27.007 8.69; INT_MAX means invalid/unreported.
    uint32_t bitErrorRate;                // bit error rate (0-7, 99) as defined in TS 27.007 8.5;
                                          // INT_MAX means invalid/unreported.
    int32_t timingAdvance;                // Timing Advance in bit periods. 1 bit period = 48/13 us.
                                          // INT_MAX means invalid/unreported.
} Radio_V1_0_GsmSignalStrength;

typedef struct {
    uint32_t dbm;                         // This value is the actual RSSI
                                          // value multiplied by -1. Example: If the
                                          // actual RSSI is -75, then this response value will
                                          // be 75. INT_MAX means invalid/unreported.
    uint32_t ecio;                        // This value is the actual
                                          // Ec/Io multiplied by -10. Example: If the
                                          // actual Ec/Io is -12.5 dB, then this response value
                                          // will be 125. INT_MAX means invalid/unreported.
} Radio_V1_0_CdmaSignalStrength;

typedef struct {
    uint32_t dbm;                         // This value is the actual
                                          // RSSI value multiplied by -1.
                                          // Example: If the actual RSSI is -75,
                                          // then this response value will be 75; INT_MAX means
                                          // invalid/unreported.
    uint32_t ecio;                        // This value is the actual
                                          // Ec/Io multiplied by -10. Example: If the
                                          // actual Ec/Io is -12.5 dB, then this response value
                                          // will be 125; INT_MAX means invalid/unreported.
    uint32_t signalNoiseRatio;            // Valid values are 0-8. 8 is the highest signal to
                                          // noise ratio; INT_MAX means invalid/unreported.
} Radio_V1_0_EvdoSignalStrength;

typedef struct {
    uint32_t signalStrength;              // Valid values are (0-31, 99) as defined in
                                          // TS 27.007 8.5; INT_MAX means invalid/unreported.
    uint32_t rsrp;                        // The current Reference Signal Receive Power in dBm
                                          // multipled by -1.
                                          // Range: 44 to 140 dBm;
                                          // INT_MAX: 0x7FFFFFFF denotes invalid/unreported value.
                                          // Reference: 3GPP TS 36.133 9.1.4
    uint32_t rsrq;                        // The current Reference Signal Receive Quality in dB
                                          // multiplied by -1.
                                          // Range: 20 to 3 dB;
                                          // INT_MAX: 0x7FFFFFFF denotes invalid/unreported value.
                                          // Reference: 3GPP TS 36.133 9.1.7
    int32_t rssnr;                        // The current reference signal signal-to-noise ratio in
                                          // 0.1 dB units.
                                          // Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).
                                          // INT_MAX: 0x7FFFFFFF denotes invalid/unreported value.
                                          // Reference: 3GPP TS 36.101 8.1.1
    uint32_t cqi;                         // The current Channel Quality Indicator.
                                          // Range: 0 to 15.
                                          // INT_MAX: 0x7FFFFFFF denotes invalid/unreported value.
                                          // Reference: 3GPP TS 36.101 9.2, 9.3, A.4
    uint32_t timingAdvance;               // timing advance in micro seconds for a one way trip
                                          // from cell to device.
                                          // Approximate distance is calculated using
                                          // 300m/us * timingAdvance.
                                          // Range: 0 to 1282 inclusive.
                                          // INT_MAX: 0x7FFFFFFF denotes invalid/unreported value.
                                          // Reference: 3GPP 36.213 section 4.2.3
} Radio_V1_0_LteSignalStrength;


// The following structs are based on android.hardware.radio@1.2.

typedef struct {
    int32_t signalStrength;               // Valid values are (0-31, 99) as defined in
                                          // TS 27.007 8.5; INT_MAX means unreported.
    int32_t bitErrorRate;                 // bit error rate (0-7, 99) as defined in TS 27.007 8.5;
                                          // INT_MAX means invalid/unreported.
    /**
     * CPICH RSCP as defined in TS 25.215 5.1.1
     * Valid values are (0-96, 255) as defined in TS 27.007 8.69
     * INT_MAX denotes that the value is invalid/unreported.
     */
    uint32_t rscp;
    /**
     * Ec/No value as defined in TS 25.215 5.1.5
     * Valid values are (0-49, 255) as defined in TS 27.007 8.69
     * INT_MAX denotes that the value is invalid/unreported.
     */
    uint32_t ecno;
} Radio_V1_2_WcdmaSignalStrength;

typedef struct {
    /**
     * UTRA carrier RSSI as defined in TS 25.225 5.1.4
     * Valid values are (0-31, 99) as defined in TS 27.007 8.5
     * INT_MAX denotes that the value is invalid/unreported.
     */
    uint32_t signalStrength;
    /**
     * Transport Channel BER as defined in TS 25.225 5.2.5
     * Valid values are (0-7, 99) as defined in TS 27.007 8.5
     * INT_MAX denotes that the value is invalid/unreported.
     */
    uint32_t bitErrorRate;
    /**
     * P-CCPCH RSCP as defined in TS 25.225 5.1.1
     * Valid values are (0-96, 255) as defined in TS 27.007 8.69
     * INT_MAX denotes that the value is invalid/unreported.
     */
    uint32_t rscp;
} Radio_V1_2_TdscdmaSignalStrength;


// The following structs are based on android.hardware.radio@1.4.

typedef struct {
    /**
     * SS reference signal received power, multipled by -1.
     *
     * Reference: 3GPP TS 38.215.
     *
     * Range [44, 140], INT_MAX means invalid/unreported.
     */
    int32_t ssRsrp;

    /**
     * SS reference signal received quality, multipled by -1.
     *
     * Reference: 3GPP TS 38.215, 3GPP TS 38.133 section 10.
     *
     * Range [-20 dB, 43 dB], INT_MAX means invalid/unreported.
     */
    int32_t ssRsrq;

    /**
     * SS signal-to-noise and interference ratio.
     *
     * Reference: 3GPP TS 38.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
     *
     * Range [-23, 40], INT_MAX means invalid/unreported.
     */
    int32_t ssSinr;

    /**
     * CSI reference signal received power, multipled by -1.
     *
     * Reference: 3GPP TS 38.215.
     *
     * Range [44, 140], INT_MAX means invalid/unreported.
     */
    int32_t csiRsrp;

    /**
     * CSI reference signal received quality, multipled by -1.
     *
     * Reference: 3GPP TS 38.215.
     *
     * Range [3, 20], INT_MAX means invalid/unreported.
     */
    int32_t csiRsrq;

    /**
     * CSI signal-to-noise and interference ratio.
     *
     * Reference: 3GPP TS 138.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
     *
     * Range [-23, 40], INT_MAX means invalid/unreported.
     */
    int32_t csiSinr;
} Radio_V1_4_NrSignalStrength;

typedef struct {
    Radio_V1_0_GsmSignalStrength GW_SignalStrength;
    Radio_V1_0_CdmaSignalStrength CDMA_SignalStrength;
    Radio_V1_0_EvdoSignalStrength EVDO_SignalStrength;
    Radio_V1_0_LteSignalStrength LTE_SignalStrength;
    Radio_V1_2_TdscdmaSignalStrength TD_SCDMA_SignalStrength;
    Radio_V1_2_WcdmaSignalStrength WCDMA_SignalStrength;
    Radio_V1_4_NrSignalStrength NR_SignalStrength;
} Radio_V1_4_SignalStrength;

// This struct is found in vendor.lge.hardware.radio@2.0.
typedef struct {
    Radio_V1_0_GsmSignalStrength GW_SignalStrength;
    int32_t timingAdvance;
    int32_t dbm;
    int32_t ecio;
    int32_t ecno;
} Lge_Radio_V2_0_LgeGsmSignalStrength;

// This struct is found in vendor.lge.hardware.radio@2.0.
typedef struct {
    Lge_Radio_V2_0_LgeGsmSignalStrength GW_SignalStrength;
    Radio_V1_0_CdmaSignalStrength CDMA_SignalStrength;
    Radio_V1_0_EvdoSignalStrength EVDO_SignalStrength;
    Radio_V1_0_LteSignalStrength LTE_SignalStrength;
    Radio_V1_2_TdscdmaSignalStrength TD_SCDMA_SignalStrength;
    Radio_V1_2_WcdmaSignalStrength WCDMA_SignalStrength;
    Radio_V1_4_NrSignalStrength NR_SignalStrength;
} Lge_Radio_V2_0_LgeSignalStrength;
