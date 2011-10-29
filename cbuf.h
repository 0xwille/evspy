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

#ifndef _EVS_CBUF_H
#define _EVS_CBUF_H


#include <asm/page.h>

#define CBUF_SIZE		PAGE_SIZE			// size of the circular buffer (4K)

void cbuf_init(void);
int cbuf_read(char *to, int count, int *eof);
void cbuf_write(char c);


#endif		/* _EVS_CBUF_H */
