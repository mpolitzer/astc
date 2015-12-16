#include <stdarg.h>
#include <stdio.h>
#include "gen.h"

static void gen_type_name_from_decl(FILE *f, Decl *d) {
	fprintf(f, "union ");
	gen_name(f, d->n, "_t *");
}
static void gen_entry(FILE *f, Entry *e, unsigned l) {
	gen_type(f, e->t, " ");
	gen_name(f, e->n, "");
}
static void gen_fn_declaration_header(FILE *f, Decl *d, Clause *c, unsigned l) {
	gen_type_name_from_decl(f, d);
	gen_function_name(f, "mk_", d->n, c->n, 0, "(");
	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_entry(f, e, l);
		fprintf(f, "%s", e->nxt ? ", " : "");
	}
	fprintf(f, ") {\n");
}
static void gen_fn_declaration_body(FILE *f, Decl *d, Clause *c, unsigned l) {
	gen_struct(f, "NEW(me, struct ", d->n, c->n, l, ");\n");

	gen_ident(f, l, "me->_.tp = ");
	gen_enum_entry_name(f, d->n, c->n, ";\n");

	for (Entry *e = c->es->fst; e; e = e->nxt) {
		gen_ident(f, l, "me->");
		gen_name (f, e->n, " = ");
		gen_name (f, e->n, ";\n");
	}
	gen_ident(f, l, "return (union ");
	gen_name(f, d->n, "_t *) me;\n");
	fprintf(f, "}\n\n");
}
static void gen_decl(FILE *f, Decl *d, unsigned l) {
	for (Clause *c = d->cs->fst; c; c = c->nxt) {
		gen_fn_declaration_header(f, d, c, l);
		gen_fn_declaration_body(f, d, c, l);
	}
}
static void gen_decls(FILE *f, Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt) {
		gen_decl(f, d, l+1);
	}
	fprintf(f, "\n");
}
void gen_functions(gen_t *me, Decls *ds) {
	fprintf(me->cf,
		"#include <stdlib.h>\n"
		"#include \"%s\"\n"
		"#ifndef NEW\n"
		"#define NEW(_name, _type) _type *_name = (_type *)malloc(sizeof(_type))\n"
		"#endif\n\n",
		me->h);

	gen_decls(me->cf, ds, 0);
}

