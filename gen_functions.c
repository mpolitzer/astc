#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

static void gen_type_name_from_decl(gen_t *me, Decl *d) {
	fprintf(me->cf, "union ");
	gen_name(me->cf, d->n, "_t *");
}
static void gen_mengled_function_name(gen_t *me, const char *prefix, Decl *d, Clause *c) {
	fprintf(me->cf, "%s", prefix);
	gen_name(me->cf, d->n, "_");
	gen_name(me->cf, c->n, "");
}


static void gen_entry(gen_t *me, Entry *e, unsigned l) {
	fprintf(me->cf, "union ");
	gen_type(me->cf, e->t, "* ");
	gen_name(me->cf, e->n, "");
}
static void gen_fn_declaration_header(gen_t *me, Decl *d, Clause *c, unsigned l) {
	gen_type_name_from_decl(me, d);
	gen_struct(me->cf, "mk_", d->n, c->n, 0, "(");
	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_entry(me, e, l);
		fprintf(me->cf, "%s", e->nxt ? ", " : ") {\n");
	}
}
static void gen_fn_declaration_body(gen_t *me, Decl *d, Clause *c, unsigned l) {
	gen_struct(me->cf, "NEW(me, struct ", d->n, c->n, l, ");\n");

	gen_ident(me->cf, l, "me->_.tp = ");
	gen_enum_entry_name(me->cf, d->n, c->n, ";\n");

	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_ident(me->cf, l, "me->");
		gen_name (me->cf, e->n, " = ");
		gen_name (me->cf, e->n, ";\n");
	}
	gen_ident(me->cf, l, "return (union ");
	gen_name(me->cf, d->n, "_t *) me;\n");
	fprintf(me->cf, "}\n");
}
static void gen_decl(gen_t *me, Decl *d, unsigned l) {
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_fn_declaration_header(me, d, c, l);
		gen_fn_declaration_body(me, d, c, l);
	}
}
static void gen_decls(gen_t *me, Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt)
		gen_decl(me, d, l+1);
	fprintf(me->cf, "\n");
}
void gen_functions(gen_t *me, Decls *ds) {
	fprintf(me->cf,
		"#include <stdlib.h>\n"
		"#include \"%s\"\n" // TODO: fixme!
		"#ifndef NEW\n"
		"#define NEW(_name, _type) _type *_name = (_type *)malloc(sizeof(_type))\n"
		"#endif\n\n",
		me->h);

	gen_decls(me, ds, 0);
}

