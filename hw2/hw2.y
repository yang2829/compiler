/************************* calc.y ***********************/
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #define YYERROR_VERBOSE 1
    void yyerror (char const *);
    extern int yylex();
    extern int yyparse();
    // extern FILE *yyin;
%}
%union {
    float   floatNum;
    int     intNum;
    char    cstr[50];
}
%token <cstr> symWHILE symIF symELSE symID symNUM symINT symCHAR symVOID symCONST symJR symFOR
%token <cstr> symGEQ symLEQ symEQ symNEQ

%left '+' '-' '*' '/'
%left '<' '>' symGEQ symLEQ symEQ symNEQ
%right '='

%%
Program
    : 
    | Program EXDeclaration
    ;

EXDeclaration
    : FDefinition
    | VarDeclaration
    ;

FDefinition
    : DataType symID '(' ')' ';'
    | DataType symID '(' ')' Block
    | DataType symID '(' ')' JRBlock
    ;

JRBlock
    : '{' CompoundStatement JRStatement'}'
    ;

JRStatement
    : ';'
    | symJR Expression ';'
    ;

Block
    : '{' CompoundStatement '}'
    ;

CompoundStatement
    : Statement
    | CompoundStatement Statement
    ;

VarDeclaration
    : DataType DeclarationList ';'
    ;

DeclarationList
    : Declaration
    | DeclarationList ',' Declaration
    ;

Declaration
    : symID
    | AssignmentStatement
    ;

Statement
    : ';'
    | VarDeclaration
    | AssignmentStatement ';'
    | IfStatement
    | WhileStatement
    | FORStatement
    | Expression ';'
    ;

AssignmentStatement
    : symID '=' Expression
    ;

IfStatement
    : symIF '(' Condition ')' Block
    | symIF '(' Condition ')' Statement
    | symIF '(' Condition ')' Block ElseStatement
    | symIF '(' Condition ')' Statement ElseStatement
    ;

ElseStatement
    : symELSE Block
    | symELSE Statement
    ;

WhileStatement
    : symWHILE '(' Condition ')' Block
    | symWHILE '(' Condition ')' Statement
    ;

FORStatement
    : symFOR '(' Statement Condition ';' AssignmentStatement ')' Statement
    | symFOR '(' Statement Condition ';' AssignmentStatement ')' Block
    ;

Condition
    : Expression
    | Condition RelOP Expression
    ;

RelOP
    : '<'
    | '>'
    | symGEQ
    | symLEQ
    | symEQ
    | symNEQ
    ;

Expression
    : Term
    | '+' Term
    | '-' Term
    | Expression '+' Term
    | Expression '-' Term
    ;

Term
    : Factor
    | Term '*' Factor
    | Term '/' Factor
    ;

Factor
    : symID
    | symNUM
    | symID '(' ')'
    | '(' Expression ')'
    ;

DataType
    : symVOID
    | symINT
    | symCHAR
    | symCONST
    ;


%%

void yyerror(const char *s) {
    printf("%-10s\n", s);
}
int main() {
    return yyparse();
}
