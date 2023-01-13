/* Copyright 2022 Tuomas Mäkinen <tuomasvmakinen@outlook.com>
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
#include "keymap_finnish.h"

enum layers {
    //_TYOMIES = 0,
    _QWERTY = 0,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
};

// Aliases for readability
//#define TYOMIES  DF(_TYOMIES)
#define QWERTY   DF(_QWERTY)

#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)

#define LCTL_A   LCTL_T(KC_A)
#define LALT_S   LALT_T(KC_S)
#define LGUI_D   LGUI_T(KC_D)
#define LSFT_F   LSFT_T(KC_F)
#define RSFT_J   RSFT_T(KC_J)
#define RGUI_K   RGUI_T(KC_K)
#define RALT_L   RALT_T(KC_L)
#define RCTL_CLN RCTL_T(KC_SCLN)

enum td_keycodes {
    ADIA_QUOT_DQUO,
    COMM_LABK,
    DOT_RABK,
    SLSH_QUES
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_TRIPLE_TAP,
} td_state_t;

static td_state_t td_state;

td_state_t cur_dance(qk_tap_dance_state_t *state);

void adiaquotdquo_finished(qk_tap_dance_state_t *state, void *user_data);
void adiaquotdquo_reset(qk_tap_dance_state_t *state, void *user_data);

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Esc   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  Tab   |Ctrl/A|Alt/S |GUI/D |Shft/F|   G  |                              |   H  |Shft/J|GUI/K |Alt/L |Ctrl/Ö|    Ä   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |      |  |      |  ] } |   N  |   M  | ,  < | .  > | -  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
        KC_ESC,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                                                       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_BSPC,
        KC_TAB,     LCTL_A,     LALT_S,     LGUI_D,     LSFT_F,     KC_G,                                                       KC_H,       RSFT_J,     RGUI_K,     RALT_L,     RCTL_CLN,   FI_ADIA,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       A(KC_8),    _______,    _______,    A(KC_9),    KC_N,       KC_M,       TD(COMM_LABK),TD(DOT_RABK),TD(SLSH_QUES),KC_RSFT,
                                            ADJUST,     KC_LGUI,    ALT_ENT,    KC_SPC,     NAV,        SYM,        KC_SPC,     KC_RALT,    KC_RGUI,    KC_APP
    ),

/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | Ctrl |  Alt |  GUI | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
        _______,    _______,    _______,    _______,    _______,    _______,                                                    KC_PGUP,    KC_HOME,    KC_UP,      KC_END,     KC_VOLU,    KC_DEL,
        _______,    KC_LCTL,    KC_LALT,    KC_LGUI,    KC_LSFT,    _______,                                                    KC_PGDN,    KC_LEFT,    KC_DOWN,    KC_RGHT,    KC_VOLD,    KC_INS,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_SLCK,    _______,    _______,    KC_PAUSE,   KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_PSCR,
                                            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    §   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   ~    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    °   |  !   |  @   |  #   |  $   |  %   |                              |   &  |  /   |  (   |  )   |  =   |   ^    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    *   |  '   |  "   |  £   |  €   |  [   |  {   |      |  |      |   }  |   ]  |  |   |  \   |  .   |  -   |   _    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                                                       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       A(KC_RBRC),
        KC_TILD,    KC_EXLM,    A(KC_2),    KC_HASH,    A(KC_4),    KC_PERC,                                                    KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    A(KC_RCBR),
        KC_PIPE,    KC_BSLS,    KC_AT,      A(KC_3),    KC_DLR,     A(KC_8),    S(A(KC_8)), _______,    _______,    S(A(KC_9)), A(KC_9),    A(KC_7),    S(A(KC_7)), KC_MINS,    KC_SLSH,    KC_QUES,
                                            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              |      | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTION] = LAYOUT(
        _______,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,                                                    _______,    _______,    _______,    _______,    _______,    _______,
        _______,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      _______,                                                    _______,    KC_RSFT,    KC_RCTL,    KC_LALT,    KC_RGUI,    _______,
        _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
                                            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |QWERTY|      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
        _______,    _______,    _______,    QWERTY,     _______,    _______,                                                    _______,    _______,    _______,    _______,    _______,    _______,
        _______,    _______,    _______,    QWERTY,     _______,    _______,                                                    RGB_TOG,    RGB_SAI,    RGB_HUI,    RGB_VAI,    RGB_MOD,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_SAD,    RGB_HUD,    RGB_VAD,    RGB_RMOD,   _______,
                                            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_TAP;
    if (state->count == 3) return TD_TRIPLE_TAP;
    else return TD_UNKNOWN;
}

void adiaquotdquo_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(FI_ADIA);
            break;
        case TD_DOUBLE_TAP:
            register_code16(FI_QUOT);
            break;
        case TD_TRIPLE_TAP:
            register_code16(FI_DQUO);
        default:
            break;
    }
}

void adiaquotdquo_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(FI_ADIA);
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(FI_QUOT);
            break;
        case TD_TRIPLE_TAP:
            unregister_code16(FI_DQUO);
        default:
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [ADIA_QUOT_DQUO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, adiaquotdquo_finished, adiaquotdquo_reset),
    [COMM_LABK] = ACTION_TAP_DANCE_DOUBLE(FI_COMM, FI_LABK),
    [DOT_RABK] = ACTION_TAP_DANCE_DOUBLE(FI_DOT, FI_RABK),
    [SLSH_QUES] = ACTION_TAP_DANCE_DOUBLE(FI_MINS, FI_QUES)
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev2.0\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            // case _TYOMIES:
            //     oled_write_P(PSTR("TYÖMIES\n"), false);
            //     break;
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            case _FUNCTION:
                oled_write_P(PSTR("Function\n"), false);
                break;
            case _ADJUST:
                oled_write_P(PSTR("Adjust\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif

/* TODO
 * Base layer: TYOMIES
 *
 *      ,---------------------------------------.                          ,-------------------------------------.
 *      |  TAB   |   Q  |  M  |  K  |  R  |  G  |                          |  Ö  |  H  |  U  |  Ä  |  W  |  BSP  |
 *      |--------+------+-----+-----+-----+-----|                          |-----+-----+-----+-----+-----+-------|
 *      |Ctrl/Esc|Ctrl/L|ALT/S|GUI/N|SFT/I|  Y  |                          |  P  |SFT/A|GUI/T|ALT/E|CTL/O|CTL/'/"|
 *      |--------+------+-----+-----+-----+-----+-----------.  ,-----------+-----+-----+-----+-----+-----+-------|
 *      | LShift |   Z  |  X  |  C  |  V  |  F  |     |     |  |     |     |  B  |  J  |  D  | ./, | -/? | SHIFT |
 *      `---------------------+-----+-----+-----| SPC | NAV |  | SYM | SPC |-----+-----+-----+-------------------'
 *                     | ADJ | GUI | ALT |     |     |  |     |     |     |     |     |
 *                     |     |     | ENT |     |     |  |     |     |     |     |     |
 *                     `-----------------------------'  `-----------------------------'
 */
    // [_TYOMIES] = LAYOUT(
    //     KC_TAB,  KC_Q, KC_M, KC_K, KC_R, KC_G, FI_ODIA, KC_H, KC_U, FI_ADIA, KC_W, KC_BSPC,
    //     CTL_ESC, LCTL_T(KC_L), LALT_T(KC_S), LGUI_T(KC_N), LSFT_T(KC_I), KC_Y, KC_P, LSFT_T(KC_A), LGUI_T(KC_T), LALT_T(KC_E), LCTL_T(KC_O), TD(CTL_QUOT_DQUO),
    //     KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_F, _______, _______, _______, _______, KC_B, KC_J, KC_D, TD(DOT_COMM), TD(MINS_QUES), KC_RSFT,
    //     ADJUST,  KC_LGUI, ALT_ENT, KC_SPC, NAV, SYM, KC_SPC, KC_RALT, KC_RGUI, KC_APP
    // ),

