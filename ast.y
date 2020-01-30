%{
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "AST.h"
#include "ast.tab.h"
extern int yylex();
extern int yyerror();
Node *top, *tmp1, *tmp2, *tmp3, *tmp4,tmp;  //抽象構文木のルートノードへのポインタ、tmp1：追加分，関数の引数の型に合わせるため
Node *tmp5; //define用
%}
%union{
    Node* np;
    int ival;
    char* sp;
 }
%token DEFINE ARRAY WHILE IF ELSE
%token SEMIC L_BRACKET R_BRACKET L_PARAM R_PARAM L_BRACE R_BRACE
%token ASSIGN ADD SUB MUL DIV MOD 
%token EQ LT GT

%token <sp> IDENT
%token <ival> NUMBER
%type <np> declarations decl_statement statements statement assignment_stmt expression term factor var loop_stmt cond_stmt condition 
%%

program : declarations statements {top=build_child(Pro_AST,$1,$2);}
;
declarations : decl_statement declarations {$$=build_child(Decl_AST,$1,$2);}
| decl_statement {$$=build_1_child(Decl_AST,$1);}

decl_statement: DEFINE IDENT SEMIC {tmp5=build_ident_node(IDENT_AST,$2); $$=build_1_child(DEFINE_AST,tmp5);} //build_1_child　追加
| ARRAY IDENT L_BRACKET NUMBER R_BRACKET SEMIC {$$=build_array_node(ARRAY_AST,$2,$4);}
;
statements : statement statements {$$=build_child(Stmts_AST,$1,$2);}
| statement {$$=build_1_child(Stmts_AST,$1);}
;
statement : assignment_stmt {$$=build_1_child(Stmt_AST,$1);}
| loop_stmt {$$=build_1_child(Stmt_AST,$1);}
| cond_stmt {$$=build_1_child(Stmt_AST,$1);}
;
assignment_stmt : IDENT ASSIGN expression SEMIC {tmp2=build_ident_node(IDENT_AST,$1); $$=build_child(ASSIGN_AST,tmp2,$3);}
| IDENT L_BRACKET NUMBER R_BRACKET ASSIGN expression SEMIC {tmp3=build_ident_node(IDENT_AST,$1); tmp4= build_num_node(NUM_AST,$3); $$=build_3_child(ASSIGN_AST,tmp3,tmp4,$6);}  //$3の部分は，varからNUMBER変更
;
expression : expression ADD term {$$ = build_child(ADD_AST,$1,$3);}
| expression SUB term {$$ = build_child(SUB_AST,$1,$3);}
| term {$$=build_1_child(Expr_AST,$1);}
;

term : term MUL factor {$$ = build_child(MUL_AST,$1,$3);}
| term DIV factor {$$ = build_child(DIV_AST,$1,$3);}
| term MOD factor {$$ = build_child(MOD_AST,$1,$3);}
| factor {$$=build_1_child(Term_AST,$1);}
;

factor : var {$$=build_1_child(Factor_AST,$1);}
| L_PARAM expression R_PARAM {$$=build_1_child(Factor_AST,$2);}
;

var : IDENT {$$ = build_ident_node(IDENT_AST,$1);}
| NUMBER {$$ = build_num_node(NUM_AST,$1);}
| IDENT L_BRACKET NUMBER R_BRACKET {$$ = build_array_node(ARRAY_AST,$1,$3);}
;

loop_stmt : WHILE L_PARAM condition R_PARAM L_BRACE statements R_BRACE
{$$=build_child(WHILE_AST,$3,$6);}
;

cond_stmt : IF L_PARAM condition R_PARAM L_BRACE statements R_BRACE {$$=build_child(IF_AST,$3,$6);}
| IF L_PARAM condition R_PARAM L_BRACE statements R_BRACE ELSE L_BRACE statements R_BRACE {$$=build_3_child(IF_AST,$3,$6,$10);}
;

condition : expression EQ expression {$$=build_child(EQ_AST,$1,$3);}
| expression LT expression {$$=build_child(LT_AST,$1,$3);}
| expression GT expression {$$=build_child(GT_AST,$1,$3);}
;

/*
cond_op : EQ | EQ ASSIGN| LT | LT ASSIGN | GT 
;
*/
%%

int main(void){
    FILE *fp;
    /* char str[] ="                                                    \
    INITIAL_GP = 0x10008000     # initial value of global pointer \n\
    INITIAL_SP = 0x7ffffffc     # initial value of stack pointer \n\
    # system call service number \n\
    stop_service = 99 \n\
\n\
    .text \n\
init: \n\
    # initialize $gp (global pointer) and $sp (stack pointer) \n\
    la	$gp, INITIAL_GP    # (下の2行に置き換えられる) \n\
#   lui	$gp, 0x1000     # $gp ← 0x10008000 (INITIAL_GP) \n\
#   ori	$gp, $gp, 0x8000 \n\
    la	$sp, INITIAL_SP     # (下の2行に置き換えられる) \n\
#   lui	$sp, 0x7fff     # $sp ← 0x7ffffffc (INITIAL_SP) \n\
#   ori	$sp, $sp, 0xfffc \n\
    jal	main            # jump to `main' \n\
    nop             # (delay slot) \n\
    li	$v0, stop_service   # $v0 ← 99 (stop_service) \n\
    syscall             # stop \n\
    nop \n\
    # not reach here \n\
stop:                   # if syscall return  \n\
    j stop              # infinite loop... \n\
    nop             # (delay slot) \n\
 \n\
    .text 	0x00001000";
*/
    if(yyparse()){
        fprintf(stderr,"Error\n");
        return 1;
    }

    fp=fopen("test.asm","w");

    printTree(top,fp);
    fclose(fp);
    return 0;
}
