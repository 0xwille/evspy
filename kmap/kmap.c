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

#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "kmap.h"

/*
 * Returns the node with the value passed, or NULL in case it does not exist
 */
static inline struct kmap *kmap_search(struct kmap *head, int value)
{
	struct list_head *list;
	struct kmap *node;

	list_for_each(list, &head->l) {
		node = list_entry(list, struct kmap, l);
		if (node->value == value)
			return node;
	}

	return NULL;
}

/*
 * Prints in klog a representation of all the nodes in the map
 */
void kmap_display(struct kmap *head)
{
	struct kmap *node;
	struct list_head *list;

	printk(KERN_ALERT "Displaying...");
	list_for_each(list, &head->l) {
		node = list_entry(list, struct kmap, l);
		printk(KERN_ALERT "  %p: v=%d d=%s\n",
				node, node->value, (char*)node->data);
	}
}

/*
 * Creates and returns a new head node
 */
inline struct kmap *kmap_create(void)
{
	struct kmap *head;

	head = kmalloc(sizeof(struct kmap), GFP_KERNEL);
	if (unlikely(!head))
		return NULL;

	INIT_LIST_HEAD(&head->l);

	return head;
}

/*
 * Deletes the map and frees the memory used by its nodes
 */
void kmap_destroy(struct kmap *head)
{
	struct kmap *node;
	struct list_head *list = head->l.next;

	while (list != &head->l) {
		node = list_entry(list, struct kmap, l);
		list = list->next;
		kfree(node);
	}

	kfree(head);
}

/*
 * Creates a new node with the given value and data, and adds it to the map head
 */
int kmap_insert(struct kmap *head, int value, void *data)
{
	struct kmap *new;

	// Key already exists
	if (kmap_search(head, value))
		return -EINVAL;

	new = kmap_create();
	if (unlikely(!new))
		return -ENOMEM;

	new->value = value;
	new->data = data;
	list_add_tail(&new->l, &head->l);

	return 0;
}

/*
 * Removes from the map the node with the given value.
 */
int kmap_delete(struct kmap *head, int value)
{
	struct kmap *node = kmap_search(head, value);

	if (node) {
		list_del(&node->l);
		kfree(node);
		return 0;
	} else {
		return -EINVAL;
	}
}

/*
 * Returns the data mapped to the given value, or NULL if that value has no data
 * associated to it
 */
void *kmap_get(struct kmap *head, int value)
{
	struct kmap *node = kmap_search(head, value);

	if (node)
		return node->data;
	else
		return NULL;
}

/*
 * Associates the given value with new data, if the value was already in the map
 */
void kmap_set(struct kmap *head, int value, void *data)
{
	struct kmap *node = kmap_search(head, value);

	if (node)
		node->data = data;
}
