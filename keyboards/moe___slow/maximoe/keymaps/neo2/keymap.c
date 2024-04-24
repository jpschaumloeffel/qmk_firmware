// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include "keymap_us.h"
#include QMK_KEYBOARD_H

enum layer_names {
    _NL1,
    _NL2,
    _NL3,
    _NL4,
    _FN,
};

// #define UC_ UC(0x0000)

// ümlauts
#define UC_AE     UC(0x00E4)
#define UC_SS     UC(0x00DF)
#define UC_OE     UC(0x00F6)
#define UC_UE     UC(0x00FC)
#define UC_AE_C   UC(0x00C4)
#define UC_SS_C   UC(0x1E9E)
#define UC_OE_C   UC(0x00D6)
#define UC_UE_C   UC(0x00DC)

// unsure - should use shifted or unicode? hotkeys? deadkeys?
#define UC_CIRC  UC(0x005E) // ^  // TODO should be deadkey
#define UC_ACUT  UC(0x00B4) // ´  // TODO should be deadkey
#define UC_GRAV  UC(0x0060) // `  // TODO should be deadkey

// number row shifted
#define UC_DEG   UC(0x00B0) // °  // TODO should be deadkey
#define UC_SECT  UC(0x00A7) // §
#define UC_SCRL  UC(0x2113) // ℓ
#define UC_RDAQ  UC(0x00BB) // »
#define UC_LDAQ  UC(0x00AB) // «
#define UC_DLR   UC(0x0024) // $  // KC_DLR?
#define UC_EURO  UC(0x20AC) // €
#define UC_LLDQ  UC(0x201E) // „
#define UC_LDQ   UC(0x201C) // “
#define UC_RDQ   UC(0x201D) // ”
#define UC_EMDS  UC(0x2014) // —
#define UC_CED   UC(0x00B8) // ¸  // TODO should be deadkey

#define UC_STIL  UC(0x02DC) // ˜  // TODO should be deadkey

#define UC_ENDS  UC(0x2013) // –
#define UC_BULT  UC(0x2022) // •


// layer 3 funky stuff
#define UC_BSLS  UC(0x005C)
#define UC_PIPE  UC(0x007C)
#define UC_TILD  UC(0x007E)
#define UC_SUP1  UC(0x00B9) // ¹
#define UC_SUP2  UC(0x00B2) // ²
#define UC_SUP3  UC(0x00B3) // ³
#define UC_SRAQ  UC(0x203A) // ›
#define UC_SLAQ  UC(0x2039) // ‹
#define UC_CENT  UC(0x00A2) // ¢
#define UC_YEN   UC(0x00A5) // ¥
#define UC_LLSQ  UC(0x201A) // ‚
#define UC_LSQ   UC(0x2018) // ‘
#define UC_RSQ   UC(0x2019) // ’
#define UC_ELLP  UC(0x2026) // …
#define UC_LNGS  UC(0x017F) // ſ

#define UC_MINS  UC(0x002D) // - because of :_) rolloff problem, should debug why that happens


// neo-shift -> go into layer 2
#define KC_LNSH LM(_NL2, MOD_LSFT)
#define KC_RNSH LM(_NL2, MOD_RSFT)

// neo mod 3 -> go into layer 3, no modifier applied
#define KC_MOD3 MO(_NL3)

// neo mod 4 -> go into layer 4, no modifier applied
#define KC_MOD4 MO(_NL4)

// take screenshot on mac
#define KC_SSHT C(S(G(KC_4)))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌────┬────┬────┬────┬────┬┬────┬────┬────┬────┬┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┐
     * │Esc │ F1 │ F2 │ F3 │ F4 ││ F5 │ F6 │ F7 │ F8 ││ F9 │ F10│ F11│ F12││    │    │    │    │    │    │
     * ├────┼────┴────┴────┴────┴┴────┴────┴────┴────┴┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┘
     * ├────┼┬────┬────┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┬┬────┬────┬────┬────┬────┬────┐
     * │    ││ ^  │ 1  │ 2  │ 3  │ 4  │ 5  ││ 6  │ 7  │ 8  │ 9  │ 0  │ -  ││ Bck│ *  │ /  │ -  │ +  │PgUp│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ -> │ X  │ V  │ L  │ C  │ W  ││ K  │ H  │ G  │ F  │ Q  │ ß  ││ Del│ 7  │ 8  │ 9  │ %  │PgDn│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ M3 │ U  │ I  │ A  │ E  │ O  ││ S  │ N  │ R  │ T  │ D  │ Y  ││Home│ 4  │ 5  │ 6  │ .  │Num │
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ LSh│ Ü  │ Ö  │ Ä  │ P  │ Z  ││ B  │ M  │ ,  │ .  │ J  │ RSh││ End│ 1  │ 2  │ 3  │ Up │ Ret│
     * ├────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┼┼────┼────┼────┼────┼────┼────┤
     * │    ││ Ctl│ Fn │ Alt│ M4 │ Cmd│ Sp ││ Ret│ M4 │RAlt│    │ Fn │ M3 ││    │ 0  │ ,  │ Lt │ Dn │ Rt │
     * └────┴┴────┴────┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┴┴────┴────┴────┴────┴────┴────┘
     */

    [_NL1] = LAYOUT_ortho_6x19(
        KC_ESC , KC_F1  , KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______,
        _______, KC_CIRC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_BSPC, KC_PAST, KC_PSLS, KC_MINS, KC_PLUS, KC_PGUP,
        _______, KC_TAB,  KC_X,    KC_V,    KC_L,    KC_C,    KC_W,    KC_K,    KC_H,    KC_G,    KC_F,    KC_Q,    UC_SS,   KC_DEL,  KC_P7,   KC_P8,   KC_P9,   KC_PERC, KC_PGDN,
        _______, KC_MOD3, KC_U,    KC_I,    KC_A,    KC_E,    KC_O,    KC_S,    KC_N,    KC_R,    KC_T,    KC_D,    KC_Y,    KC_HOME, KC_P4,   KC_P5,   KC_P6,   KC_PDOT, KC_NUM,
        _______, KC_LNSH, UC_UE,   UC_OE,   UC_AE,   KC_P,    KC_Z,    KC_B,    KC_M,    KC_COMM, KC_DOT,  KC_J,    KC_RNSH, KC_END,  KC_P1,   KC_P2,   KC_P3,   KC_UP,   KC_PENT,
        _______, KC_LCTL, MO(_FN), KC_LALT, KC_MOD4, KC_LGUI, KC_SPC,  KC_ENT,  KC_MOD4, KC_RALT, _______, MO(_FN), KC_MOD3, _______, KC_P0,   KC_COMM, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_NL2] = LAYOUT_ortho_6x19(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, UC_DEG,  UC_SECT, UC_SCRL, UC_RDAQ, UC_LDAQ, UC_DLR , UC_EURO, UC_LLDQ, UC_LDQ , UC_RDQ , UC_EMDS, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, UC_SS_C, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, UC_UE_C, UC_OE_C, UC_AE_C, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_NL3] = LAYOUT_ortho_6x19(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, UC_SUP1, UC_SUP2, UC_SUP3, UC_SLAQ, UC_SRAQ, UC_CENT, UC_YEN , UC_LLSQ, UC_LSQ , UC_RSQ , _______, _______, _______, _______, _______, _______, _______,
        _______, _______, UC_ELLP, KC_UNDS, KC_LBRC, KC_RBRC, KC_CIRC, KC_EXLM, KC_LABK, KC_RABK, KC_EQL , KC_AMPR, UC_LNGS, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_BSLS, KC_SLSH, KC_LCBR, KC_RCBR, KC_ASTR, KC_QUES, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN, KC_AT  , _______, _______, _______, _______, _______, _______,
        _______, _______, KC_HASH, KC_DLR , KC_PIPE, KC_TILD, KC_GRAVE,KC_PLUS, KC_PERC, KC_DQUO, KC_QUOT, KC_SCLN, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_NL4] = LAYOUT_ortho_6x19(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_PGUP, KC_BSPC, KC_UP  , KC_DEL , KC_PGDN, _______, KC_7   , KC_8   , KC_9   , KC_PLUS, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END , _______, KC_4   , KC_5   , KC_6   , _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_TAB , KC_INS , KC_ENT , _______, KC_COLN, KC_1   , KC_2   , KC_3   , _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_0   , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_FN] = LAYOUT_ortho_6x19(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, UC_PREV, UC_NEXT, QK_RBT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch(keycode) {
        case KC_MINS:
            unregister_mods(MOD_MASK_SHIFT);
            unregister_weak_mods(MOD_MASK_SHIFT);
            return true;
    }
    return true;
}
