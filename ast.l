%{
#include "AST.h"
#include "ast.tab.h"
%}
%%

"define"  return DEFINE;
"array"  return ARRAY;
"while"  return WHILE;
"if"  return IF;
"else"  return ELSE; 
"else if" return ELSEIF; //追加分 
";"  return SEMIC;
"["  return L_BRACKET;
"]"  return R_BRACKET;
"("  return L_PARAM;
")"  return R_PARAM;
"{"  return L_BRACE;
"}"  return R_BRACE;
"="  return ASSIGN;
"+"  return ADD;
"-"  return SUB;
"*"  return MUL;
"/"  return DIV;
"%"  return MOD;
"=="  return EQ;
"<"  return LT;
">"  return GT;
[0-9]+  {yylval.ival = atoi(yytext); return NUMBER;}
[a-zA-Z][a-zA-Z0-9]*  {yylval.sp=strdup(yytext); return IDENT;}
[ \t\n]  ; /*ignore space*/
%%
