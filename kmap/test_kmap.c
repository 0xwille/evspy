#include <linux/module.h>
#include <linux/init.h>
#include "kmap.h"

#define Z_ASD	1
#define Z_QWE	2
#define Z_BLR	3

static struct kmap *km;

static int __init test_init(void)
{
	char *values;

	km = kmap_create();

	if ((values = (char*)kmap_get(km, Z_BLR)))
		printk(KERN_ALERT "ERROR: Z_BLR exists: %s\n", values);
	else
		printk(KERN_ALERT "Z_BLR does not exist (good)\n");

	if (kmap_insert(km, Z_ASD, "ASDF"))
		goto insert_err;
	if (kmap_insert(km, Z_QWE, "QWERTY"))
		goto insert_err;
	if (kmap_insert(km, Z_BLR, "BLRBLRBLR"))
		goto insert_err;

	kmap_display(km);
	kmap_delete(km, Z_QWE);
	kmap_display(km);
	kmap_set(km, Z_ASD, "ASDF NEW!!");
	kmap_display(km);

	if ((values = (char*)kmap_get(km, Z_ASD)))
		printk(KERN_ALERT "Z_ASD: %s\n", values);
	else
		printk(KERN_ALERT "Z_ASD does not exist\n");
	if ((values = (char*)kmap_get(km, Z_QWE)))
		printk(KERN_ALERT "Z_QWE: %s\n", values);
	else
		printk(KERN_ALERT "Z_QWE does not exist\n");
	if ((values = (char*)kmap_get(km, Z_BLR)))
		printk(KERN_ALERT "Z_BLR: %s\n", values);
	else
		printk(KERN_ALERT "Z_BLR does not exist\n");

	if (kmap_insert(km, Z_ASD, "REPEATED!!"))
		goto insert_err;

	return 0;

insert_err:
	printk(KERN_ALERT "Error inserting (good)\n");
	return 0;
}

static void __exit test_exit(void)
{
	kmap_destroy(km);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
