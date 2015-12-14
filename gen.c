#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

#define IDENT_AMOUNT "  "

static void gen_ident(unsigned n, const char *fmt, ...) {
	for (unsigned i=0; i<n; ++i) {
		printf(IDENT_AMOUNT);
	}
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
}
static void gen_name(Name *n, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	printf("%.*s", n->n, n->s);
	vprintf(fmt, ap);
}
static void gen_type_(Type *t) {
	switch (t->any.tp) {
	case TYPE_BASE:
		printf("%.*s ", t->base.n, t->base.s);
		break;
	case TYPE_PTR_OF:
		gen_type_(t->ptr.of);
		printf("*");
		break;
	}
}
static void gen_type(Type *t, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_type_(t);
	vprintf(fmt, ap);
}
static void gen_enum(Decl *d, unsigned l) {
	gen_ident(l, "enum ");
	gen_name(d->n, "_e {\n");
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_ident(l+1, "");
		gen_name(d->n, "_");
		gen_name(c->n, ",\n");
	}
	gen_ident(l, "} tp;\n");
}
static void gen_common_struct_name(Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_ident(l, "struct ");
	gen_name(d->n, "_any_t");
	vprintf(fmt, ap);
}
static void gen_common_struct(Decl *d, unsigned l) {
	gen_common_struct_name(d, l, " {\n");
	gen_enum(d, l+1);
	gen_ident(l, "} any;\n");
}
static void gen_struct_name(Name *d, Name *c, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	printf("struct ");
	gen_name(d, "_");
	gen_name(c, "_t");
	vprintf(fmt, ap);
}
static void gen_entry(Entry *e, unsigned l) {
	printf("union ");
	gen_type(e->t, "*");
	gen_name(e->n, ";\n");
}
static void gen_entries(Decl *d, Clause *c, unsigned l) {
	gen_common_struct_name(d, l, " _;\n");
	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_ident(l, "");
		gen_entry(e, l);
	}
}
static void gen_individual_struct(Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_ident(l, "");
		gen_struct_name(d->n, c->n, " {\n");
		gen_entries(d, c, l+1);
		gen_ident(l, "} ");
		gen_name(c->n, ";\n");
	}
	vprintf(fmt, ap);
}
static void gen_decl(Decl *d, unsigned l) {
	printf("union ");
	gen_name(d->n, "_t {\n");
	gen_common_struct(d, l);
	gen_individual_struct(d, l, "};\n");
}
static void gen_decls(Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt)
		gen_decl(d, l+1);
}
void gen_structs(Decls *ds) {
	gen_decls(ds, 0);
	printf("\n");
}

