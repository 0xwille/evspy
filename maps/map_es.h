/*
 * ## SPANISH KEYBOARD LAYOUT ## *
 *
 */

#ifndef __EVS_MAP_ES
#define __EVS_MAP_ES


// Right Alt (ALTGR) map
#define EVS_ALTGR_ENABLED
#define EVS_ALTGR_BIND(KEY, VALUE)	kmap_insert(akm, (KEY), (VALUE))

static struct kmap *akm;

static void init_altgrmap(void)
{
	akm = kmap_create();
	EVS_ALTGR_BIND(KEY_1, "|");
	EVS_ALTGR_BIND(KEY_2, "@");
	EVS_ALTGR_BIND(KEY_3, "#");
	EVS_ALTGR_BIND(KEY_4, "~");
//	EVS_ALTGR_BIND(KEY_5, "½");		// TODO enable
//	EVS_ALTGR_BIND(KEY_6, "¬");		// TODO enable
	EVS_ALTGR_BIND(KEY_7, "{");
	EVS_ALTGR_BIND(KEY_8, "[");
	EVS_ALTGR_BIND(KEY_9, "]");
	EVS_ALTGR_BIND(KEY_0, "}");
	EVS_ALTGR_BIND(KEY_MINUS, "\\");
	EVS_ALTGR_BIND(KEY_EQUAL, "~");
	EVS_ALTGR_BIND(KEY_LEFTBRACE, "[");
	EVS_ALTGR_BIND(KEY_RIGHTBRACE, "]");
	EVS_ALTGR_BIND(KEY_SEMICOLON, "~");
	EVS_ALTGR_BIND(KEY_APOSTROPHE, "{");
	EVS_ALTGR_BIND(KEY_GRAVE, "}");
	EVS_ALTGR_BIND(KEY_BACKSLASH, "}");
	EVS_ALTGR_BIND(KEY_COMMA, "─");
//	EVS_ALTGR_BIND(KEY_DOT, "·");		// TODO enable
	EVS_ALTGR_BIND(KEY_102ND, "|");
}

static inline void exit_altgrmap(void)
{
	kmap_destroy(akm);
}

// Shift map
#define EVS_SHIFT_BIND(KEY, VALUE)	kmap_insert(skm, (KEY), (VALUE))

static struct kmap *skm;

static void init_shiftmap(void)
{
	skm = kmap_create();
	EVS_SHIFT_BIND(KEY_1, "!");
	EVS_SHIFT_BIND(KEY_2, "\"");
	EVS_SHIFT_BIND(KEY_4, "$");
	EVS_SHIFT_BIND(KEY_5, "%");
	EVS_SHIFT_BIND(KEY_6, "&");
	EVS_SHIFT_BIND(KEY_7, "/");
	EVS_SHIFT_BIND(KEY_8, "(");
	EVS_SHIFT_BIND(KEY_9, ")");
	EVS_SHIFT_BIND(KEY_0, "=");
	EVS_SHIFT_BIND(KEY_MINUS, "?");
	EVS_SHIFT_BIND(KEY_EQUAL, "?");		// TODO change to '¿'
	EVS_SHIFT_BIND(KEY_LEFTBRACE, "^");
	EVS_SHIFT_BIND(KEY_RIGHTBRACE, "*");
	EVS_SHIFT_BIND(KEY_SEMICOLON, "N");	// TODO change to 'Ñ'
//	EVS_SHIFT_BIND(KEY_GRAVE, "Ç");		// TODO enable
	EVS_SHIFT_BIND(KEY_COMMA, ";");
	EVS_SHIFT_BIND(KEY_DOT, ":");
	EVS_SHIFT_BIND(KEY_SLASH, "_");
	EVS_SHIFT_BIND(KEY_102ND, ">");
}

static inline void exit_shiftmap(void)
{
	kmap_destroy(skm);
}

static char map[] = {
	'.', '.', '1', '2', '3',	//0		// 1:ESC
	'4', '5', '6', '7', '8',	//5
	'9', '0', '\'', '!', '\b',	//10	// 13:¡ (NOASCII) 14:BACKSPACE
	'.', 'q', 'w', 'e', 'r',	//15	// 15:TAB
	't', 'y', 'u', 'i', 'o',	//20
	'p', '`', '+', '\n', '.',	//25	// 29:LCTRL
	'a', 's', 'd', 'f', 'g',	//30
	'h', 'j', 'k', 'l', 'n',	//35	// 39:ñ (NOASCII)
	'\'', '.', '.', 'c', 'z',	//40	// 42:LMAYUS 43:ç (NOASCII)
	'x', 'c', 'v', 'b', 'n',	//45
	'm', ',', '.', '-', '.',	//50	// 52:. 54:RMAYUS
	'*', '.', ' ', '.', '.',	//55	// 56:ALT 58:BLKMAYUS 59-68:F1-F10
	'.', '.', '.', '.', '.',	//60
	'.', '.', '.', '.', '.',	//65
	'.', '7', '8', '9', '-',	//70
	'4', '5', '6', '+', '1',	//75
	'2', '3', '.', '.', '.',	//80
	'.', '<', '.', '.', '.',	//85
	'.', '.', '.', '.', '.',	//90
	'.', '\n', '.', '/', '.',	//95	// 97:RCTRL
};


#endif	// __EVS_MAP_ES
