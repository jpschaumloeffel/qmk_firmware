#include "keymap_german.h"

#include "iris.h"
#include "action_layer.h"
#include "eeconfig.h"


extern keymap_config_t keymap_config;

#define _NEO2 0
#define _LAYER2 1
#define _LAYER3 2
#define _LAYER4 16

enum custom_keycodes {
  NEO2 = SAFE_RANGE,
  MOD3,
  MOD4,
};

#define KC_ KC_TRNS
#define _______ KC_TRNS

#define KC_MOD3 MOD3
#define KC_MOD4 MOD4

#define KC_AE KC_QUOT
#define KC_OE KC_SCLN
#define KC_UE KC_LBRC
#define KC_SS KC_MINS


#define KC__SL DE_SLSH
#define KC__BSL DE_BSLS
#define KC__LPR LSFT(KC_8)
#define KC__RPR LSFT(KC_9)



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_NEO2] = KC_KEYMAP(
  //,------+------+------+------+------+------.                     ,------+------+------+------+------+------.
      ESC  ,  1   ,  2   ,  3   ,  4   ,  5   ,                        6   ,  7   ,  8   ,  9   ,  0   , BSPC ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
      TAB  ,  X   ,  V   ,  L   ,  C   ,  W   ,                        K   ,  H   ,  G   ,  F   ,  Q   , SS   ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
      MOD3 ,  U   ,  I   ,  A   ,  E   ,  O   ,                        S   ,  N   ,  R   ,  T   ,  D   ,  Z   ,
  //|------+------+------+------+------+------+------.       ,------|------+------+------+------+------+------|
      LSFT , UE   , OE   , AE   ,  P   ,  Y   , MOD4  ,        DEL  ,  B   ,  M   , COMM , DOT  ,  J   , RSFT ,
  //`------+------+------+------+------+------+------/       \------+------+------+------+------+------+------'
                                  LCTL , SPC  , LALT,           LGUI, ENT  , MOD4
  //                            `------+------+----'          `-----+------+------'
  ),


  // differences in shifted layer
  [_LAYER2] = KC_KEYMAP(
  //,------+------+------+------+------+------.                     ,------+------+------+------+------+------.
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------+------.       ,------|------+------+------+------+------+------|
           ,      ,      ,      ,      ,      ,      ,              ,      ,      ,      ,      ,      ,      ,
  //`------+------+------+------+------+------+------/       \------+------+------+------+------+------+------'
                                       ,      ,     ,               ,      ,  
  //                            `------+------+----'          `-----+------+------'
  ),

  [_LAYER3] = KC_KEYMAP(
  //,------+------+------+------+------+------.                     ,------+------+------+------+------+------.
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           , _BSL , _SL  ,      ,      ,      ,                            , _LPR , _RPR ,      ,      ,      ,
  //|------+------+------+------+------+------+------.       ,------|------+------+------+------+------+------|
           ,      ,      ,      ,      ,      ,      ,              ,      ,      ,      ,      ,      ,      ,
  //`------+------+------+------+------+------+------/       \------+------+------+------+------+------+------'
                                       ,      ,     ,               ,      ,  
  //                            `------+------+----'          `-----+------+------'
  ),


  [_LAYER4] = KC_KEYMAP(
  //,------+------+------+------+------+------.                     ,------+------+------+------+------+------.
           ,      ,      ,      ,      ,      ,                            ,      ,      ,      ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           , PGUP , BSPC ,  UP  , DEL  , PGDN ,                            ,  7   ,  8   ,  9   ,      ,      ,
  //|------+------+------+------+------+------|                     |------+------+------+------+------+------|
           , HOME , LEFT , DOWN , RIGHT,  END ,                            ,  4   ,  5   ,  6   ,      ,      ,
  //|------+------+------+------+------+------+------.       ,------|------+------+------+------+------+------|
           ,      ,      ,      , ENT  ,      ,      ,              ,      ,  1   ,  2   ,  3   ,      ,      ,
  //`------+------+------+------+------+------+------/       \------+------+------+------+------+------+------'
                                       ,      ,     ,               ,  0   ,  
  //                            `------+------+----'          `-----+------+------'
  )
};
#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LSFT:
      if (record->event.pressed) {
        layer_on(_LAYER2);
        //update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LAYER2);
        //update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return true;
      break;
    case MOD3:
      if (record->event.pressed) {
        layer_on(_LAYER3);
        //update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LAYER3);
        //update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case MOD4:
      if (record->event.pressed) {
        layer_on(_LAYER4);
      } else {
        layer_off(_LAYER4);
      }
      return false;
      break;
  }
  return true;
}


/*
bool override_key(keyrecord_t* record, uint16_t normal, uint16_t shifted) {
    const uint8_t shift = MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT);
    // Todo, should maybe also check at least the weak mods
    uint8_t current_mods = get_mods();
    if (record->event.pressed) {
        // Todo share this code with send keyboard report
#ifndef NO_ACTION_ONESHOT
        if (get_oneshot_mods()) {
#if (defined(ONESHOT_TIMEOUT) && (ONESHOT_TIMEOUT > 0))
            if (has_oneshot_mods_timed_out()) {
                dprintf("Oneshot: timeout\n");
                clear_oneshot_mods();
            }
#endif
            current_mods |= get_oneshot_mods();
            clear_oneshot_mods();
        }
#endif
        bool shift_pressed = current_mods & shift;
        const uint16_t target = shift_pressed ? shifted : normal;
        uint8_t keycode = target & 0xFF;
        if (keycode == KC_NO) {
            return false;
        }
        press_key_with_level_mods(target);
    }
    return false;
}

static bool wake_dead_key(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        register_code16(keycode);
        unregister_code16(keycode);
        register_code16(KC_SPACE);
        unregister_code16(KC_SPACE);
    }
    // Let QMK handle onshots
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
    case APQU:
        return override_key(record, SV_APOS, SV_QUOT);
    case TILD:
        return wake_dead_key(SV_TILD, record);
    case CIRC:
        return wake_dead_key(SV_CIRC, record);
    case ACUT:
        return wake_dead_key(SV_ACUT, record);
    case GRAV:
        return wake_dead_key(SV_GRAV, record);
    }
    return true;
}
*/
