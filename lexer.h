#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <stdbool.h>
#include "token.h"
#include "common.h"

struct lexer_t {
	const char *fname;
	const char *data, *cur, *end;
	unsigned lineno, charno;
};
typedef struct lexer_t lexer_t;
#define CONSTRUCT_LEXER(_me, _fname, _p, _n) (lexer_t){ \
	.fname= (_fname), \
	.data = (_p), \
	.cur  = (_p), \
	.end  = (_p)+(_n), \
	.lineno= 1, \
	.charno= 0, \
}
#define DECLARE_LEXER(_name, _fname, _p, _n)\
	lexer_t _name = CONSTRUCT_LEXER(&_name, _fname, _p, _n)

lexer_t mk_lexer(const char *file);
token_e lex(lexer_t *l, token_value_t *t);

__attribute__((noreturn)) void emit_l_error(lexer_t *l, const char *msg);

#endif /* LEXER_H */

