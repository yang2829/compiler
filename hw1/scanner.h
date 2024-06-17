/********************** scanner.h ********************/
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdlib.h>
#include "sym.h"
#include "symbol.h"
#include "resword.h"
int nextChar;
int cp = 0;
int linelen = 0;
int linenum = 0;
char line[512];
FILE *sourcefile;
void advance()
{
  cp++;
  if (cp < linelen)
    nextChar = line[cp];
  else
  {
    if (feof(sourcefile))
    {
      linelen = 0;
      nextChar = -1;
    }
    else
    {
      if (fgets(line, 512, sourcefile) != NULL)
      {
        linelen = strlen(line);
        linenum++;
        cp = 0;
        nextChar = line[cp];
        printf("%4d %s", linenum, line);
      }
    }
  }
}
struct symbolTag *nextToken()
{
  static char s[128] = "";
  int n=0;
  while (1)
  {
    if (nextChar>='a' && nextChar<='z' ||
        nextChar>='A' && nextChar<='Z')
    {
      do
      {
        s[n++]=nextChar;
        advance();
      } while (nextChar >= 'a' && nextChar <= 'z' ||
               nextChar >= 'A' && nextChar <= 'Z' ||
               nextChar>='0' && nextChar<='9');
      s[n]='\0';
      int temp = isResword(s);
      switch (temp)
      {
      case 0:
        return newSymbol(symBEGIN,linenum,cp,s);
        break;
      case 1:
        return newSymbol(symCALL,linenum,cp,s);
        break;
      case 2:
        return newSymbol(symCONST,linenum,cp,s);
        break;
      case 3:
        return newSymbol(symDO,linenum,cp,s);
        break;
      case 4:
        return newSymbol(symEND,linenum,cp,s);
        break;
      case 5:
        return newSymbol(symIF,linenum,cp,s);
        break;
      case 6:
        return newSymbol(symPROCEDURE,linenum,cp,s);
        break;
      case 7:
        return newSymbol(symPROGRAM,linenum,cp,s);
        break;
      case 8:
        return newSymbol(symREAD,linenum,cp,s);
        break;
      case 9:
        return newSymbol(symTHEN,linenum,cp,s);
        break;
      case 10:
        return newSymbol(symVAR,linenum,cp,s);
        break;
      case 11:
        return newSymbol(symWHILE,linenum,cp,s);
        break;
      case 12:
        return newSymbol(symWRITE,linenum,cp,s);
        break;
      case 13:
        return newSymbol(symELSE,linenum,cp,s);
        break;
      case 14:
        return newSymbol(symMOD,linenum,cp,s);
        break;
      case 15:
        return newSymbol(symDDIV,linenum,cp,s);
        break;
      default:
        return newSymbol(symIDENTIFIER,linenum,cp,s);
      }
    }
    else if (nextChar>='0' && nextChar<='9')
    {
      do
      {
        s[n++]=nextChar;
        advance();
      } while (nextChar >= '0' && nextChar <= '9');
      s[n] = '\0';
      return newSymbol(symNUMBER,linenum,cp,s);
    }
    else
    {
      switch (nextChar)
      {
      case ';':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symSEMI,linenum,cp,s);
      case '.':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symPERIOD,linenum,cp,s);
      case ',':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symCOMMA,linenum,cp,s);
      case '(':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symLPAREN,linenum,cp, s);
      case ')':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symRPAREN,linenum,cp, s);
      case '+':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symPLUS,linenum,cp,s);
      case '-':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symMINUS,linenum,cp, s);
      case '*':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symMUL,linenum,cp, s);
      case '/':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symDIV,linenum,cp, s);
      case '=':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symEQ,linenum,cp, s);
      case '<':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        if (nextChar=='=')
        {
          s[n++]=nextChar;
          advance();
          s[n]='\0';
          return newSymbol(symLEQ,linenum,cp,s);
        }
        else if (nextChar=='>')
        {
          s[n++]=nextChar;
          advance();
          s[n]='\0';
          return newSymbol(symNEQ,linenum,cp,s);
        }
        else
        {
          return newSymbol(symLESS,linenum,cp,s);
        }
      case '>':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        if (nextChar=='=')
        {
          s[n++]=nextChar;
          advance();
          s[n]='\0';
          return newSymbol(symGEQ,linenum,cp,s);
        }
        else
        {
          return newSymbol(symGREATER,linenum,cp,s);
        }
      case ':':
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        if (nextChar=='=')
        {
          s[n++]=nextChar;
          advance();
          s[n]='\0';
          return newSymbol(symBECOMES,linenum,cp, s);
        }
        else
        {
          return newSymbol(symerror,linenum,cp, s);
        }
      case '"':
        advance();
        while (nextChar!='"')
        {
          s[n++]=nextChar;
          advance();
        }
        advance();
        s[n]='\0';
        return newSymbol(symSTRING,linenum,cp,s);
      case '\0':
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        advance();          //�����ťզr��
        break;
      case -1:
        return NULL;
      default:
        s[n++]=nextChar;
        s[n]='\0';
        advance();
        return newSymbol(symerror, linenum,cp,s);
      }
    }
  }
}
void scanner(FILE *f)
{
  sourcefile = f;
}
#endif
