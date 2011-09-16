/*
 *   Copyright (c) 2011 Guillermo Ramos <0xwille@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can mail your message to
 * <0xwille@gmail.com>
 */

#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "khashmap.h"

static inline struct khashmap *khm_search(struct khashmap *head, int value)
{
	struct list_head *list;
	struct khashmap *node;

	list_for_each(list, &head->l) {
		node = list_entry(list, struct khashmap, l);
		if (node->value == value)
			return node;
	}

	return NULL;
}

void khm_display(struct khashmap *head)
{
	struct khashmap *node;
	struct list_head *list;
	int i = 0;
	int limit = 10;

	printk(KERN_ALERT "Displaying...");
	list_for_each(list, &head->l) {
		node = list_entry(list, struct khashmap, l);
		printk(KERN_ALERT "  N%d (%p) v=%d d=%s\n", i++, node,
				node->value, (char*)node->data);
		if (i >= limit)
			break;
	}
}

inline struct khashmap *khm_create(void)
{
	struct khashmap *head;

	head = kmalloc(sizeof(struct khashmap), GFP_KERNEL);
	if (!head)
		return NULL;

	INIT_LIST_HEAD(&head->l);

	return head;
}

void khm_destroy(struct khashmap *head)
{
	struct khashmap *node;
	struct list_head *list = head->l.next;

	while (list != &head->l) {
		node = list_entry(list, struct khashmap, l);
		list = list->next;
		kfree(node);
	}

	kfree(head);
}

int khm_insert(struct khashmap *head, int value, void *data)
{
	struct khashmap *new;

	// Key already exists
	if (khm_search(head, value))
		return -EINVAL;

	new = khm_create();
	if (!new)
		return -ENOMEM;

	new->value = value;
	new->data = data;
	list_add_tail(&new->l, &head->l);

	return 0;
}

int khm_delete(struct khashmap *head, int value)
{
	struct khashmap *node = khm_search(head, value);

	if (node) {
		list_del(&node->l);
		kfree(node);
		return 0;
	} else {
		return -EINVAL;
	}
}

void *khm_get(struct khashmap *head, int value)
{
	struct khashmap *node = khm_search(head, value);

	if (node)
		return node->data;
	else
		return NULL;
}

void khm_set(struct khashmap *head, int value, void *data)
{
	struct khashmap *node = khm_search(head, value);

	if (node)
		node->data = data;
}
