#include<stdio.h>
#include<stdlib.h> 
#include <string.h>
#include "AST.h"

int Decl_num=0; // Decl_AStの時，一度だけ .data 0x10004000を表示するため

int struct_num=0;  //　記号表の配列の引数
int assign_num=0; 
int n=0; //ソケットのアドレス指定

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

void printTree(Node *p,FILE *fp){
int t_reg=0; //レジスタ$tの引数
    if(p != NULL){
        switch(p->type){
        case Pro_AST:
            printf("PROGRAM\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case Decl_AST:
            printf("DECLARATION\n");
            if(Decl_num==0){
                fprintf(fp,"    .data   0x00004000\n");
                Decl_num++;
            }
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case Decl_stmt_AST:
            printf("Decl_statement\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
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
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
            
////////NUM,IDENT/////
        case IDENT_AST:
            printf("%s\t",p->varName);
            //fprintf(fp,"%s",p->varName);
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case NUM_AST:
            printf("%d\t",p->value);
            fprintf(fp,"%d",p->value);

            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;


        case Expr_AST:
            printf("expression\n");

            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case Term_AST:
            printf("term\n");

            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case Factor_AST:
            printf("factor\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
            
//////////DEFINE,ARRAY
        case DEFINE_AST:
            printf("DEFINE\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case ARRAY_AST:
            printf("ARRAY\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
/////WHILE,IF
        case WHILE_AST:
            printf("WHILE\n");
            fprintf(fp,"$LOOP:\n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
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
            fprintf(fp,"    li    $a%d,",assign_num);
            assign_num++;
            }
           if(p->child != NULL){
                printTree(p->child,fp);
            }
           fprintf(fp,"\n");
            break;


        case ADD_AST:
            printf(" + \n");
            fprintf(fp,"\
    lw    $t0,0(第1要素のアドレス) \n\
    lw    $t1,0(第2要素のアドレス) \n\
    add   $t3,$t0,$t1 \n\
    sw    $t3,%d($sp) \\次の行の1はi=i+1の1   \n",n);   //レジスタ$tの部分を記号表作成後レジスタに修正
            n=n+4;
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
            
        case SUB_AST:
            printf(" - \n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case MUL_AST:
            printf(" * \n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case DIV_AST:
            printf(" / \n");
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            break;
        case EQ_AST:
            printf(" == \n");
            fprintf(fp,"    beq");
            break;
        case LT_AST:
            printf(" < \n");
            fprintf(fp,"    bltz   $t%d,",t_reg); //$t%dの部分は以前に定義されている関数を用いる場合はその関数のレジスタに変える
            if(p->child != NULL){
                printTree(p->child,fp);
            }
            fprintf(fp,",$EXIT\n");
            break;
        case GT_AST:
            printf(" > \n");
            fprintf(fp,"    bgtz");
            break;
        default :
            fprintf(stderr,"print error\n");
        }

/*
        if(p->varName != NULL){
            printf("%s\t", p->varName);
            strcpy(simTable[struct_num].name,p->varName);
            struct_num++;
        }
        if(p->value){
            printf("%d\t",p->value);
        }
*/
/*
        if(p->child != NULL){
            printf("(");
            printTree(p->child,fp);
        }else{
            printf(")");
        }
*/
        if(p->brother != NULL){
            printf("\n\t(");
            printTree(p->brother,fp);
        }
    }
}




/*
void printType(Node *p,FILE *fp){
    NType t = p->type;
    switch(t){
    case Pro_AST:
        printf("PROGRAM\n");
        break;
    case Decl_AST:
        printf("DECLARATION\n");
        if(Decl_num==0){
        fprintf(fp,"    .data   0x00004000\n");
        Decl_num++;
        }
        break;
    case Decl_stmt_AST:
        printf("Decl_statement\n");
        break;
    case Stmts_AST:
        printf("statements\n");
        break;
    case Stmt_AST:
        printf("statement\n");
        break;

    case IDENT_AST:
        printf("%s\t",p->varName);
        break;
    case NUM_AST:
        printf("%d\t",p->value);
        fprintf(fp,"%d\n",p->value);
        break;
    case Expr_AST:
        printf("expression\n");
        break;
    case Term_AST:
        printf("term\n");
        break;
    case Factor_AST:
        printf("factor\n");
        break;

    case DEFINE_AST:
        printf("DEFINE\n");
        break;
    case ARRAY_AST:
        printf("ARRAY\n");
        break;
    case WHILE_AST:
        printf("WHILE\n");
        fprintf(fp,"$LOOP:\n");
        //printTree(p->child,fp);
        break;
    case IF_AST:
        printf("IF\n");
        break;
    case ASSIGN_AST:
        printf(" = \n");
        fprintf(fp,"    li    $a%d,",assign_num);
        assign_num++;
        break;
    case ADD_AST:
        printf(" + \n");
        break;
    case SUB_AST:
        printf(" - \n");
        break;
    case MUL_AST:
        printf(" * \n");
        break;
    case DIV_AST:
        printf(" / \n");
        break;
    case EQ_AST:
        printf(" == \n");
        fprintf(fp,"    beq");
        break;
    case LT_AST:
        printf(" < \n");
        fprintf(fp,"    bltz   $t%d,",while_num);
        fprintf(fp,",$EXIT");
        break;
    case GT_AST:
        printf(" > \n");
        fprintf(fp,"    bgtz");
        break;
    default :
        fprintf(stderr,"print error\n");
    }
}
void printTree(Node *p,FILE *fp){
    if(p != NULL){
        printType(p,fp);
        if(p->varName != NULL){
            printf("%s\t", p->varName);
            strcpy(simTable[struct_num].name,p->varName);
            struct_num++;
        }
        if(p->value){
            printf("%d\t",p->value);
        }

        if(p->child != NULL){
            printf("(");
            printTree(p->child,fp);
        }else{
            printf(")");
        }

        if(p->brother != NULL){
            printf("\n\t(");
            printTree(p->brother,fp);
        }
    }
}
*/


