/* Copyright 2018-2019 eswai <@eswai>
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

#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif

#include "keymap_jp.h"

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
// 薙刀式

enum keymap_layers {
  _QWERTY,
// 薙刀式
  _NAGINATA, // 薙刀式入力レイヤー
// 薙刀式
  _LOWER,
  _RAISE,
};

enum custom_keycodes {
  EISU = NG_SAFE_RANGE,
  KANA2,
};

uint32_t oled_sleep_timer;

#define KC_L_SPC LT(_LOWER, KC_SPC)  // lower
#define KC_R_ENT LT(_RAISE, KC_ENT)  // raise
#define KC_G_JA LGUI_T(KC_LANG1)     // cmd or win
#define KC_G_EN LGUI_T(KC_LANG2)     // cmd or win
#define KC_C_BS LCTL_T(KC_BSPC)      // ctrl
#define KC_A_DEL ALT_T(KC_DEL)       // alt

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT( \
    //,--------+--------+---------+--------+---------+--------.   ,--------+---------+--------+---------+--------+--------.
       KC_ESC , KC_Q   , KC_W    , KC_E   , KC_R    , KC_T   ,     KC_Y   , KC_U    , KC_I   , KC_O    , KC_P   , KC_MINS,
    //|--------+--------+---------+--------+---------+--------|   |--------+---------+--------+---------+--------+--------|
       KC_TAB , KC_A   , KC_S    , KC_D   , KC_F    , KC_G   ,     KC_H   , KC_J    , KC_K   , KC_L    , KC_SCLN, KC_QUOT,
    //|--------+--------+---------+--------+---------+--------|   |--------+---------+--------+---------+--------+--------|
       KC_LSFT, KC_Z   , KC_X    , KC_C   , KC_V    , KC_B   ,     KC_N   , KC_M    , KC_COMM, KC_DOT  , KC_SLSH, KC_RSFT,
    //`--------+--------+---------+--------+---------+--------/   \--------+---------+--------+---------+--------+--------'
                                   KC_G_EN, KC_L_SPC, KC_C_BS,     KC_C_BS, KC_R_ENT, KC_G_JA
    //                 `----------+--------+---------+--------'   `--------+---------+--------+---------'
    ),

// 薙刀式
  [_NAGINATA] = LAYOUT(
    _______,NG_Q   ,NG_W   ,NG_E   ,NG_R   ,NG_T   ,                NG_Y   ,NG_U   ,NG_I   ,NG_O   ,NG_P   ,_______, \
    _______,NG_A   ,NG_S   ,NG_D   ,NG_F   ,NG_G   ,                NG_H   ,NG_J   ,NG_K   ,NG_L   ,NG_SCLN,_______, \
    _______,NG_Z   ,NG_X   ,NG_C   ,NG_V   ,NG_B   ,                NG_N   ,NG_M   ,NG_COMM,NG_DOT ,NG_SLSH,_______, \
                            _______,NG_SHFT,_______,                _______,NG_SHFT,_______
  ),
// 薙刀式

    [_RAISE] = LAYOUT( \
    //,--------+--------+--------+--------+--------+--------.   ,--------+--------+--------+--------+--------+--------.
       _______, KC_BSLS, KC_CIRC, KC_EXLM, KC_AMPR, KC_PIPE,     KC_AT  , KC_EQL , KC_PLUS, KC_ASTR, KC_PERC, KC_MINS,
    //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
       KC_LPRN, KC_HASH, KC_DLR , KC_DQT , KC_QUOT, KC_TILD,     KC_LEFT, KC_DOWN,  KC_UP , KC_RGHT, KC_GRV , KC_RPRN,
    //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
       _______, _______, _______, _______, KC_LCBR, KC_LBRC,     KC_RBRC, KC_RCBR, _______, _______, _______, _______,
    //`--------+--------+--------+--------+--------+--------/   \--------+--------+--------+--------+--------+--------'
                                  _______, _______, _______,     _______, _______, RESET
    //                  `--------+--------+--------+--------'   `--------+--------+--------+--------'
    ),

    [_LOWER] = LAYOUT( \
    //,--------+--------+--------+--------+--------+--------.   ,--------+--------+--------+--------+--------+--------.
       KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,     _______, KC_EQL , KC_PLUS, KC_ASTR, KC_PERC, KC_MINS,
    //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
       _______, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,     KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
    //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
       KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,     _______, _______, KC_COMM, KC_DOT , KC_SLSH, _______,
    //`--------+--------+--------+--------+--------+--------/   \--------+--------+--------+--------+--------+--------'
                                  RESET  , _______, _______,     _______, _______, _______
    //                  `--------+--------+--------+--------'   `--------+--------+--------+--------'
    ),
};

void matrix_init_user(void) {
  // 薙刀式
  uint16_t ngonkeys[] = {KC_H, KC_J};
  uint16_t ngoffkeys[] = {KC_F, KC_G};
  set_naginata(_NAGINATA, ngonkeys, ngoffkeys);
  // 薙刀式

  oled_sleep_timer = timer_read32() + OLED_TIMEOUT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  oled_sleep_timer = timer_read32() + OLED_TIMEOUT;

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
  }

  // 薙刀式
    if (!process_naginata(keycode, record))
      return false;
  // 薙刀式

  return true;
}

// 薙刀式 OLED表示
#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }
    return rotation;
}

/*
    薙刀式ロゴの著作権は大岡俊彦氏に帰属します。
    http://oookaworks.seesaa.net
*/
static void naginata_logo(void) {
    static const char PROGMEM ng_logo[] = {
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x40, 0x30, 0xf0, 0xf0, 0xe8, 0xe8, 0xc, 0xc, 0x4, 0x4, 0x4, 0xc, 0xc, 0xc8, 0xe8, 0xe8, 0xf0, 0x30, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xe0, 0xe0, 0xa0, 0x80, 0x80, 0x80, 0xe0, 0xe0, 0xa0, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0x80, 0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0xe0, 0x20, 0x40, 0xc0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x79, 0x9b, 0xe3, 0xf3, 0xf3, 0xf3, 0xf7, 0x77, 0x8f, 0xcf, 0x3, 0x43, 0x43, 0x3, 0x83, 0xef, 0xff, 0xf7, 0xf7, 0xb3, 0xd3, 0xe3, 0xb3, 0x9b, 0xa9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x60, 0x38, 0x1e, 0xf6, 0xf0, 0x13, 0x1b, 0x50, 0x60, 0xf8, 0xfe, 0x97, 0x91, 0xf8, 0xfe, 0x94, 0x90, 0xd8, 0x90, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0xfe, 0x7f, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x1, 0x0, 0x0, 0x0, 0x0, 0x2, 0x42, 0x42, 0x42, 0x42, 0xc2, 0xc2, 0x42, 0x62, 0x72, 0x62, 0xf, 0xff, 0xc2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x87, 0xd9, 0x35, 0x5b, 0xcc, 0x86, 0x3, 0x37, 0x76, 0x76, 0x62, 0x0, 0x3, 0x7f, 0x3f, 0x1f, 0x60, 0x6a, 0xff, 0x6f, 0x6a, 0x60, 0x6e, 0x60, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x82, 0x62, 0x32, 0x1f, 0xf, 0x12, 0x73, 0x63, 0x2, 0xff, 0xff, 0x88, 0x88, 0xff, 0xff, 0x88, 0x88, 0xcc, 0xc8, 0x80, 0x0, 0x0, 0x80, 0xc0, 0x40, 0x20, 0x38, 0xe, 0x7, 0x1, 0x0, 0x0, 0x40, 0x40, 0xc0, 0xc0, 0xf0, 0x7f, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x60, 0xe0, 0x60, 0x60, 0x20, 0x3f, 0x3f, 0x10, 0x10, 0x8, 0x8, 0x0, 0x1, 0xf, 0x1e, 0x78, 0xe0, 0xc0, 0xf0, 0x9c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x22, 0x37, 0x32, 0x18, 0xc, 0x6, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };
    oled_write_raw_P(ng_logo, sizeof(ng_logo));
}

static void delete_char(uint16_t x, uint16_t w) {
  for (uint16_t i = x; i < x + w; i++) {
    oled_write_raw_byte(0x00, i);
  }
}

static void render_mode(void) {
    switch (naginata_config.os) {
      case NG_WIN:
        delete_char(12, 14);
        break;
      case NG_MAC:
        delete_char(5, 7);
        delete_char(19, 7);
        break;
      case NG_LINUX:
        delete_char(5, 14);
        break;
    }
    if (naginata_config.tategaki) {
      delete_char(128 + 12, 7);
    } else {
      delete_char(128 + 5, 7);
    }
    if (!naginata_config.kouchi_shift) {
      delete_char(2 * 128 + 5, 7);
    }
    if (!naginata_config.live_conv) {
      delete_char(3 * 128 + 5, 7);
    }
}

static void render_kana(void) {
    static const char PROGMEM kana_pic[] = {
      0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc1, 0xbf, 0xc7, 0xbf, 0xc1, 0xff, 0xff, 0x81, 0xfb, 0xe7, 0xfb, 0x81, 0xff, 0xff, 0x81, 0xbf, 0xbf, 0xbf, 0xbf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xf9, 0xfd, 0x81, 0xfd, 0xf9, 0xff, 0xff, 0xf9, 0xf7, 0x8f, 0xf7, 0xf9, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x60, 0x60, 0x60, 0xe0, 0xf0, 0x3f, 0x1f, 0x19, 0x38, 0xf0, 0xc0, 0x6, 0xc, 0x18, 0x70, 0xe0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x18, 0x18, 0x98, 0xf8, 0x3d, 0x1f, 0xb, 0xc, 0xe, 0x6, 0x4, 0xc0, 0x0, 0x6, 0x2c, 0x38, 0x70, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x81, 0xef, 0xd7, 0xbb, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0xf8, 0x3e, 0x7, 0x21, 0x40, 0xc0, 0xe0, 0xfe, 0x3f, 0x1, 0x0, 0x4, 0x4, 0x6, 0xf, 0xf, 0x7, 0x0, 0x0, 0x0, 0x0, 0xc, 0x1f, 0x7, 0x1, 0xe0, 0xf0, 0x90, 0x98, 0x98, 0x98, 0xff, 0xff, 0x30, 0x60, 0xe0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x81, 0xbf, 0xbf, 0xbf, 0xbf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };
    oled_write_raw_P(kana_pic, sizeof(kana_pic));
}

static void render_eisu(void) {
    static const char PROGMEM eisu_pic[] = {
      0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc1, 0xbf, 0xc7, 0xbf, 0xc1, 0xff, 0xff, 0x81, 0xfb, 0xe7, 0xfb, 0x81, 0xff, 0xff, 0x81, 0xbf, 0xbf, 0xbf, 0xbf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xf9, 0xfd, 0x81, 0xfd, 0xf9, 0xff, 0xff, 0xf9, 0xf7, 0x8f, 0xf7, 0xf9, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xf0, 0x3c, 0xf, 0x3, 0x7, 0x1c, 0x70, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x81, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x81, 0x83, 0xc3, 0x7e, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x78, 0xc, 0x6, 0x2, 0x3, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x81, 0xef, 0xd7, 0xbb, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xf0, 0x3c, 0xf, 0xb, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x9, 0xf, 0x3c, 0xf0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x81, 0x83, 0xc6, 0x7e, 0x18, 0x0, 0x0, 0x0, 0x0, 0xf, 0x3c, 0x60, 0xc0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x81, 0xbf, 0xbf, 0xbf, 0xbf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };
    oled_write_raw_P(eisu_pic, sizeof(eisu_pic));
}

void oled_task_user(void) {
    // なぜか明示的にOLEDのスリープ処理が必要
    if (timer_expired32(timer_read32(), oled_sleep_timer)) {
      oled_off();
      return;
    } else {
      oled_on();
    }

    if (is_keyboard_master()) {
      if (naginata_state()) {
        render_kana();
      } else {
        render_eisu();
      }
      render_mode();
    } else {
      naginata_logo();
    }
}
#endif
