// vim: ft=cpp
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-NumPad.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LED-AlphaSquare.h"
#include "Kaleidoscope-Colormap.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-USB-Quirks.h"

// Macros.
enum {
  MACRO_VERSION_INFO,
  MACRO_ANY,
};

// Layers.
enum {
  BASE,
  LEFT_FN,
  RIGHT_FN,
};

KEYMAPS(
  [BASE] = KEYMAP_STACKED(
    Key_Equals,      Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
    Key_Tab,         Key_Q, Key_W, Key_E, Key_R, Key_T, Key_LeftBracket,
    Key_LeftControl, Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_LeftShift,   Key_Z, Key_X, Key_C, Key_V, Key_B, Key_LeftAlt,

    Key_LeftGui, Key_Backspace, Key_Escape, Key_Backtick,
    ShiftToLayer(LEFT_FN),

    M(MACRO_ANY),     Key_6, Key_7, Key_8,     Key_9,       Key_0,         Key_Minus,
    Key_RightBracket, Key_Y, Key_U, Key_I,     Key_O,       Key_P,         Key_Backslash,
                      Key_H, Key_J, Key_K,     Key_L,       Key_Semicolon, Key_Quote,
    Key_RightAlt,     Key_N, Key_M, Key_Comma, Key_Period,  Key_Slash,     Key_RightShift,

    Key_Insert, Key_Enter, Key_Spacebar, Key_Equals,
    ShiftToLayer(RIGHT_FN)
  ),

  [LEFT_FN] = KEYMAP_STACKED(
    ___, ___, ___, ___, ___, ___, ___,
    ___, Consumer_ScanPreviousTrack, Consumer_PlaySlashPause, Consumer_ScanNextTrack, ___, ___, Key_PageUp,
    ___, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_Mute, ___, ___,
    ___, Key_F14, Key_F15, ___, ___, ___, Key_PageDown,

    ___, Key_Delete, ___, ___,
    ___,

    M(MACRO_VERSION_INFO),  Key_KeypadNumLock, Key_Keypad7, Key_KeypadDivide, Key_KeypadMultiply, Key_KeypadSubtract, ___,
    Key_Home,               Key_PrintScreen,   Key_Keypad7, Key_Keypad8, Key_Keypad9, Key_KeypadAdd, ___,
                            Key_ScrollLock,    Key_Keypad4, Key_Keypad5, Key_Keypad6, Key_KeypadAdd, ___,
    Key_End,                Key_Pause,         Key_Keypad1, Key_Keypad2, Key_Keypad3, Key_Equals,    ___,

    Key_Enter, Key_Enter, Key_Keypad0, Key_Keypad0,
    ___
  ),

  [RIGHT_FN] = KEYMAP_STACKED(
    ___,          Key_F1, Key_F2,           Key_F3,      Key_F4,           Key_F5,            XXX,
    Key_CapsLock, ___,    Key_mouseScrollL, Key_mouseUp, Key_mouseScrollR, Key_mouseScrollUp, ___,
    ___,          ___,    Key_mouseL,       Key_mouseDn, Key_mouseR,       Key_mouseScrollDn,
    ___,          ___,    ___,              ___,         ___,              ___,    ___,

    Key_mouseBtnM, Key_mouseBtnL, Key_mouseBtnR, ___,
    ___,

    ___, Key_F6,        Key_F7,               Key_F8,                Key_F9,          Key_F10,          Key_F11,
    ___, ___,           Key_LeftCurlyBracket, Key_RightCurlyBracket, Key_LeftBracket, Key_RightBracket, Key_F12,
         Key_LeftArrow, Key_DownArrow,        Key_UpArrow,           Key_RightArrow,  ___,              ___,
    ___, ___,           ___,                  ___,                   ___,             ___,              ___,

    ___, ___, Key_Enter, ___,
    ___
  ),
)

void
versionInfoMacro(uint8_t keyState)
{
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Property of @hkmix"));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

const
macro_t *macroAction(uint8_t macroIndex, uint8_t keyState)
{
  switch (macroIndex) {
  case MACRO_VERSION_INFO:
    versionInfoMacro(keyState);
    break;
  }

  return MACRO_NONE;
}

// These 'solid' color effect definitions define a rainbow of LED color modes
// calibrated to draw 500mA or less on the Keyboardio Model 01.
static kaleidoscope::plugin::LEDSolidColor solidWhite(120, 120, 120);
static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

// toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
// and turns them back on when it wakes up.
void
toggleLedsOnSuspendResume(
  kaleidoscope::plugin::HostPowerManagement::Event event)
{
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    LEDControl.paused = true;
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

// hostPowerManagementEventHandler dispatches power management events (suspend,
// resume, and sleep) to other functions that perform action based on these
// events.
void
hostPowerManagementEventHandler(
  kaleidoscope::plugin::HostPowerManagement::Event event)
{
  toggleLedsOnSuspendResume(event);
}

// This 'enum' is a list of all the magic combos used by the Model 01's
// firmware The names aren't particularly important. What is important is that
// each is unique.
//
// These are the names of your magic combos. They will be used by the
// `USE_MAGIC_COMBOS` call below.
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
};

// A tiny wrapper, to be used by MagicCombo.
// This simply toggles the keyboard protocol via USBQuirks, and wraps it within
// a function with an unused argument, to match what MagicCombo expects.
void
toggleKeyboardProtocol(uint8_t combo_index)
{
  USBQuirks.toggleKeyboardProtocol();
}

/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({
  .action = toggleKeyboardProtocol,
  // Left Fn + Esc + Shift
  .keys = {R3C6, R2C6, R3C7},
});

KALEIDOSCOPE_INIT_PLUGINS(
  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // The boot greeting effect pulses the LED button for 10 seconds after the
  // keyboard is first connected.
  BootGreetingEffect,

  // The hardware test mode, which can be invoked by tapping Prog, LED and
  // the left Fn button at the same time.
  TestMode,

  // LEDControl provides support for other LED modes.
  LEDControl,

  // Some LED effects.
  LEDOff,
  LEDRainbowEffect,
  LEDRainbowWaveEffect,
  solidWhite,

  // Misc LED effects.
  AlphaSquareEffect,
  StalkerEffect,

  // The macros plugin adds support for macros
  Macros,

  // The MouseKeys plugin lets you add keys to your keymap which move the
  // mouse.
  MouseKeys,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at
  // the same time.
  MagicCombo,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks
);

void
setup()
{
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // Mouse keys options.
  MouseKeys.speed = 4;
  MouseKeys.speedDelay = 4;
  MouseKeys.accelSpeed = 1;
  MouseKeys.accelDelay = 12;
  MouseKeys.setSpeedLimit(64);

  // We configure the AlphaSquare effect to use RED letters.
  AlphaSquare.color = CRGB(255, 0, 0);

  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255)
  // This draws more than 500mA, but looks much nicer than a dimmer effect.
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);

  // The LED Stalker mode has a few effects. The one we like is called
  // 'BlazingTrail'. For details on other options, see
  // https://github.com/keyboardio/Kaleidoscope/blob/master/doc/plugin/LED-Stalker.md
  StalkerEffect.variant = STALKER(BlazingTrail);

  // Start with LEDs off.
  LEDOff.activate();
}

void
loop()
{
  Kaleidoscope.loop();
}
