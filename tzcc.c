#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "gen.h"

int main(int argc, const char *argv[]) {

	assert(argc > 1);
	parser_t p = mk_parser(mk_lexer(argv[1]));
	Decls *root = parse(&p);
	print_ast(root);
	gen_structs(root);

	return 0;
}

