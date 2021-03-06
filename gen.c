#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

#define IDENT_AMOUNT "  "

void gen_ident_only(FILE *f, unsigned n) {
	for (unsigned i=0; i<n; ++i) {
		fprintf(f, IDENT_AMOUNT);
	}
}
void gen_ident(FILE *f, unsigned n, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_ident_only(f, n);
	vfprintf(f, fmt, ap);
}
void gen_name(FILE *f, Name *n, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(f, "%.*s", n->n, n->s);
	vfprintf(f, fmt, ap);
}
static void gen_type_(FILE *f, Type *t) {
	switch (t->any.tp) {
	case TYPE_BASE:
		fprintf(f, "union %.*s_t ", t->base.n, t->base.s);
		break;
	case TYPE_LIT:
		fprintf(f, "%.*s ", t->base.n, t->base.s);
		break;
	case TYPE_PTR_OF:
		gen_type_(f, t->ptr.of);
		fprintf(f, "*");
		break;
	}
}
void gen_type(FILE *f, Type *t, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_type_(f, t);
	vfprintf(f, fmt, ap);
}
void gen_lit_entry(FILE *f, Entry *e, unsigned l) {
	gen_type(f, e->t, "");
	gen_name(f, e->n, ";\n");
}
void gen_struct_entry(FILE *f, Entry *e, unsigned l) {
	fprintf (f, "struct ");
	gen_type(f, e->t, "");
	gen_name(f, e->n, ";\n");
}
void gen_union_entry(FILE *f, Entry *e, unsigned l) {
	fprintf (f, "union ");
	gen_type(f, e->t, "");
	gen_name(f, e->n, ";\n");
}
static void gen_struct_name_prefix(FILE *f, Name *d, Name *c) {
	gen_name(f, d, "_");
	gen_name(f, c, "");
}
void gen_struct(FILE *f, const char *pre, Name *d, Name *c, unsigned n, const char *pos) {
	gen_ident(f, n, "%s", pre);
	gen_struct_name_prefix(f, d, c);
	fprintf(f, "_t%s", pos);
}
void gen_function_name(FILE *f, const char *pre, Name *d, Name *c, unsigned n, const char *pos) {
	gen_ident(f, n, "%s", pre);
	gen_struct_name_prefix(f, d, c);
	fprintf(f, "%s", pos);
}
void gen_enum_entry_name(FILE *f, Name *d, Name *c, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_name(f, d, "_");
	gen_name(f, c, "_e");
	vfprintf(f, fmt, ap);
}
void gen_decl_separator(FILE *f, Decl *d) {
	fprintf(f, "/* ========== ");
	gen_name(f, d->n, " ========== */\n");
}
/* -------------------------------------------------------------------------- */

gen_t mk_gen(const char *c, const char *h) {
	return CONSTRUCT_GEN(g, c, h);
}

void gen_free(gen_t *me) {
	fclose(me->cf);
	fclose(me->hf);
}

