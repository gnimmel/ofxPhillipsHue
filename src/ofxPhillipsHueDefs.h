#pragma once

#define MAX_HUE 65535

#define ATTRIBUTE_NAME "name"
#define ATTRIBUTE_TYPE "type"
#define ATTRIBUTE_MODEL "modelid"
#define ATTRIBUTE_SOFTWARE "swversion"

#define STATE_ON "on"								// true or false
#define STATE_HUE "hue"								// 0 - MAX_HUE
#define STATE_BRIGHTNESS "bri"						// 0 - 255
#define STATE_SATURATION "sat"						// 0 - 255
#define STATE_CIE_COORDINATES "xy"					// (0 - 1, 0 - 1)
#define STATE_TEMPERATURE "ct"						// 153 (6500K) to 500 (2000K)
#define STATE_ALERT "alert"							// none, select, or lselect
#define STATE_EFFECT "effect"						// none or colorloop
#define STATE_TRANSITION_TIME "transitiontime"		// multiple of 100ms, defaults to 4 (400ms)

#define STATE_COLORMODE "colormode"					// hs, xy, or ct
#define STATE_REACHABLE "reachable"					// always returns true

#define STATE_ALERT_NONE "none"
#define STATE_ALERT_ONE_CYCLE "select"
#define STATE_ALERT_30SEC_CYCLE "lselect"

#define STATE_EFFECT_NONE "none"
#define STATE_EFFECT_COLORLOOP "colorloop"

#define YES true
#define NO false

