#include <stdarg.h>
#include <stdio.h>
#include "common.h"
#include "ast.h"

static void print_name(Name *n, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	printf("%*.s%.*s", l, "", n->n, n->s);
	vprintf(fmt, ap);
}
static void print_type_(Type *t) {
	switch (t->any.tp) {
	case TYPE_BASE:
		printf("%.*s", t->base.n, t->base.s);
		break;
	case TYPE_PTR_OF:
		print_type_(t->ptr.of);
		printf("*");
		break;
	default: emit_error(0, 0, 0, "invalid type", 0, 0, 0);
	}
}
static void print_type(Type *t, unsigned l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	print_type_(t);
	vprintf(fmt, ap);
}
static void print_entry(Entry *e, unsigned l) {
	printf("%*.s", l, "");
	print_type(e->t, l, ":");
	print_name(e->n, 0, "");
}
static void print_entries(Entries *es, unsigned l) {
	for (Entry *e = es->fst; e; e = e->nxt) {
		printf("\n");
		print_entry(e, l);
	}
	printf("\n");
}
static void print_clause(Clause *c, unsigned l) {
	print_name(c->n, l, " ");
	print_entries(c->es, l);
}
static void print_clauses(Clauses *cs, unsigned l) {
	for (Clause *c = cs->fst; c; c = c->nxt) {
		printf("\n");
		print_clause(c, l+1);
	}
}
static void print_decl(Decl *d, unsigned l) {
	print_name(d->n, l, " ");
	print_clauses(d->cs, l+1);
}
static void print_decls(Decls *ds, unsigned l) {
	for (Decl *d = ds->fst; d; d = d->nxt)
		print_decl(d, l);
}
void print_ast(Decls *ds) {
	print_decls(ds, 0);
}


