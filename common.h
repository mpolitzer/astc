#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

const char *bol(const char *s, const char *min);
const char *eol(const char *s, const char *max);

#define ANSI_COLOR_RED     "\x1b[01;31m"
#define ANSI_COLOR_GREEN   "\x1b[01;32m"
#define ANSI_COLOR_YELLOW  "\x1b[01;33m"
#define ANSI_COLOR_BLUE    "\x1b[01;34m"
#define ANSI_COLOR_MAGENTA "\x1b[01;35m"
#define ANSI_COLOR_CYAN    "\x1b[01;36m"
#define ANSI_COLOR_RESET   "\x1b[01;0m"

__attribute__((noreturn))
void emit_error(
		const char *fname,
		int lineno, int charno,
		const char *level,
		const char *msg,
		const char *begin,
		const char *i,
		const char *end);
#define emit_error(fn, l, c, m, b, i, e)\
	do {emit_msg(ANSI_COLOR_RED, fn, l, c, "error", m, b, i, e); raise(SIGINT); exit(1); } while (0)

void emit_warning(
		const char *fname,
		int lineno, int charno,
		const char *level,
		const char *msg,
		const char *begin,
		const char *i,
		const char *end);
#define emit_warning(fn, l, c, m, b, i, e)\
	emit_msg(ANSI_COLOR_CYAN, fn, l, c, "warning", m, b, i, e)

void emit_msg(
		const char *color,
		const char *fname,
		int lineno, int charno,
		const char *level,
		const char *msg,
		const char *begin,
		const char *i,
		const char *end);

#define assert(exp) do { \
	if (!(exp)) emit_assert(#exp, __PRETTY_FUNCTION__, __FILE__, __LINE__); } while(0)

void emit_assert(
		const char *exp,
		const char *func,
		const char *file,
		int line);

#define NEW(n, t) t *n = malloc(sizeof(t))

#endif /* COMMON_H */

