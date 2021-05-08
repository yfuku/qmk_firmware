#include QMK_KEYBOARD_H


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE
};

enum layer_number {
    _QWERTY = 0,
    _RAISE,
    _LOWER,
};

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
      TAB, A  ,GS_S,A_D ,G_F ,C_G ,         C_H ,G_J ,A_K ,GS_L,SCLN,ESC ,
  //|----+----+----+----+----+----+        |----+----+----+----+----+----|
     LEFT, Z  , X  , C  , V  , B  ,          N  , M  ,COMM,DOT ,SLSH,RGHT,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                    DOWN,S_EN,L_SPC,       R_ENT,S_JA,UP
  //          `----+----+----+----'        `----+----+----+----'
  ),

  //   \ ! & |      = + * %
  //   # ` " ' ~  ← ↓ ↑ → $
  //       { [ (  ) ] } @ ^

  [_RAISE] = LAYOUT_kc( \
  //,----+----+----+----+----+----.        ,----+----+----+----+----+----.
         ,BSLS,EXLM,AMPR,PIPE,XXXX,         XXXX,EQL ,PLUS,ASTR,PERC,MINS,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,HASH,GRV ,DQT ,QUOT,TILD,         LEFT,DOWN, UP ,RGHT,DLR ,    ,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,    ,    ,LCBR,LBRC,LPRN,         RPRN,RBRC,RCBR,AT  ,CIRC,    ,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                        ,    ,BSPC         ,    ,    ,RST
  //          `----+----+----+----'        `----+----+----+----'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,----+----+----+----+----+----.        ,----+----+----+----+----+----.
         ,    ,    ,    ,    ,XXXX,         XXXX,EQL ,PLUS,ASTR,PERC,MINS,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         , 1  , 2  , 3  , 4  , 5  ,          6  , 7  , 8  , 9  , 0  ,    ,
  //|----+----+----+----+----+----|        |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,             ,    ,COMM,DOT ,SLSH,    ,
  //`----+----+----+----+----+----/        \----+----+----+----+----+----'
                    RST ,    ,    ,        DEL ,    ,
  //          `----+----+----+----'        `----+----+----+----'
  ),

};

const int SENSOR_OUT = B6;
const int SENSOR_IN = B2;
const int threshold = 5;

void matrix_init_user(void) {
    setPinInput(SENSOR_IN);
    setPinOutput(SENSOR_OUT);
}

void keyboard_post_init_user() {
    debug_enable = true;
}

void matrix_scan_user(void) {
    //dprint("START\n");
    int counter = 0;
    writePinHigh(SENSOR_OUT);
    //dprint("1\n");

    //dprintf("%d\n", readPin(SENSOR_IN));
    while (readPin(SENSOR_IN)!= 1) {
        //dprintf("2::%d\n", readPin(SENSOR_IN));
        counter++;
    }
    dprintf("%d\n", counter);

    writePinLow(SENSOR_OUT);
    //dprint("4\n");
    _delay_ms(1);
    //dprint("5\n");

    if (counter > threshold) {
        //dprint("on\n");
    } else {
        //dprint("off\n");
    }
    //dprint("END\n");
}

