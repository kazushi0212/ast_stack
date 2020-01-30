#include<stdio.h>
#include<stdlib.h> 
#include <ctype.h>

typedef enum{
    Pro_AST,
    Decl_AST,
    Decl_stmt_AST,
    Stmts_AST,
    Stmt_AST,

    IDENT_AST, //変数のノード型
    NUM_AST,  

    Expr_AST,
    Term_AST,
    Factor_AST,  //IDENT,NUMの親ノード
 
    DEFINE_AST,
    ARRAY_AST,
    WHILE_AST,
    IF_AST,
    ELSE_AST,
    ELSEIF_AST,

    ASSIGN_AST,
    ADD_AST,
    SUB_AST,
    MUL_AST,
    DIV_AST,
    MOD_AST,

    EQ_AST,
    LT_AST,
    GT_AST,
}NType;

typedef struct node{
    NType type;
    char *varName;
    int value;
    struct node *child;
    struct node *brother;
}Node;

#define SIMNAMELEN 64
#define MAXSIMENTRIES 100

typedef struct simTableEntry {
    char name[SIMNAMELEN];
    //SType type;
    //SScope scope;
    //SSize size;
    //SValue value;
}SimTableEntry;

SimTableEntry simTable[MAXSIMENTRIES];

Node *build_1_child(NType t,Node *p1);
Node *build_child(NType t,Node *p1,Node *p2);
Node *build_3_child(NType t,Node *p1,Node *p2,Node *p3);
Node *build_4_child(NType t,Node *p1,Node *p2,Node *p3,Node *p4);
Node *build_num_node(NType t,int n);
Node *build_ident_node(NType t,char *str);
Node *build_array_node(NType t,char *str,int n);
//Node *build_elseif_child(NType t,Node *p1,Node *p2,Node *p3);
void printTree(Node *p,FILE *fp);
void checkNode(Node *p,FILE *fp);



