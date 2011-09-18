/*
 * ## ENGLISH KEYBOARD LAYOUT ## *
 *
 */

#ifndef __EVS_MAP_EN
#define __EVS_MAP_EN

static void init_shiftmap(void)
{
	shm = khm_create();
	EVS_SHIFT(KEY_1, "!");
	EVS_SHIFT(KEY_2, "@");
	EVS_SHIFT(KEY_3, "#");
	EVS_SHIFT(KEY_4, "$");
	EVS_SHIFT(KEY_5, "%");
	EVS_SHIFT(KEY_6, "^");
	EVS_SHIFT(KEY_7, "&");
	EVS_SHIFT(KEY_8, "*");
	EVS_SHIFT(KEY_9, "(");
	EVS_SHIFT(KEY_0, ")");
	EVS_SHIFT(KEY_MINUS, "_");
	EVS_SHIFT(KEY_EQUAL, "+");
	EVS_SHIFT(KEY_LEFTBRACE, "{");
	EVS_SHIFT(KEY_RIGHTBRACE, "}");
	EVS_SHIFT(KEY_SEMICOLON, ":");
	EVS_SHIFT(KEY_APOSTROPHE, "\"");
	EVS_SHIFT(KEY_GRAVE, "|");
	EVS_SHIFT(KEY_BACKSLASH, "|");
	EVS_SHIFT(KEY_COMMA, "<");
	EVS_SHIFT(KEY_DOT, ">");
	EVS_SHIFT(KEY_SLASH, "?");
	EVS_SHIFT(KEY_102ND, ">");
}

static char map[] = {
	'.', '.', '1', '2', '3',	//0		// 1:ESC
	'4', '5', '6', '7', '8',	//5
	'9', '0', '-', '=', '\b',	//10	// 14:BACKSPACE
	'.', 'q', 'w', 'e', 'r',	//15	// 15:TAB
	't', 'y', 'u', 'i', 'o',	//20
	'p', '[', ']', '\n', '.',	//25	// 29:LCTRL
	'a', 's', 'd', 'f', 'g',	//30
	'h', 'j', 'k', 'l', ';',	//35
	'\'', '.', '.', '\\', 'z',	//40	// 42:LMAYUS
	'x', 'c', 'v', 'b', 'n',	//45
	'm', ',', '.', '/', '.',	//50	// 52:. 54:RMAYUS
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


#endif	// __EVS_MAP_EN
