#include<stdio.h>
#include<stdlib.h> 
#include <string.h>
#include "AST.h"

int stack_num=0;
int loop_num=0;
int loop_rec=0;

int if_num=0;
int else_count=0;
int if_end_num=0;

int def_num=0; //defineの時に一度だけ.dataを宣言


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
    p2->brother = p3;
    p->brother=NULL;
    return p;
}


Node *build_4_child(NType t,Node *p1,Node *p2,Node *p3,Node *p4){
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL){
        yyerror("out of memory");
    }
    p->type=t;
    p->child = p1;
    p1->brother = p2;
    p2->brother = p3;
    p3->brother = p4;
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
    nop             # (delay slot) \n\
\n\
\t.text      0x00001000\n\
main:\n";
    fprintf(fp,"%s\n",str);
    fprintf(fp,"\taddi  $sp, $sp, -32\n\tsw  $ra, 28($sp)\n");
}

void checkNode(Node *p,FILE *text_fp,FILE *data_fp){
  if(p->child != NULL){
    printTree(p->child,text_fp,data_fp);
  }

  if(p->brother != NULL){
    printTree(p->brother,text_fp,data_fp);
  } 
}

void printTree(Node *p,FILE *text_fp,FILE *data_fp){
  if(p != NULL){
        switch(p->type){
        case Pro_AST:
            make_init(text_fp);
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,"end:\n\tlw  $ra, 28($sp)\n\taddi  $sp, $sp, 32\n");
            fprintf(text_fp,"$EXIT: \n \tjr   $ra \t;in Pro_AST \n \tnop \n");
            break;
        case Decl_AST:  
            if(def_num==0){
                fprintf(data_fp,"\t.data   0x10004000 \t;in Decl_AST \n");
                def_num++;
            }
            checkNode(p,text_fp,data_fp);
            break;
        case Decl_stmt_AST:
            checkNode(p,text_fp,data_fp);
            break;
        case Stmts_AST:
            checkNode(p,text_fp,data_fp);
            break;
        case Stmt_AST:
            checkNode(p,text_fp,data_fp);
            break;
            
////////NUM,IDENT/////
        case IDENT_AST:
            fprintf(text_fp,"\tli  $t0, %s\n",p->varName);
            fprintf(text_fp,"\tlw  $v0, 0($t0)\n\tnop\n");
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
        case NUM_AST:
            fprintf(text_fp,"\tli  $v0, %d\n",p->value);
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
            
        case Expr_AST:
            checkNode(p,text_fp,data_fp);            
            break;
        case Term_AST:
            checkNode(p,text_fp,data_fp);
            break;
        case Factor_AST:
            checkNode(p,text_fp,data_fp);
            break;
            
//////////DEFINE,ARRAY
        case DEFINE_AST:
            fprintf(data_fp,"%s:\t.word  0x0000 \t;in DEFINE_AST \n",p->child->varName);
            checkNode(p->child,text_fp,data_fp);
            printTree(p->brother,text_fp,data_fp);   
            break;
        case ARRAY_AST:
            printf("ARRAY\n");
            checkNode(p,text_fp,data_fp);
            break;
	    
/////WHILE,IF
        case WHILE_AST:

            fprintf(text_fp,"$L%d_%d:\n",loop_rec,loop_num);
            checkNode(p,text_fp,data_fp);

            if(p->child->type != EQ_AST){
                fprintf(text_fp,"\tbeq   $v0,$zero,$L%d_%d \t;in WHILE_AST \n",loop_rec,loop_num+1);
            }
            if(p->child->type == EQ_AST){
                fprintf(text_fp,"\tbne   $t0,$t1,$L%d_%d \t;in WHILE_AST \n",loop_rec,loop_num+1);
		stack_num=0;
            }
            loop_rec++;
            printTree(p->child->brother,text_fp,data_fp);
            loop_rec--;
            fprintf(text_fp,"\tj $L%d_%d \t;in WHILE_AST \n",loop_rec,loop_num);
            fprintf(text_fp,"\tnop \t;in WHILE_AST \n");
            fprintf(text_fp,"$L%d_%d: \t;in WHILE_AST \n",loop_rec,loop_num+1);
            if(loop_rec==0) loop_num=loop_num+2;
            break;

	    
        case IF_AST:
            //fprintf(text_fp,"!!!!!!!!!!!!!IF\n");
            printTree(p->child,text_fp,data_fp);
      
            if(p->child->type != EQ_AST){
                fprintf(text_fp,"\tbeq   $v0,$zero,$D%d \t;in IF_AST \n",if_num);
                else_count++;
            }
            if(p->child->type == EQ_AST){
                fprintf(text_fp,"\tbne   $t0,$t1,$D%d \t;in IF_AST \n",if_num);
                else_count++;
            }
            printTree(p->child->brother,text_fp,data_fp);
	    
            if(p->child->brother->brother!=NULL){   //else if ,elseが存在する時
                printTree(p->child->brother->brother,text_fp,data_fp); 
            }
            fprintf(text_fp,"\tnop \t;in IF_AST\n");
            fprintf(text_fp,"$D0_%d: \t;in IF_AST\n",if_end_num);
            
            fprintf(text_fp,"$D%d:\n",if_num); //if文の終点のラベル
            if_num++;
            if_end_num++;
            break;

        case ELSEIF_AST:
            //fprintf(text_fp,"!!!!!!!!!!!!!ELSEIF\n");
            fprintf(text_fp,"\tj   $D0_%d \t;ELSEIF_AST \n ",if_end_num);
            fprintf(text_fp,"\tnop \n");
            fprintf(text_fp,"$D%d: \t;in ELSEIF_AST \n",if_num);
          
           
            printTree(p->child,text_fp,data_fp);

            if(p->child->type != EQ_AST){
                if(p->brother !=NULL || else_count!=0){
                    fprintf(text_fp,"\tbeq   $v0,$zero,$D%d \t;in ELSEIF_AST \n ",if_num+1);
                    else_count++;
                } else {
                    fprintf(text_fp,"\tbeq   $v0,$zero,$D0 \t;in ELSEIF_AST \n ");
                }
            }
            if(p->child->type == EQ_AST){
                if(p->brother !=NULL || else_count!=0){
                fprintf(text_fp,"\tbne   $t0,$t1,$D%d \t;in ELSEIF_AST\n ",if_num+1);
                else_count++;
                } else{
                    fprintf(text_fp,"\tbne   $t0,$t1,$D0_%d \t;in ELSEIF_AST\n ",if_end_num);
                }
            }    
            if_num++;
            printTree(p->child->brother,text_fp,data_fp);
/*
            if(p->child->brother->brother!=NULL){  //elseifが複数個ある可能性
                printTree(p->child->brother->brother,text_fp,data_fp);
            }
*/
            if(p->brother!=NULL){
                printTree(p->brother,text_fp,data_fp);
            }
            break;

        case ELSE_AST:
            fprintf(text_fp,"\tj   $D0_%d \t;in ELSE_AST \n",if_end_num);
            fprintf(text_fp,"\tnop \t;in ELSE_AST \n");
            fprintf(text_fp,"$D%d: \t;in ELSE_AST \n",if_num);
            checkNode(p,text_fp,data_fp);
            if_num++;
            break;            


//////算術式
        case ASSIGN_AST:  
            checkNode(p->child,text_fp,data_fp);
            fprintf(text_fp,"\tli   $t0, %s\n",p->child->varName);
            fprintf(text_fp,"\tsw   $v0, 0($t0)\n");
            stack_num = 0;
            break;
	    
        case ADD_AST:
            fprintf(text_fp,";!!!ADD\n");
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,";ADD!!!\n");
            fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
            fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
            fprintf(text_fp,"\tadd  $v0, $t0, $t1\n\tnop\n");
            stack_num=stack_num-2;
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
            
        case SUB_AST:
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
            fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
            fprintf(text_fp,"\tsub  $v0, $t0, $t1\n\tnop\n");
            stack_num=stack_num-2;
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
	  
        case MUL_AST:
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
            fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
            fprintf(text_fp,"\tmult  $t0, $t1\n");
            fprintf(text_fp,"\tmflo  $v0\n");
            stack_num=stack_num-2;
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
            
        case DIV_AST:
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
            fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
            fprintf(text_fp,"\tdiv  $t0, $t1\n");
            fprintf(text_fp,"\tmflo  $v0\n");
            stack_num=stack_num-2;
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;
            
        case MOD_AST:
            checkNode(p,text_fp,data_fp);
            fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
            fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
            fprintf(text_fp,"\tdiv  $t0, $t1\n");
            fprintf(text_fp,"\tmfhi  $v0\n");
            stack_num=stack_num-2;
            fprintf(text_fp,"\tsw  $v0, %d($sp)\n",stack_num*4);
            stack_num++;
            break;


//比較
        case EQ_AST:
	  checkNode(p->child,text_fp,data_fp);
	  fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
	  fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
	  
	  break;

        case LT_AST:
          fprintf(text_fp,";LT_AST1\n");
          checkNode(p->child,text_fp,data_fp);
          fprintf(text_fp,";LT_AST2\n");
          fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
          fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
          fprintf(text_fp,"\tslt   $v0,$t0,$t1\n");
          stack_num=0;
          break;

        case GT_AST:
	  checkNode(p->child,text_fp,data_fp);
	  fprintf(text_fp,"\tlw  $t0, %d($sp)\n",(stack_num-2)*4);
	  fprintf(text_fp,"\tlw  $t1, %d($sp)\n\tnop\n",(stack_num-1)*4);
	  fprintf(text_fp,"\tslt   $v0,$t1,$t0\n");
	  stack_num=0;
	  break;

/*
case LET_AST:
先にイコールの判定をしてOKならwhileの処理にジャンプ，つぎに＜の判定をしてOKならwhileの処理にジャンプ
whileにLET_ASTようの処理を追加し，判定前に$v0に1を入れておき，判定が正解なら1がはいり，不正解なら0がはいるので，whileでその判定を行う．

*/
        default :
            fprintf(stderr,"print error\n");
        }
    }
}







