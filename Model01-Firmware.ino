// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

// The Kaleidoscope core.
#include "Kaleidoscope.h"

// Support for keys that move the mouse.
#include "Kaleidoscope-MouseKeys.h"

// Support for macros.
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs.
#include "Kaleidoscope-LEDControl.h"

// Support for an "LED off mode".
#include "LED-Off.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for
// 10s when the keyboard is connected to a computer (or that computer is
// powered on).
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for LED modes that set all LEDs to a single color.
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for an LED mode that makes all the LEDs 'breathe'.
#include "Kaleidoscope-LEDEffect-Breathe.h"

// Support for LED modes that pulse the keyboard's LED in a rainbow pattern.
#include "Kaleidoscope-LEDEffect-Rainbow.h"

// Support for an LED mode that lights up the keys as you press them.
#include "Kaleidoscope-LED-Stalker.h"

// Support for Keyboardio's internal keyboard testing mode.
#include "Kaleidoscope-Model01-TestMode.h"

// Support for host power management (suspend & wakeup).
#include "Kaleidoscope-HostPowerManagement.h"

// This 'enum' is a list of all the macros used by the Model 01's firmware The
// names aren't particularly important. What is important is that each is
// unique.
//
// These are the names of your macros. They'll be used in two places. The first
// is in your keymap definitions. There, you'll use the syntax `M(MACRO_NAME)`
// to mark a specific keymap position as triggering `MACRO_NAME`
//
// The second usage is in the 'switch' statement in the `macroAction` function.
// That switch statement actually runs the code associated with a macro when a
// macro key is pressed.

enum {
  MACRO_VERSION_INFO,
  MACRO_ANY
};

// The Model 01's key layouts are defined as 'keymaps'. By default, there are
// three keymaps: The standard QWERTY keymap, the "Function layer" keymap and
// the "Numpad" keymap.
//
// Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built of
// first the left hand's layout, followed by the right hand's layout.
//
// Keymaps typically consist mostly of `Key_` definitions. There are many, many
// keys defined as part of the USB HID Keyboard specification. You can find the
// names (if not yet the explanations) for all the standard `Key_` defintions
// offered by Kaleidoscope in these files:
//    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keyboard.h
//    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_consumerctl.h
//    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_sysctl.h
//    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keymaps.h
//
// Additional things that should be documented here include
//   using ___ to let keypresses fall through to the previously active layer
//   using XXX to mark a keyswitch as 'blocked' on this layer
//   using ShiftToLayer() and LockLayer() keys to change the active keymap.
//   the special nature of the PROG key
//   keeping NUM and FN consistent and accessible on all layers
//
// The "keymaps" data structure is a list of the keymaps compiled into the
// firmware. The order of keymaps in the list is important, as the
// ShiftToLayer(#) and LockLayer(#) macros switch to key layers based on this
// list.
//
// A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while
// held. Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD
// when tapped.

// Layers.
enum {
  BASE,
  LEFT_FN,
  RIGHT_FN
};

const Key keymaps[][ROWS][COLS] PROGMEM = {
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
};

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

  case MACRO_VERSION_INFO:
    versionInfoMacro(keyState);
    break;
  }

  return MACRO_NONE;
}


// These 'solid' color effect definitions define a rainbow of LED color modes
// calibrated to draw 500mA or less on the Keyboardio Model 01.
static kaleidoscope::LEDSolidColor solidWhite(120, 120, 120);
static kaleidoscope::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::LEDSolidColor solidViolet(130, 0, 120);

// toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
// and turns them back on when it wakes up.
void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event)
{
  switch (event) {
  case kaleidoscope::HostPowerManagement::Suspend:
    LEDControl.paused = true;
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    break;

  case kaleidoscope::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;

  case kaleidoscope::HostPowerManagement::Sleep:
    break;
  }
}

// hostPowerManagementEventHandler dispatches power management events (suspend,
// resume, and sleep) to other functions that perform action based on these
// events.
void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

KALEIDOSCOPE_INIT_PLUGINS(
  // The hardware test mode, which can be invoked by tapping Prog, LED and
  // the left Fn button at the same time.
  TestMode,

  // LEDControl provides support for other LED modes.
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // Some LED effects.
  LEDRainbowEffect,
  LEDRainbowWaveEffect,
  StalkerEffect,

  // These static effects turn your keyboard's LEDs a variety of colors.
  solidWhite, // &solidRed, &solidOrange, &solidYellow, &solidGreen,
  // &solidBlue, &solidIndigo, &solidViolet,

  // The breathe effect slowly pulses all of the LEDs on your keyboard.
  LEDBreatheEffect,

  // The macros plugin adds support for macros.
  Macros,

  // The MouseKeys plugin lets you add keys to your keymap which move the
  // mouse.
  MouseKeys,

  // The HostPowerManagement plugin enables waking up the host from suspend,
  // and allows us to turn LEDs off when it goes to sleep.
  HostPowerManagement
)

void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // Mouse keys options.
  MouseKeys.speed = 4;
  MouseKeys.speedDelay = 4;
  MouseKeys.accelSpeed = 1;
  MouseKeys.accelDelay = 12;
  MouseKeys.speedLimit = 64;

  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255).
  // This draws more than 500mA, but looks much nicer than a dimmer effect.
  LEDRainbowEffect.brightness(150);
  LEDRainbowEffect.update_delay(100);
  LEDRainbowWaveEffect.brightness(150);

  // The LED Stalker mode has a few effects. The one we like is called
  // 'BlazingTrail'. For details on other options, see
  // https://github.com/keyboardio/Kaleidoscope-LED-Stalker
  StalkerEffect.variant = STALKER(Haunt, (CRGB(160, 160, 160)));

  // We want the keyboard to be able to wake the host up from suspend.
  HostPowerManagement.enableWakeup();

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();
}

// loop is the second of the standard Arduino sketch functions. As you might
// expect, it runs in a loop, never exiting.
//
// For Kaleidoscope-based keyboard firmware, you usually just want to call
// Kaleidoscope.loop(); and not do anything custom here.
void loop() {
  Kaleidoscope.loop();
}
