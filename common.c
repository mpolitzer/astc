#include <stdlib.h>
#include "common.h"

void emit_msg(
		const char *color,
		const char *fname,
		int lineno, int charno,
		const char *level,
		const char *msg,
		const char *begin,
		const char *i,
		const char *end) {
	fprintf(stderr, "%s" ":%d:%d:"  "%s%s:" ANSI_COLOR_RESET " %s\n",
			fname, lineno, charno, color, level, msg);
	fprintf(stderr, "%.*s\n" ANSI_COLOR_GREEN "%*s%s" ANSI_COLOR_RESET "\n",
			(int)(end-begin), begin,
			(int)(i-begin), "", "^~");
}

void emit_assert(
		const char *exp,
		const char *func,
		const char *file,
		int line)
{
	// <exe-name>: <src-name>:<line> <function>: Assertion `<exp>' failed.
	fprintf(stderr, "%s:%d %s: Assertion `%s' failed.\n", file, line, func, exp);
	raise(SIGINT);
}

const char *bol(const char *s, const char *min) {
	while (s >= min && *s != '\n')
		s--;
	return s+1;
}

const char *eol(const char *s, const char *max) {
	while (s < max && *s != '\n')
		s++;
	return s;
}

