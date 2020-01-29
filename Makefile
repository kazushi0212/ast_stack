CC = gcc
CFLAGS = -O -Wall
LEX = flex -d
YACC = bison -d
HDRS = ast.tab.h
LDFLAGS = -lfl -ly
LIBS = 
OBJS = ast.tab.o lex.yy.o AST.o 
#OBJS = ast.tab.o lex.yy.o AST_ch.o 
PROGRAM = compliler

all:	$(PROGRAM) 
$(PROGRAM):	$(OBJS) $(HDRS)
			$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)
#clean:; rm -f *.o *~

lex.yy.c : ast.l
	$(LEX) ast.l
ast.tab.c : ast.y
	$(YACC) ast.y
ast.o : AST.c
	$(CC) -c AST.c
lex.yy.o :lex.yy.c
	$(CC) -c lex.yy.c
ast.tab.o : ast.tab.c
	$(CC) -c ast.tab.c -DYYERROR_VERBOSE

clean:; rm -f *.o *~
###
ast:
	$(LEX) ast.l
	$(YACC) ast.y
	$(CC) -c AST.c
	#$(CC) -c AST_ch.c
	$(CC) -c lex.yy.c
	$(CC) -c ast.tab.c -DYYERROR_VERBOSE
	make $(PROGRAM)
