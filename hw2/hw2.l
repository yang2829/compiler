%{
	#include <stdlib.h>
	#include <string.h>
	#include "hw2.tab.h"                     /*bison所建*/
    int line = 1;
%}
%x COMMENT

%%
\/\/              {printf("line = %-2d text = %-10s\n", line, yytext); BEGIN COMMENT; }
"#"               {printf("line = %-2d text = %-10s\n", line, yytext); BEGIN COMMENT; }
<COMMENT>[^\n]*   {printf("line = %-2d text = %-10s\n", line, yytext); }
<COMMENT>\n       {line++; BEGIN 0;}
"while"   {printf("line = %-2d text = %-10s\n", line, yytext); return symWHILE;}
"for"     {printf("line = %-2d text = %-10s\n", line, yytext); return symFOR;}
"if"      {printf("line = %-2d text = %-10s\n", line, yytext); return symIF;}
"else"    {printf("line = %-2d text = %-10s\n", line, yytext); return symELSE;}
"VOID"    {printf("line = %-2d text = %-10s\n", line, yytext); return symVOID;}
"int"     {printf("line = %-2d text = %-10s\n", line, yytext); return symINT;}
"char"    {printf("line = %-2d text = %-10s\n", line, yytext); return symCHAR;}
"const"   {printf("line = %-2d text = %-10s\n", line, yytext); return symCONST;}
"return"  {printf("line = %-2d text = %-10s\n", line, yytext); return symJR;}
[a-zA-Z][a-zA-Z0-9_]*     {printf("line = %-2d text = %-10s\n", line, yytext); return symID;}
[0-9]+    {printf("line = %-2d text = %-10s\n", line, yytext); return symNUM;}
"("       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
")"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"{"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"}"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"+"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"-"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"*"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"/"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"<"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
">"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"="       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
">="      {printf("line = %-2d text = %-10s\n", line, yytext); return symGEQ;}
"<="      {printf("line = %-2d text = %-10s\n", line, yytext); return symLEQ;}
"=="      {printf("line = %-2d text = %-10s\n", line, yytext); return symEQ;}
"!="      {printf("line = %-2d text = %-10s\n", line, yytext); return symNEQ;}
";"       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
"."       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
","       {printf("line = %-2d text = %-10s\n", line, yytext); return yytext[0];}
<<EOF>>   return -1;
[ \t]+    ;
[\n]      {line++;}
.   {;}         /*其他字元*/
%%

int yywrap()
{
	return 1;                                 /*檔尾時結束*/
}
