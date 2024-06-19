/************************* plone.c ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 自訂標頭黨
#include "scanner.h"
#include "resword.h"
#include "err.h"
#include "followsym.h"
#include "IdProclist.h"
// 自訂常數
#define IDLEN      100
#define BUFSIZE   512
#define PROCDEPTH  36
// function declaration
void Factor();
void Term();
void Expression();
void Condition();
void WriteStatement();
void ReadStatement();
void WhileStatement();
void IfElseStatement();
void AssignmentStatement();
void Statement();
void ProcDeclaration();
void VarDeclaration();
void Block();
void Program();
// still not complete
// void ForStatement();
// global var
struct proclist pl_head = {
    .name = "",
    .next = NULL
};
struct proclist* pl_tail = &pl_head;
struct symbolTag *token;
struct symbolTag *temp_token = NULL;
int errorCount = 0;
FILE *outfile;
int labelCount = 0;
int level = 0;
char procname[IDLEN] = "";
char id[IDLEN];
char buf[BUFSIZE];

void Error(int n)
{
  int j;
  printf("****");
  for (j=0; j<=token->right; j++) printf(" ");
  if (n<0) {
    printf("this error message not complete\n");
  }
  else
    printf("^%d  %s\n",n, errmsgs[n]);
  errorCount++;
}

void skip(char follows[], int n)
{
  if (follows[token->sym]==0)
  {
    Error(n);
    while (follows[token->sym]==0)
      token = nextToken();
  }
}
// #1 <Program>
void Program()
{
  while (token->sym != symEOF) {
    if (token->sym == symVOID) {
      ProcDeclaration();
    }
    else if (token->sym == symINT || token->sym == symCHARA) {
      VarDeclaration();
    }
    else {
      printf("sym: %d\n", token->sym);
      Error(27);
      token = nextToken();
    }
  }
}
// procd
void ProcDeclaration()
{
  token = nextToken();
  if (token->sym == symIDENTIFIER) {
    char temp_value[IDLEN];
    strcpy(temp_value, token->value);
    token = nextToken();
    if (token->sym == symLPAREN) {
      token = nextToken();
      if (token->sym == symRPAREN) {
        token = nextToken();
        int temp_i = procexist(&pl_head, temp_value, 0);
        if (temp_i == 2) {
          // needdo error (function already been used)
          Error(-1);
        }
        if (token->sym == symSEMI) {
          token = nextToken();
          if (temp_i == 0) {
            pl_tail->next = newProc(temp_value, 0);
            pl_tail = pl_tail->next;
          }
        } 
        else if (token->sym == symLBRAKET) {
          if (temp_i == 0) {
            pl_tail->next = newProc(temp_value, 1);
            pl_tail = pl_tail->next;
          } else {
            procexist(&pl_head, temp_value, 1);
          }
          sprintf(buf,"_%s:\n", temp_value);
          fprintf(outfile,buf);
          strcpy(procname, temp_value);
          Block();
          if (strcmp(procname, "main") == 0) {
            fprintf(outfile, "\tMOV\tAX, 4C00H\n"
                             "\tINT\t21H\n");
          }
          else {
            sprintf(buf, "\tRET\n");
            fprintf(outfile, buf);
          }
          strcpy(procname, "");
        } else {
          // needdo not ; or {
          Error(-1);
        }
      }
      else {
        // needdo not )
        Error(-1);
      }
    }
    else {
      // needdo not (
      Error(-1);
    }
  }
  else {
    // needdo not id
    Error(-1);
  }
}
// #vard
void VarDeclaration()
{
  if (token->sym == symINT) {
    token = nextToken();
    if (token->sym == symIDENTIFIER && idexist(token->value) == NULL) {
      char temp_value[IDLEN];
      strcpy(temp_value, token->value);
      idpush(newid(temp_value, symINT, level));
      token = nextToken();
      if (token->sym == symASSIGN) {
        token = nextToken();
        if (token->sym == symNUMBER) {
          sprintf(buf, "%s_%s\tDW\t%s\n",
          procname, temp_value, token->value);
          fprintf(outfile, buf);
          token = nextToken();
        }
        else {
          // needdo not a number
          Error(-1);
        }
      }
      else {
        sprintf(buf, "%s_%s\tDW\t0\n",
        procname, temp_value);
        fprintf(outfile, buf);
      }
      while (token->sym == symCOMMA) {
        if (token->sym == symIDENTIFIER && idexist(token->value) == NULL) {
          strcpy(temp_value, token->value);
          idpush(newid(temp_value, symINT, level));
          token = nextToken();
          if (token->sym == symASSIGN) {
            token = nextToken();
            if (token->sym == symNUMBER) {
              sprintf(buf, "%s_%s\tDW\t%s\n",
              procname, temp_value, token->value);
              fprintf(outfile, buf);
              token = nextToken();
            }
            else {
              // needdo not a number
              Error(-1);
            }
          }
          else {
            sprintf(buf, "%s_%s\tDW\t0\n",
            procname, temp_value);
            fprintf(outfile, buf);
          }
        }
        else {
          // needdo not id
          Error(-1);
        }
      }
      if (token->sym == symSEMI) {
        token = nextToken();
      }
      else {
        // needdo slip if not ;
        Error(6);
      }
    }
    else {
      // needdo not id
      Error(-1);
    }
  }
  else if (token->sym == symCHARA) {
    token = nextToken();
    if (token->sym == symIDENTIFIER && idexist(token->value) == NULL) {
      char temp_value[IDLEN];
      strcpy(temp_value, token->value);
      idpush(newid(temp_value, symCHARA, level));
      token = nextToken();
      if (token->sym == symASSIGN) {
        token = nextToken();
        if (token->sym == symSTRING) {
          sprintf(buf, "%s_%s\tDB\t", procname, temp_value);
          fprintf(outfile, buf);
          sprintf(buf, "'%s','$'\n", token->value);
          fprintf(outfile, buf);
          token = nextToken();
        }
        else {
          // needdo not a string
          Error(-1);
        }
      }
      else {
        sprintf(buf, "%s_%s\tDB\t", procname, temp_value);
        fprintf(outfile, buf);
        sprintf(buf, "'','$'\n");
        fprintf(outfile, buf);
      }
      while (token->sym == symCOMMA) {
        if (token->sym == symIDENTIFIER && idexist(token->value) == NULL) {
          strcpy(temp_value, token->value);
          idpush(newid(temp_value, symCHARA, level));
          token = nextToken();
          if (token->sym == symASSIGN) {
            token = nextToken();
            if (token->sym == symSTRING) {
              sprintf(buf, "%s_%s\tDB\t", procname, temp_value);
              fprintf(outfile, buf);
              sprintf(buf, "'%s','$'\n", token->value);
              fprintf(outfile, buf);
              token = nextToken();
            }
            else {
              // needdo not a string
              Error(-1);
            }
          }
          else {
            sprintf(buf, "%s_%s\tDB\t", procname, temp_value);
            fprintf(outfile, buf);
            sprintf(buf, "'','$'\n");
            fprintf(outfile, buf);
          }
        }
        else {
          // needdo not id
          Error(-1);
        }
      }
      if (token->sym == symSEMI) {
        token = nextToken();
      }
      else {
        // needdo slip if not ;
        Error(6);
      }
    }
    else {
      // needdo not id and id exist
      Error(-1);
    }
  }
  else {
    Error(-1);
    // needdo var datatype error
  }
}
// block
void Block()
{
  if (token->sym == symLBRAKET) {
    token = nextToken();
    ++level;
    while (token->sym != symRBRAKET) {
      Statement();
    }
    --level;
    idpop(level);
    token = nextToken();
  }
  else {
    // needdo no {
    Error(-1);
  }
}
// statement
void Statement()
{
  switch (token->sym)
  {
  case symSEMI:
    token = nextToken();
    break;
  case symLBRAKET:
    Block();
    break;
  case symIF:
    IfElseStatement();
    break;
  case symRETURN:
    if (procname == "main")
      fprintf(outfile, "\tMOV\tAX, 4C00H\n"
                       "\tINT\t21H\n");
    else if (procname == "")
      Error(-1);
    else
      fprintf(outfile, "\tRET\n");
    break;
  case symPRINTF:
    WriteStatement();
    break;
  case symSCANF:
    ReadStatement();
    break;
  case symWHILE:
    WhileStatement();
    break;
  // case symFOR:
  //   ForStatement();
  //   break;
  case symINT:
  case symCHARA:
    VarDeclaration();
    break;
  case symIDENTIFIER:
    temp_token = nextToken();
    if (temp_token->sym == symASSIGN)
      AssignmentStatement();
    else
      Expression();
    if (token->sym == symSEMI)
      token = nextToken();
    else {
      // needdo not ;
      Error(-1);
    }
    break;
  default:
    token = nextToken();
    // skip(statement, 23);
  }
}
// ifstatement
void IfElseStatement()
{
  if (token->sym == symIF)
  {
    token = nextToken();
    if (token->sym == symLPAREN) {
      token = nextToken();
      int temp_count = labelCount;
      Condition();
      labelCount++;
      if (token->sym == symRPAREN) {
        token = nextToken();
        if (token->sym == symLBRAKET)
          Block();
        else
          Statement();
        if (token->sym == symELSE) {
          token = nextToken();
          sprintf(buf, "\tjmp\t_end%d\n", temp_count);
          fprintf(outfile, buf);
          sprintf(buf, "_else%d:\n", temp_count);
          fprintf(outfile, buf);
          if (token->sym == symLBRAKET)
            Block();
          else
            Statement();
          sprintf(buf, "_end%d:\n", temp_count);
          fprintf(outfile, buf);
        }
        else {
          sprintf(buf, "_else%d:\n", temp_count);
          fprintf(outfile, buf);
        }
      }
      else {
        //needdo not )
        Error(-1);
      }
    }
    else {
      // needdo not (
      Error(-1);
    }
  }
  else {
    Error(12);
    skip(statement, 23);
  }
}
// opposite condition
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
    int operator = token->sym;
    token = nextToken();
    Expression();
    sprintf(buf, "\tPOP\tBX\n"
                  "\tPOP\tAX\n"
                  "\tCMP\tAX, BX\n");
    fprintf(outfile, buf);
    switch (operator)
    {
      case symEQ:
        sprintf(buf,"\tJNE\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
      case symNEQ:
        sprintf(buf,"\tJE\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
      case symLESS:
        sprintf(buf,"\tJGE\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
      case symLEQ:
        sprintf(buf,"\tJG\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
      case symGREATER:
        sprintf(buf,"\tJLE\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
      case symGEQ:
        sprintf(buf,"\tJL\t_else%d\n",labelCount);
        fprintf(outfile, buf);
        break;
    }
  }
  else
  {
    Error(20);
    skip(statement, 23);
  }
}
// assign
void AssignmentStatement()
{
  char result[BUFSIZE];
  struct idlist *tempid = idexist(token->value);
  if (temp_token != NULL) {
    token = temp_token;
    temp_token = NULL;
  }
  else {
    token = nextToken();
  }
  if (tempid != NULL && tempid->sym == symINT) {
    char temp_procname[IDLEN] = "";
    if (tempid->level != 0)
      strcpy(temp_procname, procname);
    sprintf(result, "%s_%s", temp_procname, tempid->name);
  }
  if (token->sym == symASSIGN)
  {
    token = nextToken();
    Expression();
    sprintf(buf, "\tPOP\tAX\n"
                  "\tMOV\t[%s], AX\n", result);
    fprintf(outfile, buf);
  }
  else
  {
    Error(8);
    skip(statement, 23);
  }
}
// while
void WhileStatement()
{
  int home, head, tail;
  if (token->sym == symWHILE)
  {
    token = nextToken();
    if (token->sym == symLPAREN) {
      token = nextToken();
      int temp_count = labelCount;
      sprintf(buf, "_while%d:\n", temp_count);
      fprintf(outfile, buf);
      Condition();
      labelCount++;
      if (token->sym == symRPAREN) {
        token = nextToken();
        if (token->sym == symLBRAKET)
          Block();
        else
          Statement();
        sprintf(buf, "\tjmp\t_while%d\n", temp_count);
        fprintf(outfile, buf);
        sprintf(buf, "_else%d:\n", temp_count);
        fprintf(outfile, buf);
      }
      else {
        //needdo not )
        Error(-1);
      }
    }
    else {
      // needdo not (
      Error(-1);
    }
  }
  else
  {
    Error(14);
    skip(statement, 23);
  }
}
// scanf
void ReadStatement()
{
  if (token->sym == symSCANF)
  {
    token = nextToken();
    if (token->sym == symLPAREN)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        struct idlist *tempid = idexist(token->value);
        token = nextToken();
        if (tempid != NULL && tempid->sym == symINT)
        {
          char temp_procname[IDLEN] = "";
          if (tempid->level != 0)
            strcpy(temp_procname, procname);
          sprintf(id, "%s_%s", temp_procname, tempid->name);
          sprintf(buf, "\treadstr\t_buf\n"
                        "\tstrtoi\t_buf, '$', %s\n"
                        "\tnewline\n", id);
          fprintf(outfile, buf);
        }
        else {
          // not an id or is char array
          Error(-1);
        }
      }
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          struct idlist *tempid = idexist(token->value);
          token = nextToken();
          if (tempid != NULL && tempid->sym == symINT)
          {
            char temp_procname[IDLEN] = "";
            if (tempid->level != 0)
              strcpy(temp_procname, procname);
            sprintf(id, "%s_%s", temp_procname, tempid->name);
            sprintf(buf, "\treadstr\t_buf\n"
                          "\tstrtoi\t_buf, '$', %s\n"
                          "\tnewline\n", id);
            fprintf(outfile, buf);
          }
          else {
            // not an id or is char array
            Error(-1);
          }
        }
      }
      if (token->sym == symRPAREN)
        token = nextToken();
      else {
        Error(18);
        skip(statement, 23);
      }
      if (token->sym == symSEMI)
        token = nextToken();
      else {
        // needdo not ;
        Error(-1);
        skip(statement, 23);
      }
    }
    else {
      Error(17);
      skip(statement, 23);
    }
  }
  else {
    Error(16);
    skip(statement, 23);
  }
}
// printf
void WriteStatement()
{
  if (token->sym == symPRINTF)
  {
    token = nextToken();
    if (token->sym == symLPAREN)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        struct idlist *tempid = idexist(token->value);
        token = nextToken();
        if (tempid != NULL)
        {
          char temp_procname[IDLEN] = "";
          if (tempid->level != 0)
            strcpy(temp_procname, procname);
          sprintf(id, "%s_%s", temp_procname, tempid->name);
          if (tempid->sym == symCHARA)
          {
            sprintf(buf,"\tdispstr\t%s\n", id);
            fprintf(outfile, buf);
          }
          else
          {
            sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
              "\tMOV\tDX, _intstr\n"
              "\tMOV\tAH, 09H\n"
              "\tINT\t21H\n", id);
            fprintf(outfile, buf);
          }
        }
        else {
          Error(26);
        }
      }
      else if (token->sym == symENDL) {
        sprintf(buf,"\tnewline\n");
        fprintf(outfile, buf);
        token = nextToken();
      }
      else if (token->sym == symSTRING) {
        sprintf(buf,"\tdispstr\t%s\n", token->value);
        fprintf(outfile, buf);
        token = nextToken();
      }
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          struct idlist *tempid = idexist(token->value);
          token = nextToken();
          if (tempid != NULL)
          {
            char temp_procname[IDLEN] = "";
            if (tempid->level != 0)
              strcpy(temp_procname, procname);
            sprintf(id, "%s_%s", temp_procname, tempid->name);
            if (tempid->sym == symCHARA)
            {
              sprintf(buf,"\tdispstr\t%s\n", id);
              fprintf(outfile, buf);
            }
            else
            {
              sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
                "\tMOV\tDX, _intstr\n"
                "\tMOV\tAH, 09H\n"
                "\tINT\t21H\n", id);
              fprintf(outfile, buf);
            }
          }
          else {
            Error(26);
          }
        }
        else if (token->sym == symENDL) {
          sprintf(buf,"\tnewline\n");
          fprintf(outfile, buf);
          token = nextToken();
        }
        else if (token->sym == symSTRING) {
          sprintf(buf,"\tdispstr\t%s\n", token->value);
          fprintf(outfile, buf);
          token = nextToken();
        }
      }
      if (token->sym == symRPAREN)
        token = nextToken();
      else
      {
        Error(18);
        skip(statement, 23);
      }
      if (token->sym == symSEMI)
        token = nextToken();
      else
      {
        // needdo not ;
        Error(-1);
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
// expression not change
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
    int operator = token->sym;
    token = nextToken();
    Term();
    if (operator == symPLUS)
    {
      fprintf(outfile,"\tPOP\tBX\n"
                  "\tPOP\tAX\n"
                  "\tADD\tAX, BX\n"
                  "\tPUSH\tAX\n");
    }
    else
    {
      fprintf(outfile, "\tPOP\tBX\n"
                    "\tPOP\tAX\n"
                    "\tSUB\tAX, BX\n"
                    "\tPUSH\tAX\n");
    }
  }
}
// term not change
void Term()
{
  Factor();
  while (token->sym == symMUL ||
          token->sym == symDIV)
  {
    int operator = token->sym;
    token = nextToken();
    Factor();
    if (operator == symMUL)
    {
      fprintf(outfile, "\tPOP\tBX\n"
                    "\tPOP\tAX\n"
                    "\tMUL\tBX\n"
                    "\tPUSH\tAX\n");
    }
    else
    {
      fprintf(outfile, "\tPOP\tBX\n"
                    "\tMOV\tDX, 0\n"
                    "\tPOP\tAX\n"
                    "\tDIV\tBX\n"
                    "\tPUSH\tAX\n");
    }
  }
}
// factor a little change
void Factor()
{
  if (token->sym == symIDENTIFIER)
  {
    char temp_value[IDLEN];
    strcpy(temp_value, token->value);
    struct idlist *tempid = idexist(temp_value);
    if (temp_token != NULL) {
      token = temp_token;
      temp_token = NULL;
    }
    else
      token = nextToken();
    if (token->sym == symLPAREN) {
      token = nextToken();
      if (token->sym == symRPAREN) {
        token = nextToken();
        if (procexist(&pl_head, temp_value, 0) != 0) {
          sprintf(buf, "\tCALL\t_%s\n", temp_value);
          fprintf(outfile, buf);
        }
        else {
          // needdo func not exist
          Error(-1);
        }
      }
      else {
        // needdo func call error
        Error(-1);
      }
    }
    else if (tempid != NULL && tempid->sym == symINT) {
      char temp_procname[IDLEN] = "";
      if (tempid->level != 0)
        strcpy(temp_procname, procname);
      sprintf(id, "%s_%s", temp_procname, tempid->name);
      sprintf(buf, "\tPUSH\tWORD [%s]\n", id);
      fprintf(outfile, buf);
    }
  }
  else if (token->sym == symNUMBER)
  {
    sprintf(buf, "\tPUSH\t%s\n", token->value);
    fprintf(outfile, buf);
    token = nextToken();
  }
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
// outfile init
void outputfile_init(char* ifname)
{
  char outname[IDLEN] = "";
  int jump_i = 0;
  for (int i = 0; i < strlen(ifname)-1; i++) {
    if (ifname[i] == '\\')
      jump_i = i+1;
  }
  for (int temp_i = 0; temp_i < strlen(ifname)-jump_i; temp_i++) {
    if (ifname[temp_i+jump_i]=='.' || ifname[temp_i+jump_i] == '\0') {
      outname[temp_i] = '\0';
      break;
    }
    outname[temp_i] = ifname[temp_i+jump_i];
  }
  strcat(outname, ".asm");
  char temp_name[IDLEN] = "asmcom\\";
  strcat(temp_name, outname);
  outfile = fopen(temp_name, "w");
  sprintf(buf,
          "\tORG\t100H\n"
          "\tJMP\t_main\n"
          "_intstr\tDB\t'     ','$'\n"
          "_buf\tTIMES 256 DB ' '\n"
          "\tDB 13,10,'$'\n");
  fprintf(outfile, buf);
  strcpy(buf, "%include\t\"dispstr.mac\"\n");
  strcat(buf, "%include\t\"itostr.mac\"\n");
  strcat(buf, "%include\t\"readstr.mac\"\n");
  strcat(buf, "%include\t\"strtoi.mac\"\n");
  strcat(buf, "%include\t\"newline.mac\"\n");
  fputs(buf, outfile);
}

// main
int main(int argc, char *argv[])
{
  FILE *f=fopen(argv[1], "r");
  scanner(f);
  followsyminit();
  token = nextToken();
  outputfile_init(argv[1]);
  Program();
  printf("\n  clike compile completed. "
    "\n  Error count : %d\n", errorCount);
  fclose(outfile);
  fclose(f);
  return 0;
}
