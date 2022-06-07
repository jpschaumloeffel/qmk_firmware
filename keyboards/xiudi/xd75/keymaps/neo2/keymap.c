/* Copyright 2020 jsch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * 4-layer Neo2 implementation for xd75 (and possibly a base for Iris) using unicode input mode on mac
 * unicode input mode uses some sort of basic osx us layout and allows sending unicode with option + utf16
 * 
 * So to get all the funky neo chars working, I just remap the basic stuff and then fix it up with unicode
 * 
 * idea for dead keys:
 * create one-shot modifiers for the keys and map the resulting unicode chars in one layer for each
 */

#include QMK_KEYBOARD_H

#ifndef KEYMAP_NEO2
#define KEYMAP_NEO2

#include "keymap.h"

#endif

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
    /* Neo2 layer 1
     * ------------------------------------------------------------------------------------------------------------.--------------------------.
     * | ESC    | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      | 0      | -      | BACKSP | `      | Screen |
     * +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+--------+--------|
     * | TAB    | X      | V      | L      | C      | W      | K      | H      | G      | F      | Q      | ß      | ´      | PgUp   | PgDn   |
     * +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+-----------------|
     * | Mod3   | U      | I      | A      | *E     | O      | S      | *N     | R      | T      | D      | Y      | Mod3   | HOME   | END    |
     * +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------------------------|
     * | LShift | ü      | ö      | ä      | P      | Z      | B      | M      | ,      | .      | J      | RShift | RShift | UP     | ENTER  |
     * +--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------|-----------------+--------|
     * | Ctrl   | FN     | Alt    | Mod4   | Cmd    | SPACE  | ENTER  | Mod4   | RAlt   | FN     |        |        | LEFT   | DOWN   | RIGHT  |
     * ------------------------------------------------------------------------------------------------------------'--------------------------'
     */
    [_NL1] = LAYOUT_ortho_5x15( /* Neo2 layer 1 */
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_BSPC, KC_GRAVE, KC_SSHT,
        KC_TAB,  KC_X,    KC_V,    KC_L,    KC_C,    KC_W,    KC_K,    KC_H,    KC_G,    KC_F,    KC_Q,    UC_SS,   UC_ACUT, KC_PGUP, KC_PGDN,
        KC_MOD3, KC_U,    KC_I,    KC_A,    KC_E,    KC_O,    KC_S,    KC_N,    KC_R,    KC_T,    KC_D,    KC_Y,    KC_MOD3, KC_HOME, KC_END,
        KC_LNSH, UC_UE,   UC_OE,   UC_AE,   KC_P,    KC_Z,    KC_B,    KC_M,    KC_COMM, KC_DOT,  KC_J,    KC_RNSH, KC_RNSH, KC_UP,   KC_ENT,
        KC_LCTL, MO(_FN), KC_LALT, KC_MOD4, KC_LGUI, KC_SPC,  KC_ENT,  KC_MOD4, KC_RALT, MO(_FN), KC_NO,   KC_NO,   KC_LEFT, KC_DOWN, KC_RGHT
    ),

    /* Neo2 layer 2, where differing from US/Mac
     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        | °      | §      | ℓ      | »      | «      | $      | €      | „      | “      | ”      | —      | ¸      |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |        |        |        | ẞ      | ˜      |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        | *      |        |        | *      |        |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | Ü      | Ö      | Ä      |        |        |        |        | –      | •      |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_NL2] = LAYOUT_ortho_5x15( /* Neo2 layer 2 */
        _______, UC_DEG,  UC_SECT, UC_SCRL, UC_RDAQ, UC_LDAQ, UC_DLR , UC_EURO, UC_LLDQ, UC_LDQ , UC_RDQ , UC_EMDS, UC_CED ,_______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, UC_SS_C, UC_STIL,_______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______, _______,
        _______, UC_UE_C, UC_OE_C, UC_AE_C, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______, _______
    ),

    /* Neo2 layer 3
     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        | ¹      | ²      | ³      | ›      | ‹      | ¢      | ¥      | ‚      | ‘      | ’      |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | …      | _      | [      | ]      | ^      | !      | <      | >      | =      | &      |  ſ     | /      |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
     * |        | \      | /      | {      | *}     | *      | ?      | *(     | )      | -      | :      | @      |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+-----------------|
     * |        | #      | $      | |      | ~      | `      | +      | %      | "      | '      | ;      |        |        |        |        |
     * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
     * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_NL3] = LAYOUT_ortho_5x15( /* Neo2 layer 3 */
        _______, UC_SUP1, UC_SUP2, UC_SUP3, UC_SLAQ, UC_SRAQ, UC_CENT, UC_YEN , UC_LLSQ, UC_LSQ , UC_RSQ , _______, _______,_______, _______,
        _______, UC_ELLP, KC_UNDS, KC_LBRC, KC_RBRC, KC_CIRC, KC_EXLM, KC_LABK, KC_RABK, KC_EQL , KC_AMPR, UC_LNGS, KC_SLSH,_______, _______,
        _______, KC_BSLS, KC_SLSH, KC_LCBR, KC_RCBR, KC_ASTR, KC_QUES, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN, KC_AT  , _______,_______, _______,
        _______, KC_HASH, KC_DLR , KC_PIPE, KC_TILD, KC_GRAVE,KC_PLUS, KC_PERC, KC_DQUO, KC_QUOT, KC_SCLN, _______, _______,_______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______, _______
    ),

    /* Neo2 layer 4
     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        | Tab    | /      | *      | -      |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | PgUp   | Backsp | Up     | Del    | PgDn   |        | 7      | 8      | 9      | +      |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
     * |        | Home   | Left   | Down   | *Right | End    |        | *4     | 5      | 6      |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------|
     * |        |        | Tab    | Ins    | Enter  |        | :      | 1      | 2      | 3      |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------|
     * |        |        |        |        |        |        |        |        | 0      |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_NL4] = LAYOUT_ortho_5x15( /* Neo2 layer 4 */
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,
        _______, KC_PGUP, KC_BSPC, KC_UP  , KC_DEL , KC_PGDN, _______, KC_7   , KC_8   , KC_9   , KC_PLUS, _______, _______, _______, _______,
        _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END , _______, KC_4   , KC_5   , KC_6   , _______, _______, _______, _______, _______,
        _______, _______, KC_TAB , KC_INS , KC_ENT , _______, KC_COLN, KC_1   , KC_2   , KC_3   , _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, KC_0   , _______, _______, _______, _______, _______, _______, _______
    ),

    /* FUNCTION
     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * | F1     | F2     | F3     | F4     | F5     | F6     | NUM LK | P/     | P*     | F7     | F8     | F9     | F10    | F11    | F12    |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * | SELECT | CALC   | MYCOMP | MAIL   | RGB HD | RGB HI | P7     | P8     | P9     | -      |        |        | PR SCR | SCR LK | PAUSE  |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * | PREV   | PLAY   | NEXT   | STOP   | RGB SD | RGB SI | P4     | P5     | P6     | +      |        | RESET  |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * | VOL-   | MUTE   | VOL+   | APP    | RGB VD | RGB VI | P1     | P2     | P3     | PENT   |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        | RGB TG |        | RGB RMD| RGB MD | P0     |        | P.     | PENT   | PENT   | FN     |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, RESET,
        _______, _______, _______, _______, RGB_HUD, RGB_HUI, _______, _______, _______, _______, _______, _______, _______, _______, UC_MOD,
        _______, RGB_M_G, RGB_M_R, _______, RGB_SAD, RGB_SAI, _______, _______, _______, _______, _______, _______, _______, _______, DEBUG,
        _______, RGB_M_SN,RGB_M_P, _______, RGB_VAD, RGB_VAI, _______, _______, _______, _______, _______, _______, KC_MUTE, KC_VOLU, KC_MPLY,
        _______, MO(_FN), RGB_TOG, _______, RGB_RMOD,RGB_MOD, _______, _______, _______, MO(_FN), _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
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