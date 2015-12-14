#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>

enum token_e {
	TK_EOF = '\0',
	TK_ID,
	TK_EQ = '=',
	TK_OP = '(',
	TK_CP = ')',
	TK_COLON = ':',
	TK_ASTERISK = '*',

	TK_END
};
typedef enum token_e token_e;

struct token_value_t {
	const char *s;
	uint16_t len;
	uint16_t charno,
		 lineno;
};
typedef struct token_value_t token_value_t;

const char *tktostr(token_e t);

#endif /* TOKEN_H */

