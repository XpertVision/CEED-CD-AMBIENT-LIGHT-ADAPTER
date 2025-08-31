#ifndef _A_MODES_H_
#define _A_MODES_H_

//Uncomment for enabling sport mode
//#define SPORT_MODE

//Uncomment for enabling auto pairing
//#define AUTO_PAIR

//Uncomment for enabling enable LED indicator on control button
#define BUTTON_INDICATOR

//Enable for adapting LED indicator BRIGHTNESS level. Works only if BUTTON_INDICATOR also enabled. IMPORTANT: Experimental!!! Use only with board where exist Hardware PWM (FAST_MODE)!!!
//#define BUTTON_INDICATOR_ADAPTIVE_BRIGHTNESS

//Prevent Main LED Module glitch, when it turn on after power on, even if it was disabled. Use only if you see that behavior sometimes
//#define PREVENT_AUTO_ENABLE_GLITCH

#endif _A_MODES_H_