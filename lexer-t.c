#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include <assert.h>

int main(int argc, const char *argv[]) {
	token_e tk = TK_EOF;
	token_value_t tv;

	assert(argc > 1);
	lexer_t l = mk_lexer(argv[1]);

	while ((tk=lex(&l, &tv)) != TK_EOF) {
		printf("[%2d:%2d]: %2d %2s %.*s\n",
				tv.lineno, tv.charno,
				tk, tktostr(tk), tv.len, tv.s);
	}
	printf("[%2d:%2d]: %2d %2s\n",
			tv.lineno, tv.charno,
			tk, "eof");
	return 0;
}

