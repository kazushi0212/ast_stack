#include<stdio.h>
#include<stdlib.h> 
#include <string.h>
#include "AST.h"

int Decl_num=0; // Decl_AStの時，一度だけ .data 0x10004000を表示するため

int stack_num=0;


//SimTableEntry simTable[100];
Node *build_1_child(NType t,Node *p1){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->child = p1;
    p->brother=NULL;
    return p;
}

Node *build_child(NType t,Node *p1,Node *p2){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->child = p1;
    p1->brother = p2;
    p->brother=NULL;
    return p;
}


Node *build_3_child(NType t,Node *p1,Node *p2,Node *p3){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->child = p1;
    p1->brother = p2;
    p1->brother = p3;
    p->brother=NULL;
    return p;
}

Node *build_num_node(NType t,int n){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->value = n;
    p->child = NULL;
    p->brother = NULL;
    return p;
}


Node *build_ident_node(NType t,char *str){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->varName = (char*)malloc(sizeof(char)*strlen(str));
    strcpy(p->varName,str);
    p->child = NULL;
    p->brother = NULL;
    return p;
}

Node *build_array_node(NType t,char *str,int n){
    Node *p,*p1,*p2;
    p1=build_ident_node(IDENT_AST,str);
    p2=build_num_node(NUM_AST,n);
    p=build_child(t,p1,p2);
    return p;
}

void checkchild(Node *p,FILE *fp){
  if(p->child != NULL){
    printTree(p->child,fp);
  }

  if(p->brother != NULL){
    printTree(p->brother,fp);
  } 
}

void printTree(Node *p,FILE *fp){
  int t_reg=0; //レジスタ$tの引数
  if(p != NULL){
        switch(p->type){
        case Pro_AST:
            printf("PROGRAM\n");
	    checkchild(p,fp);
            break;
        case Decl_AST:
            printf("DECLARATION\n");
            if(Decl_num==0){
                fprintf(fp,"    .data   0x00004000\n");
                Decl_num++;
            }
	    checkchild(p,fp);
            break;
        case Decl_stmt_AST:
            printf("Decl_statement\n");
	    checkchild(p,fp);
            break;
        case Stmts_AST:
            printf("statements\n");
            if(p->child != NULL){
                printf("(");
                printTree(p->child,fp);
            }
            break;
        case Stmt_AST:
            printf("statement\n");
	    checkchild(p,fp);
            break;
            
////////NUM,IDENT/////
        case IDENT_AST:
            printf("%s\t",p->varName);
            fprintf(fp,"    sw   %s,%d($sp)",p->varName,stack_num);
	    stack_num=stack_num+4;
	    checkchild(p,fp);
            break;
        case NUM_AST:
            printf("%d\t",p->value);
            fprintf(fp,"    sw   %d,%d($sp)",p->value,stack_num);
	    stack_num=stack_num+4;
	    checkchild(p,fp);
            break;


        case Expr_AST:
            printf("expression\n");
	    checkchild(p,fp);
            break;
        case Term_AST:
            printf("term\n");
	    checkchild(p,fp);
            break;
        case Factor_AST:
            printf("factor\n");
	    checkchild(p,fp);
            break;
            
//////////DEFINE,ARRAY
        case DEFINE_AST:
            printf("DEFINE\n");
	    fprintf(fp,"%s   .word  0x0000",p->child->varName);
	    checkchild(p,fp);
            break;
        case ARRAY_AST:
            printf("ARRAY\n");
	    checkchild(p,fp);
            break;
	    
/////WHILE,IF
        case WHILE_AST:
            printf("WHILE\n");
            fprintf(fp,"$LOOP:\n");
	    checkchild(p,fp);
            fprintf(fp,"\
    j   $LOOP \n\
    nop\n");
            fprintf(fp,"\
$EXIT: \n\
    jr   $ra \n\
    nop");
            break;
        case IF_AST:
            printf("IF\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;

//////算術式
        case ASSIGN_AST:
            printf(" = \n");
            if(p->child->type != ADD_AST && p->child->brother->type != ADD_AST){
            fprintf(fp,"    li    register,");
            assign_num++;
            }
	    checkchild(p,fp);
           fprintf(fp,"\n");
            break;


        case ADD_AST:
            printf(" + \n");
	    checkchild(p,fp);
	    stack_num=stack_num-4;
            fprintf(fp,"\
    lw    $t0,%d($sp) \n\
    lw    $t1,%d(&sp) \n\
    add   $t0,$t0,$t1 \n\
    sw    $t0,%d($sp) \n",stack_num,stack_num+4,stack_num);
	    stck_num=stack_num+4;
            
	    
            break;
            
        case SUB_AST:
            printf(" - \n");
	    checkchild(p,fp);
            break;
        case MUL_AST:
            printf(" * \n");
	    checkchild(p,fp);
            break;
        case DIV_AST:
            printf(" / \n");
	    checkchild(p,fp);
            break;
        case EQ_AST:
            printf(" == \n");
            fprintf(fp,"    beq");
	    checkchild(p,fp);
            break;
        case LT_AST:
            printf(" < \n");
            fprintf(fp,"    bltz   $t%d,",t_reg); //$t%dの部分は以前に定義されている関数を用いる場合はその関数のレジスタに変える
	    checkchild(p,fp);
            fprintf(fp,",$EXIT\n");
            break;
        case GT_AST:
            printf(" > \n");
            fprintf(fp,"    bgtz");
	    checkchild(p,fp);
            break;
        default :
            fprintf(stderr,"print error\n");
        }

/*
        if(p->child != NULL){
            printf("(");
            printTree(p->child,fp);
        }else{
            printf(")");
        }
*/
    }
}







