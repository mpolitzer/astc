#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "gen.h"

int main(int argc, const char *argv[]) {

	assert(argc > 3);
	parser_t p = mk_parser(mk_lexer(argv[1]));
	Decls *root = parse(&p);

	gen_t    g = mk_gen(argv[2], argv[3]);
	gen_header(&g, root);
	gen_functions(&g, root);
	gen_free(&g);

	return 0;
}

