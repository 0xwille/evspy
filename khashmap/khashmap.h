#include <linux/list.h>

#ifndef HASHMAP
#define HASHMAP

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

#endif
