// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "virtser.h"   // for virtser_send_string
#include <stdio.h>     // for snprintf
#include "analog.h"

#ifdef RGB_MATRIX_ENABLE

// Define the LED layout for your 6x12 split keyboard with LEDs only on first 4 rows (48 LEDs total)
led_config_t g_led_config = { {
    // Key Matrix to LED Index mapping (12 cols × 5 rows = 60 keys/LEDs)
    {  0,  1,  2,  3,  4,  5 },      // Row 0 left
    {  6,  7,  8,  9, 10, 11 },      // Row 1 left
    { 12, 13, 14, 15, 16, 17 },      // Row 2 left
    { 18, 19, 20, 21, 22, 23 },      // Row 3 left
    { 24, 25, 26, 27, 28, 29 },
    { 30, 31, 32, 33, 34, 35 },      // Row 5 left (encoders row – no LEDs, skip in positions)
    { 36, 37, 38, 39, 40, 41 },      // Row 0 right
    { 42, 43, 44, 45, 46, 47 },      // Row 1 right
    { 48, 49, 50, 51, 52, 53 },      // Row 2 right
    { 54, 55, 56, 57, 58, 59 },      // Row 3 right
    { NO_LED, NO_LED, NO_LED, NO_LED }  // Row 4 right (encoder buttons, no LEDs)
}, {
    // Physical LED positions (x, y) for 60 LEDs
    // Left half (0–29) - controlled by left microcontroller
    {   0,  10 }, {  16,  10 }, {  32,  4 }, {  48,   0 }, {  64,  4 }, { 80,   8 }, 
    {   0,  20 }, {  16,  20 }, {  32, 14 }, {  48,  15 }, {  64, 14 }, { 80,  18 }, 
    {   0,  30 }, {  16,  30 }, {  32, 24 }, {  48,  25 }, {  64, 24 }, { 80,  28 }, 
    {   0,  40 }, {  16,  40 }, {  32, 34 }, {  48,  35 }, {  64, 34 }, { 80,  38 }, 
    {   0,  50 }, {  56,  45 }, {  72, 50 }, {  96, 64 },  {  96, 33 }, { 96, 23 }, 
 
    // Right half (30–59) - reversed order
    { 224,  8 }, { 208,  4 }, { 192,  0 }, { 176,  4 }, { 160, 10 }, { 144, 10 }, 
    { 224, 18 }, { 208, 14 }, { 192, 15 }, { 176, 14 }, { 160, 20 }, { 144, 20 }, 
    { 224, 28 }, { 208, 24 }, { 192, 25 }, { 176, 24 }, { 160, 30 }, { 144, 30 }, 
    { 224, 38 }, { 208, 34 }, { 192, 35 }, { 176, 34 }, { 160, 40 }, { 144, 40 }, 
    { 224, 23 }, { 168, 33 }, { 152, 64 }, { 128, 50 }, { 128, 45 }, { 128, 50 } 

},
 {
    
    // Left half flags (LEDs 0-17)
    4, 4, 4, 4, 4, 4,  
    4, 4, 4, 4, 4, 4,  
    4, 4, 4, 4, 4, 4,  
    4, 4, 4, 4, 4, 4,  
    4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4,  
    4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4,   
    4, 4, 4, 4, 4, 4  
} };

#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* LAYER 0: BASE
    * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    * │Esc  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │ Bksp│
    * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    * │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │ \   │
    * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    * │Caps │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │
    * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    * │LShft│  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │RShft│
    * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    * │LCtrl│ LAlt│MO(1)│Space│  [  │  -  │  =  │  ]  │Enter│MO(2)│ RGui│RCtrl│
    * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    * │ XXX │ XXX │ XXX │ XXX │Mute │Sleep│Play │Calc │ XXX │ XXX │ XXX │ XXX │
    * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
    */
    [0] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,     KC_0,     KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,     KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,  KC_RSFT,
        KC_LCTL, KC_LALT, MO(1),   KC_SPC,  KC_LBRC, KC_MINS,      KC_EQL,  KC_RBRC, KC_ENT,  MO(1),    KC_RGUI,  KC_RCTL,
        RM_TOGG, KC_MPLY, KC_MUTE, KC_MPLY
    ),

    [1] = LAYOUT(
        KC_GRV , _______, _______,  _______, _______, _______,      _______, _______, _______, _______,  _______,  KC_DEL,
        KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,
        KC_INS , KC_HOME, KC_END ,  KC_PGUP, KC_PGDN, KC_PSCR,      KC_SCRL, KC_PAUSE, _______, KC_UP,   _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, KC_LEFT, KC_DOWN,  KC_RGHT,  _______,
        _______, _______, _______,  _______, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, _______, _______,  _______,  _______,
        _______, _______, _______,  _______
    ),

    [2] = LAYOUT(
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______
    ),

    [3] = LAYOUT(
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______, _______,  _______,
        _______, _______, _______,  _______
    ),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_LEFT,  KC_RGHT),  ENCODER_CCW_CW(KC_VOLU, KC_VOLD),  ENCODER_CCW_CW(KC_MNXT, KC_MPRV),  ENCODER_CCW_CW(RM_VALU, RM_VALD)},
    [1] = { ENCODER_CCW_CW(RM_PREV,  RM_NEXT),  ENCODER_CCW_CW(RM_HUEU, RM_HUED),  ENCODER_CCW_CW(RM_SATU, RM_SATD),  ENCODER_CCW_CW(RM_SPDU, RM_SPDD)},
    [2] = { ENCODER_CCW_CW(_______, _______),   ENCODER_CCW_CW(_______, _______),  ENCODER_CCW_CW(_______, _______),  ENCODER_CCW_CW(_______, _______)},
    [3] = { ENCODER_CCW_CW(_______, _______),   ENCODER_CCW_CW(_______, _______),  ENCODER_CCW_CW(_______, _______),  ENCODER_CCW_CW(_______, _______)}
};
#endif

void matrix_scan_user(void) {
    
    if (is_keyboard_master()) {
        static uint16_t last_timer = 0;
        static char buffer[64];
        static uint16_t f1 = 0, f2 = 0, f3 = 0, f4 = 0;

        const float alpha = 0.2f;

        if (timer_elapsed(last_timer) > 100) {
            last_timer = timer_read();

            uint16_t raw1 = analogReadPin(GP29) * 1023 / 4095;
            uint16_t raw2 = analogReadPin(GP28) * 1023 / 4095;
            uint16_t raw3 = analogReadPin(GP27) * 1023 / 4095;
            uint16_t raw4 = analogReadPin(GP26) * 1023 / 4095;

            if (raw1 < 10) {
                f1 = 0;
            } else if (raw1 > 1005) {
                f1 = 1023;
            } else {
                f1 = (uint16_t)(alpha * raw1 + (1.0f - alpha) * f1);
            }

            if (raw2 < 10) {
                f2 = 0;
            } else if (raw2 > 1005) {
                f2 = 1023;
            } else {
                f2 = (uint16_t)(alpha * raw2 + (1.0f - alpha) * f2);
            }

            if (raw3 < 10) {
                f3 = 0;
            } else if (raw3 > 1005) {
                f3 = 1023;
            } else {
                f3 = (uint16_t)(alpha * raw3 + (1.0f - alpha) * f3);
            }

            if (raw4 < 10) {
                f4 = 0;
            } else if (raw4 > 1005) {
                f4 = 1023;
            } else {
                f4 = (uint16_t)(alpha * raw4 + (1.0f - alpha) * f4);
            }

            snprintf(buffer, sizeof(buffer), "%u|%u|%u|%u\r\n", f1, f2, f3, f4);
            virtser_send_string(buffer);
        }
    }
}


