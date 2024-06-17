/*********************** parser.c **********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 璹繷郎
  #include "scanner.h"
  #include "resword.h"
  #include "err.h"
  #include "followsym.h"

// 璹盽计
  #define IDLEN      36
  #define BUFSIZE   512

// 璹ㄧΑ
  void Identifier();
  void Number();
  void IdentifierList();
  void Factor();
  void Term();
  void Expression();
  void Condition();
  void WriteStatement();
  void ReadStatement();
  void WhileStatement();
  void IfStatement();
  void CompoundStatement();
  void CallStatement();
  void AssignmentStatement();
  void Statement();
  void ProcDeclaration();
  void VarDeclaration();
  void ConstDeclaration();
  void Block();
  void ProgramHead();
  void Program();

// 俱砰跑计
  struct symbolTag *token;
  int errorCount = 0;
  char str[BUFSIZE];
/*
** Error()
*/
  void Error(int n)
  {
    int j;
    printf("****");
    for (j=0; j<token->right; j++)
      printf(" ");
    sprintf(str, "^%d  %s\n",n, errmsgs[n]);
    printf(str);
    if (n != 23)
      errorCount++;
  }
/*
** skip()
*/
  void skip(char follows[], int n)
  {
    if (follows[token->sym]==0)
    {
      Error(n);
      while (follows[token->sym]==0) {
        token = nextToken();
      }
    }
  }
/*
** 粂猭砏玥#1 <Program>
*/
  void Program()
  {
    ProgramHead();
    Block();
    if (token->sym != symPERIOD) Error(0);
  }
/*
** 粂猭砏玥#2 <ProgramHead>
*/
  void ProgramHead()
  {
    if (strcmp(token->value,"PROGRAM")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        token = nextToken();
        if (token->sym == symSEMI)
          token = nextToken();
        else
          Error(3);
      }
      else
        Error(2);
    }
    else
      Error(1);
  }
/*
** 粂猭砏玥#3 <Block>
*/
  void Block()
  {
    if (strcmp(token->value, "CONST")==0)
      ConstDeclaration();
    if (strcmp(token->value, "VAR")==0)
      VarDeclaration();
    if (strcmp(token->value, "PROCEDURE")==0)
      ProcDeclaration();
    CompoundStatement();
  }
/*
** 粂猭砏玥#4 <ConstDeclaration>
*/
  void ConstDeclaration()
  {
    if (strcmp(token->value, "CONST")==0)
    {
      token = nextToken();
      Identifier();
      if (token->sym == symEQ)
      {
        token = nextToken();
        if (token->sym == symSTRING)
          token = nextToken();
        else
        {
          Error(27);
          skip(statement, 23);
        }
        while (token->sym == symCOMMA)
        {
          token = nextToken();
          Identifier();
          if (token->sym == symEQ)
          {
            token = nextToken();
            if (token->sym == symSTRING)
              token = nextToken();
            else
            {
              Error(27);
              skip(statement, 23);
            }
          }
          else
          {
            Error(5);
            skip(statement, 23);
          }
        }
        if (token->sym == symSEMI)
          token = nextToken();
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(5);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(4);
    }
  }
/*
** 粂猭砏玥#5 <VarDeclaration>
*/
  void VarDeclaration()
  {
    if (strcmp(token->value, "VAR")==0)
    {
      token = nextToken();
      IdentifierList();
      if (token->sym == symSEMI)
        token = nextToken();
      else
      {
        Error(6);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(7);
    }
  }
/*
** 粂猭砏玥#6 <ProcDeclaration>
*/
  void ProcDeclaration()
  {
    while (strcmp(token->value, "PROCEDURE")==0)
    {
      token = nextToken();
      Identifier();
      if (token->sym == symSEMI)
      {
        token = nextToken();
        Block();
        if (token->sym == symSEMI)
          token = nextToken();
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(6);
      }
    }
  }
/*
** 粂猭砏玥#7 <Statement>
*/
  void Statement()
  {
    if (isResword(token->value) != -1)
    {
      if (strcmp(token->value,"IF")==0)
        IfStatement();
      else if (strcmp(token->value,"BEGIN")==0)
        CompoundStatement();
      else if (strcmp(token->value,"WHILE")==0)
        WhileStatement();
      else if (strcmp(token->value,"READ")==0)
        ReadStatement();
      else if (strcmp(token->value,"WRITE")==0)
        WriteStatement();
      else if (strcmp(token->value,"CALL")==0)
        CallStatement();
    }
    else if (token->sym == symIDENTIFIER) {
      AssignmentStatement();
    }
    else
      skip(statement, 23);
  }
/*
** 粂猭砏玥#8 <AssignmentStatement>
*/
  void AssignmentStatement()
  {
    Identifier();
    if (token->sym == symBECOMES)
    {
      token = nextToken();
      Expression();
    }
    else
    {
      Error(8);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#9 <CallStatement>
*/
  void CallStatement()
  {
    if (strcmp(token->value, "CALL")==0)
    {
      token = nextToken();
      Identifier();
    }
    else
    {
      Error(9);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#10 <CompoundStatement>
*/
  void CompoundStatement()
  {
    if (strcmp(token->value,"BEGIN")==0)
    {
      token = nextToken();
      Statement();
      while (token->sym == symSEMI)
      {
        token = nextToken();
        Statement();
      }
      if (strcmp(token->value,"END")==0)
        token = nextToken();
      else
      {
        Error(11);
        skip(statement, 23);
      }
    }
    else
    {
      Error(10);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#11 <IfStatement>
*/
  void IfStatement()
  {
    if (strcmp(token->value,"IF")==0)
    {
      token = nextToken();
      Condition();
      if (strcmp(token->value, "THEN")==0)
      {
        token = nextToken();
        Statement();
        if (strcmp(token->value,"ELSE")==0)
        {
          token = nextToken();
          Statement();
        }
      }
      else
      {
        Error(13);
        skip(statement, 23);
      }
    }
    else
    {
      Error(12);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#12 <WhileStatement>
*/
  void WhileStatement()
  {
    if (strcmp(token->value,"WHILE")==0)
    {
      token = nextToken();
      Condition();
      if (strcmp(token->value,"DO")==0)
      {
        token = nextToken();
        Statement();
      }
      else
      {
        Error(15);
        skip(statement, 23);
      }
    }
    else
    {
      Error(14);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#13 <ReadStatement>
*/
  void ReadStatement()
  {
    if (strcmp(token->value,"READ")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        IdentifierList();
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(16);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#14 <WriteStatement>
*/
  void WriteStatement()
  {
    if (strcmp(token->value, "WRITE")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        IdentifierList();
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(19);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#15 <IdentifierList>
*/
  void IdentifierList()
  {
    Identifier();
    while (token->sym == symCOMMA)
    {
      token = nextToken();
      Identifier();
    }
  }
/*
** 粂猭砏玥#16 <Condition>
*/
  void Condition()
  {
    Expression();
    if (token->sym == symLESS ||
        token->sym == symLEQ ||
        token->sym == symEQ ||
        token->sym == symNEQ ||
        token->sym == symGREATER ||
        token->sym == symGEQ)
    {
      token = nextToken();
      Expression();
    }
    else
    {
      Error(20);
      skip(statement, 23);
    }
  }
/*
** 粂猭砏玥#17 <Expression>
*/
  void Expression()
  {
    if (token->sym == symPLUS ||
        token->sym == symMINUS)
    {
      token = nextToken();
    }
    Term();
    while (token->sym == symPLUS ||
           token->sym == symMINUS)
    {
      token = nextToken();
      Term();
    }
  }
/*
** 粂猭砏玥#18 <Term>
*/
  void Term()
  {
    Factor();
    if (token->sym == 1) {
      Error(27);
      token = nextToken();
      Term();
    }
    while (token->sym == symMUL ||
           token->sym == symDIV ||
           strcmp(token->value, "MOD")==0 ||
           strcmp(token->value, "DIV")==0)
    {
      token = nextToken();
      Factor();
    }
  }
/*
** 粂猭砏玥#19 <Factor>
*/
  void Factor()
  {
    if (token->sym == symIDENTIFIER)
      Identifier();
    else if (token->sym == symNUMBER)
      Number();
    else if (token->sym == symLPAREN)
    {
      token = nextToken();
      Expression();
      if (token->sym == symRPAREN)
        token = nextToken();
      else
      {
        Error(18);
        skip(factor, 23);
      }
    }
  }
/*
** 粂猭砏玥#19 <Factor>
*/
  void Identifier()
  {
    if (token->sym == symIDENTIFIER)
      token = nextToken();
    else
      Error(21);
  }
/*
** 计才癘矪瞶
*/
  void Number()
  {
    if (token->sym == symNUMBER)
      token = nextToken();
    else
      Error(22);
  }
/*
************************** 祘Α **********************
*/
  int main(int argc, char *argv[])
  {
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    Program();
    fclose(f);
    printf("\n  Plone compile completed. "
      "\n  Error count : %d\n", errorCount);
    return 0;
  }
