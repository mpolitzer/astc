#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

static void gen_enum(FILE *f, Decl *d, unsigned l) {
	gen_ident(f, l, "enum ");
	gen_name(f, d->n, "_e {\n");
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_ident(f, l+1, "");
		gen_enum_entry_name(f, d->n, c->n, ",\n");
	}
	gen_ident(f, l, "} tp;\n");
}
static void gen_common_struct_name(FILE *f, Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_ident(f, l, "struct ");
	gen_name(f, d->n, "_any_t");
	vfprintf(f, fmt, ap);
}
static void gen_entry(FILE *f, Entry *e, unsigned l) {
	gen_type(f, e->t, ""); // auto insert a '*'
	gen_name(f, e->n, ";\n");
}
static void gen_entries(FILE *f, Decl *d, Entries *es, unsigned l) {
	for (Entry *e = es->fst; e; e = e->nxt) {
		gen_ident(f, l, "");
		gen_entry(f, e, l);
	}
}
static void gen_the_any_struct(FILE *f, Decl *d, unsigned l) {
	gen_common_struct_name(f, d, l, " {\n");
	gen_enum(f, d, l+1);
	gen_entries(f, d, d->any, l+1);
	gen_ident(f, l, "} any;\n");
}
static void gen_individual_struct(FILE *f, Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_struct(f, "struct ", d->n, c->n, l, " {\n");
		gen_common_struct_name(f, d, l+1, " _;\n");
		gen_entries(f, d, c->es, l+1);
		gen_ident(f, l, "} ");
		gen_name(f, c->n, ";\n");
	}
	vfprintf(f, fmt, ap);
}
static void gen_decl(FILE *f, Decl *d, unsigned l) {
	fprintf(f, "/* ========== ");
	gen_name(f, d->n, " ========== */\n");
	fprintf(f, "union ");
	gen_name(f, d->n, "_t {\n");
	gen_the_any_struct(f, d, l);
	gen_individual_struct(f, d, l, "};\n\n");
}
static void gen_decls(FILE *f, Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt)
		gen_decl(f, d, l+1);
}
void gen_header(gen_t *me, Decls *ds) {
	gen_decls(me->hf, ds, 0);
}

