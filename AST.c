#include<stdio.h>
#include<stdlib.h> 
#include <string.h>
#include "AST.h"

int Decl_num=0; // Decl_AStの時，一度だけ .data 0x10004000を表示するため
int def_n=0; // defineの時に，identの処理を行わない
int ast_n = 0;
int main_num=0;
int loop_num=1;

int t_reg=0; //レジスタ$t　の引数
int a=0;
int b=0;   //算術式の際適切なt_regを指定

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

void make_init(FILE *fp){
    char str[]="\
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
    nop             # (delay slot) \n";
    fprintf(fp,"%s\n",str);
}

void checkNode(Node *p,FILE *fp){
  if(p->child != NULL){
    printTree(p->child,fp);
  }

  if(p->brother != NULL){
    printTree(p->brother,fp);
  } 
}

void printTree(Node *p,FILE *fp){
  if(p != NULL){
        switch(p->type){
        case Pro_AST:
            make_init(fp);
            printf("PROGRAM\n");
            checkNode(p,fp);
            fprintf(fp,"$EXIT: \n \tjr   $ra \n \tnop");
            break;
        case Decl_AST:
            printf("DECLARATION\n");
            if(Decl_num==0){
                fprintf(fp,"\t.data   0x10004000\n");
                Decl_num++;
            }
            checkNode(p,fp);
            break;
        case Decl_stmt_AST:
            printf("Decl_statement\n");
            checkNode(p,fp);
            break;
        case Stmts_AST:
            printf("statements\n");
            if(main_num==0){
            fprintf(fp,"\t.text 	0x00001000\n");
            fprintf(fp,"main:\n");
            main_num++;
            }
            checkNode(p,fp);
            break;
        case Stmt_AST:
            printf("statement\n");
            checkNode(p,fp);
            break;
            
////////NUM,IDENT/////
        case IDENT_AST:
            printf("%s\t",p->varName);
            if(def_n == 0){
                fprintf(fp,"\tli   $t%d,%s\n",t_reg,p->varName);
                if(ast_n != 0){
                    fprintf(fp,"\tlw   $t%d,0($t%d)\n",t_reg+1,t_reg);
                    fprintf(fp,"\tnop\n");
                    t_reg++;
                    if(a==0){
                        a=t_reg;
                    }
                }
                t_reg++;
            }
            checkNode(p,fp);
            break;
        case NUM_AST:
            printf("%d\t",p->value);
            fprintf(fp,"\tli   $t%d,%d\n",t_reg,p->value);
            if(b==0){
                b=t_reg;
            }
            t_reg++;
            checkNode(p,fp);
            break;

        case Expr_AST:
            printf("expression\n");
            checkNode(p,fp);            break;
        case Term_AST:
            printf("term\n");
            checkNode(p,fp);
            break;
        case Factor_AST:
            printf("factor\n");
            checkNode(p,fp);
            break;
            
//////////DEFINE,ARRAY
        case DEFINE_AST:
            printf("DEFINE\n");
            fprintf(fp,"%s:\t.word  0x0000\n",p->child->varName);
            def_n=1;
            checkNode(p,fp);
            def_n=0;
            break;
        case ARRAY_AST:
            printf("ARRAY\n");
            checkNode(p,fp);
            break;
	    
/////WHILE,IF
        case WHILE_AST:
            printf("WHILE\n");

            fprintf(fp,"$L%d:\n",loop_num);
            checkNode(p,fp);
            fprintf(fp,"\tbeq   $t%d,$zero,$L%d\n",t_reg,loop_num+1);
            t_reg=0;
            printTree(p->child->brother,fp);
            fprintf(fp,"\tj $L%d\n",loop_num);
            fprintf(fp,"\tnop\n");
            fprintf(fp,"$L2:\n");
            a=0;
            b=0;
            break;
        case IF_AST:
            printf("IF\n");
            checkNode(p,fp);
            break;

//////算術式
        case ASSIGN_AST:
            printf(" = \n");
            checkNode(p,fp);
            fprintf(fp,"\tsw   $t%d,0($t0)\n",t_reg-1);
            t_reg=0;
            a=0;
            b=0;
            break;

        case ADD_AST:
            ast_n=1; //identでlwの処理を行うかどうか
            printf(" + \n");
            checkNode(p,fp);

            if((a>b||a==0) && b!=0){
                a=b;
            }
            fprintf(fp,"\tadd   $t%d,$t%d,$t%d\n",t_reg,a,t_reg-1);
            ast_n=0; //初期化
            a=0;
            t_reg++;
            break;
           
        case SUB_AST:
            printf(" - \n");
            checkNode(p,fp);
            break;
        case MUL_AST:
            printf(" * \n");
            ast_n=1; //identでlwの処理を行うかどうか
            checkNode(p,fp);

            if((a>b||a==0) && b!=0){
                a=b;
            }

            fprintf(fp,"\tmult   $t%d,$t%d\n",t_reg-1,a);
            fprintf(fp,"\tmflo   $t%d\n",t_reg);
            ast_n=0; //初期化
            a=0;
            t_reg++;
            break;
        case DIV_AST:
            printf(" / \n");
            checkNode(p,fp);
            break;
        case EQ_AST:
            printf(" == \n");
            checkNode(p,fp);
           fprintf(fp,"    beq");
            break;
        case LT_AST:
            ast_n=1;   //identでlwの処理がいる場合とで場合分け
            printf(" < \n");
            printTree(p->child,fp);
            
            if((a>b||a==0) && b!=0){
                a=b;
            }
            fprintf(fp,"\tslt   $t%d,$t%d,$t%d\n",t_reg,a,t_reg-1);
            //fprintf(fp,"\tslt   $t%d,$t%d,$t%d\n",t_reg,t_reg-2,t_reg-1);
            //fprintf(fp,"\tbeq   $t%d,$zero,$L%d\n",t_reg,loop_num+1);
            //t_reg=0;
           
            //printTree(p->brother,fp);
            a=0;
            ast_n=0;     //identでlwの処理がいる場合とで場合分け    
            break;
        case GT_AST:
            printf(" > \n");
            fprintf(fp,"    ");
            checkNode(p,fp);
            break;
        default :
            fprintf(stderr,"print error\n");
        }
    }
}







