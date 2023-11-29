// Copyright 2023 Jan Schaumlöffel (@jpschaumloeffel)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG
#define DEBUG
#define DEBUG_MATRIX_SCAN_RATE

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

// #define I2C1_SDA_PIN GP1
// #define I2C1_SCL_PIN GP0

#define I2C1_OPMODE OPMODE_I2C
#define I2C1_CLOCK_SPEED 100000

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_WINCOMPOSE, UNICODE_MODE_MACOS
