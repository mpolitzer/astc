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

void gen_ident_only     (FILE *me, unsigned n);
void gen_ident          (FILE *me, unsigned n, const char *fmt, ...);
void gen_name           (FILE *me, Name *n, const char *fmt, ...);
void gen_type           (FILE *me, Type *t, const char *fmt, ...);
void gen_struct         (FILE *me, const char *pre, Name *d, Name *c, unsigned n, const char *post);
void gen_struct_name    (FILE *me, Name *d, Name *c);
void gen_enum_entry_name(FILE *me, Name *d, Name *c, const char *fmt, ...);

#endif /* GEN_H */

