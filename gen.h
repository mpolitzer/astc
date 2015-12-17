#ifndef GEN_H
#define GEN_H
#include "ast.h"
#include <stdio.h>

struct gen_t {
	const char *c, *h;
	FILE *cf, *hf;
};
typedef struct gen_t gen_t;
#define CONSTRUCT_GEN(_me, _c, _h) (gen_t) {\
	.c = _c,\
	.h = _h,\
	.cf= fopen(_c, "w"),\
	.hf= fopen(_h, "w") }
#define DECLARE_GEN(_name, _c, _h) \
	gen_t _name = CONSTRUCT_GEN(&_name, _c, _h)

gen_t mk_gen(const char *c, const char *h);
void gen_free(gen_t *me);

void gen_header   (gen_t *me, Decls *ds);
void gen_functions(gen_t *me, Decls *ds);

void gen_ident_only     (FILE *f, unsigned n);
void gen_ident          (FILE *f, unsigned n, const char *fmt, ...);
void gen_name           (FILE *f, Name *n, const char *fmt, ...);
void gen_type           (FILE *f, Type *t, const char *fmt, ...);
void gen_decl_separator (FILE *f, Decl *d);

void gen_lit_entry      (FILE *f, Entry *e, unsigned l);
void gen_struct_entry   (FILE *f, Entry *e, unsigned l);
void gen_union_entry    (FILE *f, Entry *e, unsigned l);

void gen_enum_entry_name(FILE *f, Name *d, Name *c, const char *fmt, ...);
void gen_struct         (FILE *f, const char *pre, Name *d, Name *c, unsigned n, const char *post);
void gen_function_name  (FILE *f, const char *pre, Name *d, Name *c, unsigned n, const char *pos);

#endif /* GEN_H */

