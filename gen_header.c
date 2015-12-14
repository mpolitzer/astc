#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

static void gen_enum(gen_t *me, Decl *d, unsigned l) {
	gen_ident(me->hf, l, "enum ");
	gen_name(me->hf, d->n, "_e {\n");
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_ident(me->hf, l+1, "");
		gen_enum_entry_name(me->hf, d->n, c->n, ",\n");
	}
	gen_ident(me->hf, l, "} tp;\n");
}
static void gen_common_struct_name(gen_t *me, Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gen_ident(me->hf, l, "struct ");
	gen_name(me->hf, d->n, "_any_t");
	vfprintf(me->hf, fmt, ap);
}
static void gen_common_struct(gen_t *me, Decl *d, unsigned l) {
	gen_common_struct_name(me, d, l, " {\n");
	gen_enum(me, d, l+1);
	gen_ident(me->hf, l, "} any;\n");
}
static void gen_entry(gen_t *me, Entry *e, unsigned l) {
	fprintf (me->hf, "union ");
	gen_type(me->hf, e->t, "*"); // auto insert a '*'
	gen_name(me->hf, e->n, ";\n");
}
static void gen_entries(gen_t *me, Decl *d, Clause *c, unsigned l) {
	gen_common_struct_name(me, d, l, " _;\n");
	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_ident(me->hf, l, "");
		gen_entry(me, e, l);
	}
}
static void gen_individual_struct(gen_t *me, Decl *d, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_struct(me->hf, "struct ", d->n, c->n, l, " {\n");
		gen_entries(me, d, c, l+1);
		gen_ident(me->hf, l, "} ");
		gen_name(me->hf, c->n, ";\n");
	}
	vfprintf(me->hf, fmt, ap);
}
static void gen_decl(gen_t *me, Decl *d, unsigned l) {
	fprintf(me->hf, "/* ========== ");
	gen_name(me->hf, d->n, " ========== */\n");
	fprintf(me->hf, "union ");
	gen_name(me->hf, d->n, "_t {\n");
	gen_common_struct(me, d, l);
	gen_individual_struct(me, d, l, "};\n\n");
}
static void gen_decls(gen_t *me, Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt)
		gen_decl(me, d, l+1);
}
void gen_header(gen_t *me, Decls *ds) {
	gen_decls(me, ds, 0);
}

