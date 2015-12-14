cc=clang
rm=$(RM)
make=$(MAKE)

# common .o
common-o=\
	obj/ast.o\
	obj/common.o\
	obj/gen.o\
	obj/lexer.o\
	obj/parser.o\
	obj/print-ast.o\
	obj/token.o\

tzcc-t=bin/tzcc
tzcc-o=obj/tzcc.o
tzcc-c=${tzcc-o:.o=.c}

parser-t=bin/parser-t
parser-o=obj/parser-t.o
parser-c=${parser-o:.o=.c}

lexer-t=bin/lexer-t
lexer-o=obj/lexer-t.o
lexer-c=${lexer-o:.o=.c}

cflags-d =-g -march=native -std=c99 -O0 -Wall -pedantic
cflags-a =-g -march=native -std=c99 -O0 -Wall -pedantic -fsanitize=address
cflags-r =-g -march=native -std=c99 -O2 -Wall -pedantic -Werror
ldlibs =-O2 -lm

o=$(tzcc-o) $(parser-o) $(lexer-o) $(common-o)
c=${o:.o=.c}
d=${o:.o=.o.d}

# ==============================================================================

.PHONY: debug release test clean
debug:   $(tzcc-t) $(parser-t) $(lexer-t)
debug:   cflags+=$(cflags-d)
debug:   ldlibs+=$(ldlibs-d)

asan:    $(tzcc-t) $(parser-t) $(lexer-t)
asan:    cflags+=$(cflags-a)
asan:    ldlibs+=$(ldlibs-a)

release: $(tzcc-t) $(parser-t) $(lexer-t)
release: cflags+=$(cflags-r)
release: ldlibs+=$(ldlibs-r)

$(tzcc-t): $(tzcc-o) $(common-o)
	$(cc) $^ $(ldlibs) $(cflags) -o $@

$(parser-t): $(parser-o) $(common-o)
	$(cc) $^ $(ldlibs) $(cflags) -o $@

$(lexer-t): $(lexer-o) $(common-o)
	$(cc) $^ $(ldlibs) $(cflags) -o $@

# ==============================================================================

test: $(lexer-t)
	./scripts/test "lexer"  "./$(lexer-t)"
	./scripts/test "parser" "./$(parser-t)"

%.svg: %.dot
	$(dot) $^ -Tsvg -o $@

$(o) : obj/%.o: %.c
	$(cc) -MM -MQ $@ $< > $@.d
	$(cc) $(cflags) -c $< -o $@

clean:
	$(rm) $(o) $(parser-t) $(lexer-t)

-include $(d)
