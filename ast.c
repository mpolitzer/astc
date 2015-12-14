#include "common.h"
#include "ast.h"

Name *mk_name(token_value_t *tv) {
	NEW(me, Name);
	me->s = tv->s;
	me->n = tv->len;
	return me;
}

Type *mk_type_base(token_value_t *tv) {
	NEW(me, Type);
	me->any.tp = TYPE_BASE;
	me->base.s = tv->s;
	me->base.n = tv->len;
	return me;
}

Type *mk_type_of(Type *of) {
	NEW(me, Type);
	me->any.tp = TYPE_PTR_OF;
	me->ptr.of = of;
	return me;
}

Entry *mk_entry(Type *t, Name *n) {
	NEW(me, Entry);
	me->t = t;
	me->n = n;
	return me;
}

Entries *mk_entries(void) {
	NEW(me, Entries);
	me->fst = NULL;
	me->lst = &me->fst;
	return me;
}

Entries *add_entry(Entries *me, Entry *nxt) {
	*me->lst = nxt;
	me->lst = &nxt->nxt;
	return me;
}

Clause *mk_clause(Name *n, Entries *es) {
	NEW(me, Clause);
	me->n  = n;
	me->es = es;
	return me;
}

Clauses *mk_clauses(void) {
	NEW(me, Clauses);
	me->fst = NULL;
	me->lst = &me->fst;
	return me;
}

Clauses *add_clause(Clauses *me, Clause *nxt) {
	*me->lst = nxt;
	me->lst = &nxt->nxt;
	return me;
}

Decl *mk_decl(Name *n, Clauses *cs) {
	NEW(me, Decl);
	me->n  = n;
	me->cs = cs;
	return me;
}

Decls *mk_decls(void) {
	NEW(me, Decls);
	me->fst = NULL;
	me->lst = &me->fst;
	return me;
}
Decls *add_decl(Decls *me, Decl *nxt) {
	*me->lst = nxt;
	me->lst = &nxt->nxt;
	return me;
}

