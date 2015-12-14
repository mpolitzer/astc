#include <ctype.h>
#include "lexer.h"

#define __1_to_9\
	case '1': case '2': case '3': case '4':\
	case '5': case '6': case '7': case '8': case '9'
#define __0_to_9\
	__1_to_9: case '0'

#define __a_to_f\
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f'

#define __A_to_F\
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F'

#define CASE_RANGE_g_TO_z\
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':\
	case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':\
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z'

#define CASE_RANGE_G_TO_Z\
	case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':\
	case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':\
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z'

#define __a_to_z\
	__a_to_f:\
	CASE_RANGE_g_TO_z

#define __A_to_Z\
	__A_to_F:\
	CASE_RANGE_G_TO_Z


#define cur(s) (*s->cur)
#define adv(s) (s->charno++, *s->cur++)

__attribute__((noreturn))
void emit_l_error(lexer_t *l, const char *msg) {
	const char *s = l->cur;
	emit_error(l->fname, l->lineno, l->charno, msg,
			bol(s, l->data), s, eol(s, l->end));
}

static void lex_id(lexer_t *l) {
	while (cur(l) == '_' || isalnum(cur(l)))
		adv(l);
}
token_e lex(lexer_t *l, token_value_t *t) {
	const char *s;

redo:	s = l->cur;
	switch (cur(l)) {
	case '#': adv(l);
		  while (cur(l) != '\n')
			  adv(l);
	case'\n': l->charno=0;
		  l->lineno++;
	case'\t':
	case ' ': adv(l);
		  goto redo;
	case'\0':
	case '*': 
	case '(':
	case ')':
	case ':': t->charno = l->charno;
		  t->lineno = l->lineno;
		  t->s      = s;
		  t->len    = 1;
		  return adv(l);
	case '_':
	__a_to_z:
	__A_to_Z: t->charno = l->charno;
		  t->lineno = l->lineno;
		  t->s      = s;
		  lex_id(l);
		  t->len    = l->cur - s;
		  return TK_ID;
	default: emit_l_error(l, "failed to lex character");
	}
}

static const char *read_fp_contents(FILE *in, size_t *_sz) {
	fseek(in, 0, SEEK_END);
	size_t sz = ftell(in);
	fseek(in, 0, SEEK_SET);

	char *string = malloc(sz + 1);
	fread(string, sz, 1, in);
	string[sz] = 0;

	*_sz = sz;
	return string;
}
static const char *read_contents(const char *fin, size_t *sz) {
	FILE *in = fopen(fin, "r");
	const char *buf = read_fp_contents(in, sz);
	fclose(in);
	return buf;
}
lexer_t mk_lexer(const char *fname) {
	size_t sz;
	const char *s = read_contents(fname, &sz);
	return CONSTRUCT_LEXER(&l, fname, s, sz);
}

