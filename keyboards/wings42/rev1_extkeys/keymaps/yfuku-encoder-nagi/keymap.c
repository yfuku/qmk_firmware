#include QMK_KEYBOARD_H

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
// 薙刀式


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
// 薙刀式
    _NAGINATA, // 薙刀式入力レイヤー
// 薙刀式
    _RAISE,
    _LOWER,
};

enum custom_keycodes {
  EISU = NG_SAFE_RANGE,
  KANA2,
  LCTOGL, // Macのライブ変換対応オンオフ
};
// naginata
#define KC_EISU EISU
#define KC_KANA2 KANA2

// common
#define KC_ KC_TRNS
#define KC_XXXX KC_NO
#define KC_RST RESET
#define KC_VD KC__VOLDOWN
#define KC_VU KC__VOLUP

// layer
#define KC_L_SPC LT(_LOWER, KC_SPC)
#define KC_R_ENT LT(_RAISE, KC_ENT)

// shift_t
#define KC_S_EN LSFT_T(KC_LANG2)
#define KC_S_JA LSFT_T(KC_LANG1)

// cmd_t
#define KC_G_F LCMD_T(KC_F)
#define KC_G_J RCMD_T(KC_J)

// ctl_t
#define KC_C_G LCTL_T(KC_G)
#define KC_C_H RCTL_T(KC_H)

// alt_t
#define KC_A_D ALT_T(KC_D)
#define KC_A_K ALT_T(KC_K)

// cmd+shift_t
#define KC_GS_S SCMD_T(KC_S)
#define KC_GS_L SCMD_T(KC_L)

//
#define KC_MISS C(KC_UP)

#define TAPPING_LAYER_TERM 230
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_GS_S:
      return TAPPING_LAYER_TERM;
    case KC_GS_L:
      return TAPPING_LAYER_TERM;
    case KC_A_D:
      return TAPPING_LAYER_TERM;
    case KC_A_K:
      return TAPPING_LAYER_TERM;
    default:
      return TAPPING_TERM;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // M_ = LCMD_T(
  // A_ = ALT_T(
  // C_ = LCTL_T(
  // MS_ = SMD_T(
  // R_ = LT(_RAISE
  // L_ = LT(_LOWER

  [_QWERTY] = LAYOUT_kc( \
  //,----+----+----+----+----+----.        ,----+----+----+----+----+----.
      ESC, Q  , W  , E  , R  , T  ,          Y  , U  , I  , O  , P  ,MINS,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
      TAB, A  ,GS_S,A_D ,G_F ,C_G , NO,  BSPC,  C_H ,G_J ,A_K ,GS_L,SCLN,ESC ,
  //|----+----+----+----+----+----+        |----+----+----+----+----+----|
     LEFT, Z  , X  , C  , V  , B  , NO,  DEL,   N  , M  ,COMM,DOT ,SLSH,RGHT,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                    EISU,S_EN,L_SPC,       R_ENT,S_JA,KANA2
  //          `----+----+----+----'        `----+----+----+----'
  ),

// 薙刀式
  [_NAGINATA] = LAYOUT(
    _______,NG_Q   ,NG_W   ,NG_E   ,NG_R   ,NG_T   ,                NG_Y   ,NG_U   ,NG_I   ,NG_O   ,NG_P   ,_______, \
    _______,NG_A   ,NG_S   ,NG_D   ,NG_F   ,NG_G   ,_______,_______,NG_H   ,NG_J   ,NG_K   ,NG_L   ,NG_SCLN,_______, \
    _______,NG_Z   ,NG_X   ,NG_C   ,NG_V   ,NG_B   ,_______,_______,NG_N   ,NG_M   ,NG_COMM,NG_DOT ,NG_SLSH,_______, \
                            _______,NG_SHFT,_______,                _______,NG_SHFT2,_______
                          //_______,_______,NG_SHFT,                NG_SHFT2,_______,_______
  ),
// 薙刀式

  //   \ ! & |      = + * %
  //   # ` " ' ~  ← ↓ ↑ → $
  //       { [ (  ) ] } @ ^

  [_RAISE] = LAYOUT_kc( \
  //,----+----+----+----+----+----.        ,----+----+----+----+----+----.
         ,BSLS,EXLM,AMPR,PIPE,XXXX,         XXXX,EQL ,PLUS,ASTR,PERC,MINS,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,HASH,GRV ,DQT ,QUOT,TILD,  ,   ,  LEFT,DOWN, UP ,RGHT,DLR ,    ,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,    ,    ,LCBR,LBRC,LPRN,  ,   ,  RPRN,RBRC,RCBR,AT  ,CIRC,    ,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                        ,    ,BSPC         ,    ,    ,RST
  //          `----+----+----+----'        `----+----+----+----'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,----+----+----+----+----+----.        ,----+----+----+----+----+----.
         ,    ,    ,    ,    ,XXXX,         XXXX,EQL ,PLUS,ASTR,PERC,MINS,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         , 1  , 2  , 3  , 4  , 5  ,  ,   ,   6  , 7  , 8  , 9  , 0  ,    ,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,  ,   ,      ,    ,COMM,DOT ,SLSH,    ,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                    RST ,    ,    ,        DEL ,    ,
  //          `----+----+----+----'        `----+----+----+----'
  ),

};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {        // left ↓
        if (clockwise) {
            tap_code(KC_WH_U);
        } else {
            tap_code(KC_WH_D);
        }
    } else if (index == 1) { // left ↑
        if (clockwise) {
            tap_code(KC__VOLDOWN);
        } else {
            tap_code(KC__VOLUP);
        }
    }
}

void matrix_init_user(void) {
  // 薙刀式
  uint16_t ngonkeys[] = {KC_H, KC_J};
  uint16_t ngoffkeys[] = {KC_F, KC_G};
  set_naginata(_NAGINATA, ngonkeys, ngoffkeys);
  // 薙刀式

  //oled_sleep_timer = timer_read32() + OLED_TIMEOUT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case EISU:
      if (record->event.pressed) {
        // 薙刀式
        naginata_off();
        // 薙刀式
      }
      return false;
      break;
    case KANA2:
      if (record->event.pressed) {
        // 薙刀式
        naginata_on();
        // 薙刀式
      }
      return false;
      break;
    case LCTOGL:
      if (record->event.pressed) {
        mac_live_conversion_toggle();
      }
      return false;
      break;
  }

  // 薙刀式
    if (!process_naginata(keycode, record))
      return false;
  // 薙刀式

  return true;
}
