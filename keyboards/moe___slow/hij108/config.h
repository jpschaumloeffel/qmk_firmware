// Copyright 2025 Jan Schaumlöffel (@Jan Schaumlöffel)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define GPIO_ROW0 GP29
#define GPIO_ROW1 GP20
#define GPIO_ROW2 GP18
#define GPIO_ROW3 GP26
#define GPIO_ROW4 GP27
#define GPIO_ROW5 GP28

#define GPIO_COL0  GP1
#define GPIO_COL2  GP0
#define GPIO_COL4  GP2
#define GPIO_COL6  GP3
#define GPIO_COL8  GP4
#define GPIO_COL10 GP5
#define GPIO_COL12 GP6
#define GPIO_COL14 GP7
#define GPIO_COL16 GP8

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
