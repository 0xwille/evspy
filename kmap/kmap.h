/*
 *   kmap - (Linux) kernel map implementation
 *
 *   Copyright (c) 2011 Guillermo Ramos <0xwille@gmail.com>
 *
 * This file is part of kmap
 *
 * kmap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kmap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kmap.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/list.h>

#ifndef _KMAP_H
#define _KMAP_H


struct kmap {
	struct list_head l;
	int value;
	void *data;
};

struct kmap *kmap_create(void);
void kmap_destroy(struct kmap *head);
int kmap_insert(struct kmap *head, int value, void *data);
int kmap_delete(struct kmap *head, int value);
void *kmap_get(struct kmap *head, int value);
void kmap_set(struct kmap *head, int value, void *data);
void kmap_display(struct kmap *head);


#endif		/* _KMAP_H */
