/*
 * ## KEYBOARD LAYOUT MAPS ##
 *
 * Each map is an array where the content of each position is the key value
 * associated with the event code (index). An special/unknown key is represented
 * as a dot ('.'). The dot key itself is registered with its own macro, KEY_DOT.
 *
 * Maps are imported from files named map_XX.h, where XX is the layout code.
 *
 * Each file declares a char array called map, where the content of each
 * position is the key value associated with the event code (index). It also
 * defines a function called init_shiftmap that can register any number of shift
 * keys to be parsed when shift or capslock has been pressed, and exit_shiftmap,
 * that cleans up the map (by default, it just destroys the map used by the
 * previous function).
 *
 * For example,
 * 		EVS_SHIFT(KEY_7, "/");
 * associates the key of number 7 to the slash character when shift is pressed.
 * Yeah, the character associated is a string: this is because of the map (kmap)
 * implementation, [int -> (void *)].
 *
 * From version 0.3 up, you can also define how evspy should register AltGr-key
 * combinations, in keyboard layouts that support it, the same way that with
 * shift. The functions are [init,exit]_altgrmap. To enable this you should
 * define the macro EVS_ALTGR_ENABLED somewhere in your map layout file.
 *
 * By the moment, evspy does not support non-ascii characters.
 *
 * Select your map with EVS_KLAY macro in evspy-core.h
 */

#ifndef __EVS_MAPS
#define __EVS_MAPS

#include <linux/input.h>
#include "../kmap/kmap.h"


// Load the map associated with the given keyboard layout
#ifndef EVS_KLAY
#include "map_es.h"		// default layout
#elif EVS_KLAY == EVS_KLAY_EN
#include "map_en.h"
#elif EVS_KLAY == EVS_KLAY_ES
#include "map_es.h"
#endif	// EVS_KLAY

#endif	// __EVS_MAPS
