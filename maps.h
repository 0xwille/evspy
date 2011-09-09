/*
 * ## KEYBOARD LAYOUT MAPS ##
 *
 * Each map is an array where the content of each position is the key value
 * associated with the event code (index). An special/unknown key is represented
 * as a dot ('.'). The dot key itself has its own macro (KEY_DOT).
 *
 * By the moment, evspy does not support non-ascii characters.
 *
 * Select your map with EVS_MAP macro in evspy.h
 */

// Spanish
static char map_es[] = {
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
	'.', '.', '.', '.', '.',	//100	// 100:ALTGR 103:up_arrow
	'.', '.', '.', '.', '.',	//105	// 105:l_arrow 106:r_arrow 108:dwn_arrow
};
