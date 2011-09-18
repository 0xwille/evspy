/*
 *   khm - (Linux) kernel hash map implementation
 *
 *   Copyright (c) 2011 Guillermo Ramos <0xwille@gmail.com>
 *
 * This file is part of khm
 *
 * khm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * khm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with khm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/list.h>

#ifndef KHM
#define KHM

struct khashmap {
	struct list_head l;
	int value;
	void *data;
};

struct khashmap *khm_create(void);
void khm_destroy(struct khashmap *head);
int khm_insert(struct khashmap *head, int value, void *data);
int khm_delete(struct khashmap *head, int value);
void *khm_get(struct khashmap *head, int value);
void khm_set(struct khashmap *head, int value, void *data);
void khm_display(struct khashmap *head);

#endif	// KHM
