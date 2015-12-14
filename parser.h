#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct parser_t parser_t;

struct parser_t {
	lexer_t l;
	token_e cur, nxt;
	token_value_t curtv, nxttv;
};
#define CONSTRUCT_PARSER(_lexer) (parser_t){\
	.l  = _lexer,\
}

parser_t mk_parser(lexer_t l);
Decls   *parse(parser_t *p);
/* -------------------------------------------------------------------------- */

#endif /* PARSER_H */

