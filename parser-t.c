#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, const char *argv[]) {

	assert(argc > 1);
	parser_t p = mk_parser(mk_lexer(argv[1]));
	print_ast(parse(&p));

	return 0;
}

