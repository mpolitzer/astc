#include "parser.h"

#define cur(p) (p->cur)
#define nxt(p) (p->nxt)
void adv(parser_t *p) {
	p->cur  = p->nxt;
	p->curtv= p->nxttv;
	p->nxt  = lex(&p->l, &p->nxttv);
}

__attribute__((noreturn))
void emit_p_error(parser_t *p, const char *msg) {
	const char *fn  = p->l.fname;
	const char *s   = p->curtv.s;
	const char *b   = bol(s, p->l.data);
	const char *e   = eol(s, p->l.end);
	unsigned charno = p->curtv.charno;
	unsigned lineno = p->curtv.lineno;

	emit_error(fn, lineno, charno, msg, b, s, e);
}


static bool try(parser_t *p, token_e t) {
	if (cur(p) == t) {
		adv(p);
		return true;
	}
	return false;
}

static bool x(parser_t *p, token_e t) {
	if (cur(p) == t) {
		adv(p);
		return true;
	}

	emit_p_error(p, "unexpected token received");
	return false;
}
// name : <id>
static Name *name(parser_t *p) {
	if (cur(p) == TK_ID) {
		Name *name = mk_name(&p->curtv);
		adv(p);
		return name;
	}
	return NULL;
}
// type_: '*' type_ | %empty
static Type *type_(parser_t *p, Type *of) {
	Type *t = of;

	while (try(p, TK_ASTERISK)) {
		t = mk_type_of(t);
	}

	return t;
}
// type : <id>
static Type *type(parser_t *p) {
	if (cur(p) == TK_ID) {
		Type *type = mk_type_base(&p->curtv);
		adv(p);
		return type_(p, type);
	}
	return NULL;
}
// entry: type:'<id:name>
static Entry *entry(parser_t *p) {
	Type *t;
	Name *n;

	if ((t = type(p)) && x(p, ':') && (n = name(p))) {
		Entry *entry = mk_entry(t, n);
		return entry;
	}
	return NULL;
}
// entries: entry entries | entry
Entries *entries(parser_t *p) {
	Entry *e = entry(p);
	if (e == NULL) {
		emit_p_error(p, "unexpected token received");
		return NULL;
	}

	Entries *es = mk_entries();
	do {
		add_entry(es, e);
	} while ((e = entry(p)));
	return es;
}
// opt_entries: entry*
static Entries *opt_entries(parser_t *p) {
	Entries *es = mk_entries();
	Entry *e;
	while ((e = entry(p)))
		add_entry(es, e);
	return es;
}
// clause: '(' <id> opt_entries ')'
static Clause *clause(parser_t *p) {
	Name *n;
	Entries *es;

	if (try(p, '(') && (n=name(p)) && (es=opt_entries(p)) && x(p, ')')) {
		return mk_clause(n, es);
	}
	return NULL;
}
// opt_clauses: clause*;
static Clauses *opt_clauses(parser_t *p) {
	Clauses *cs = mk_clauses();
	Clause *c;
	while ((c = clause(p))) {
		add_clause(cs, c);
	}
	return cs;
}
// decl : '(' <id:name> opt_clauses ')'
static Decl *decl(parser_t *p) {
	Name    *n;
	Clauses *cs;
	if (try(p, '(') && (n=name(p)) && (cs=opt_clauses(p)) && x(p, ')')) {
		return mk_decl(n, cs);
	}
	return NULL;
}
// decls: decl decls | decl
static Decls *decls(parser_t *p) {
	Decl *d = decl(p);
	if (d == NULL) {
		emit_l_error(&p->l, "unexpected token received");
		return NULL;
	}
	Decls *ds = mk_decls();
	do {
		add_decl(ds, d);
	} while ((d = decl(p)));

	return ds;
}

/* -------------------------------------------------------------------------- */

Decls *parse(parser_t *p) {
	Decls *ds;
	adv(p);
	adv(p);
	if ((ds=decls(p)) && x(p, TK_EOF))
		return ds;
	return NULL;
}
parser_t mk_parser(lexer_t l) {
	return CONSTRUCT_PARSER(l);
}

