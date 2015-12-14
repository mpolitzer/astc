#ifndef AST_H
#define AST_H
#include "token.h"

/* --------------------------------------------------------------------------
 * decls: decl decls | decl;
 * decl : '(' <id:name> opt_clauses ')';
 *
 * opt_clauses: clauses | %empty;
 * clauses: clause clauses;
 * clause: '(' <id> opt_entries ')';
 *
 * opt_entries: entries | %empty;
 *
 * entries: entry entries | entry;
 *
 * entry: type:name
 *
 * type : <id> | <id> typr_;
 * type_: '*' type_ | %empty;;
 * name : <id>;
 * -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
typedef struct name_t Name;
struct name_t {
	const char *s;
	unsigned    n;
};

Name *mk_name(token_value_t *tv);

/* -------------------------------------------------------------------------- */
typedef struct type_t Type;
struct type_t {
	const char *s;
	unsigned    n;
};

Type *mk_type(token_value_t *tv);


/* -------------------------------------------------------------------------- */
typedef struct entry_t Entry;
struct entry_t {
	Type *t;
	Name *n;
	struct entry_t *nxt;
};

Entry *mk_entry(Type *t, Name *n);

typedef struct entries_t Entries;
struct entries_t {
	Entry *fst, **lst;
};

Entries *mk_entries(void);
Entries *add_entry (Entries *es, Entry *nxt);

/* -------------------------------------------------------------------------- */
typedef struct clause_t Clause;
struct clause_t {
	Name    *n;
	Entries *es;

	struct clause_t *nxt;
};

Clause *mk_clause(Name *n, Entries *es);

typedef struct clauses_t Clauses;
struct clauses_t {
	Clause *fst, **lst;
};

Clauses *mk_clauses(void);
Clauses *add_clause(Clauses *me, Clause *nxt);
/* -------------------------------------------------------------------------- */
typedef struct decl_t Decl;
struct decl_t {
	Name    *n;
	Clauses *cs;

	struct decl_t *nxt;
};

Decl *mk_decl(Name *n, Clauses *cs);

typedef struct decls_t Decls;
struct decls_t {
	Decl *fst, **lst;
};

Decls *mk_decls(void);
Decls *add_decl(Decls *me, Decl *nxt);

/* ========================================================================== */

void print_ast(Decls *ds);

#endif /* AST_H */
