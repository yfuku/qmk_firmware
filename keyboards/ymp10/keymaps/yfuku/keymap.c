/* Copyright 2020 yfuku
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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                    KC_ESC, KC_ENT,
      LSA(KC_F1), LSA(KC_F2),LSA(KC_F11), LSA(KC_F12),
      KC_LEFT,  KC_DOWN,  KC_UP, KC_RIGHT
    ),
    [_LOWER] = LAYOUT(
                    KC_I, KC_J,
      KC_A,  KC_B,  KC_C, KC_D,
      KC_E,  KC_F,  KC_G, KC_H
    ),
    [_RAISE] = LAYOUT(
                    KC_I, KC_J,
      KC_A,  KC_B,  KC_C, KC_D,
      KC_E,  KC_F,  KC_G, KC_H
   ),
};

