// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌────┬────┬────┬────┬────┬┬────┬────┬────┬────┬┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┐
     * │Esc │ F1 │ F2 │ F3 │ F4 ││ F5 │ F6 │ F7 │ F8 ││ F9 │ F10│ F11│ F12││    │    │    │    │    │    │
     * ├────┼────┴────┴────┴────┴┴────┴────┴────┴────┴┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┘
     * ├────┼┬────┬────┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┐
     * │    ││ ^  │ 1  │ 2  │ 3  │ 4  │ 5  ││ 6  │ 7  │ 8  │ 9  │ 0  │ -  ││ Bck│ *  │ /  │ -  │ +  │PgUp│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ -> │ X  │ L  │ V  │ C  │ W  ││ K  │ H  │ G  │ F  │ Q  │ ß  ││ Del│ 7  │ 8  │ 9  │ %  │PgDn│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ M3 │ U  │ I  │ A  │ E  │ O  ││ S  │ N  │ R  │ T  │ D  │ Y  ││Home│ 4  │ 5  │ 6  │ .  │    │
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ LSh│ Ü  │ Ö  │ Ä  │ P  │ Z  ││ B  │ M  │ ,  │ .  │ J  │ RSh││ End│ 1  │ 2  │ 3  │ Up │ Ret│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ Ctl│ Fn │ Alt│ M4 │ Cmd│ Sp ││ Ret│ M4 │RAlt│    │ Fn │ M3 ││    │ 0  │ ,  │ Lt │ Dn │ Rt │
     * └────┴┴────┴────┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┘
     */

    [0] = LAYOUT_ortho_6x19(
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S,
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S,
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S,
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S,
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S,
        KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    )
};
