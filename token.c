#include "token.h"

const char *tktostr(token_e t) {
	static const char *map[] = {
		[TK_ID] = "id",
		[TK_SK] = "sk",
		[TK_EQ] = "=",
		[TK_OP] = "(",
		[TK_CP] = ")",
		[TK_COLON] = ":",
		[TK_ASTERISK] = "*",
	};
	return map[t];
}

