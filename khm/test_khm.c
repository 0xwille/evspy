#include <linux/module.h>
#include <linux/init.h>
#include "khashmap.h"

#define Z_ASD	1
#define Z_QWE	2
#define Z_BLR	3

static struct khashmap *hm;

static int __init test_init(void)
{
	char *values;

	hm = khm_create();

	if (khm_insert(hm, Z_ASD, "ASDF"))
		goto insert_err;
	if (khm_insert(hm, Z_QWE, "QWERTY"))
		goto insert_err;
	if (khm_insert(hm, Z_BLR, "BLRBLRBLR"))
		goto insert_err;

	khm_display(hm);
	khm_delete(hm, Z_QWE);
	khm_display(hm);
	khm_set(hm, Z_ASD, "ASDF NEW!!");
	khm_display(hm);

	if ((values = (char*)khm_get(hm, Z_ASD)))
		printk(KERN_ALERT "Z_ASD: %s\n", values);
	else
		printk(KERN_ALERT "Z_ASD does not exist\n");
	if ((values = (char*)khm_get(hm, Z_QWE)))
		printk(KERN_ALERT "Z_QWE: %s\n", values);
	else
		printk(KERN_ALERT "Z_QWE does not exist\n");
	if ((values = (char*)khm_get(hm, Z_BLR)))
		printk(KERN_ALERT "Z_BLR: %s\n", values);
	else
		printk(KERN_ALERT "Z_BLR does not exist\n");

	if (khm_insert(hm, Z_ASD, "REPEATED!!"))
		goto insert_err;

	return 0;

insert_err:
	printk(KERN_ALERT "Error inserting!\n");
	return 0;
}

static void __exit test_exit(void)
{
	khm_destroy(hm);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
