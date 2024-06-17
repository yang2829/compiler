/************* startsym.h ***********/
#include "sym.h"
char factor[symSYMMAX];
char term[symSYMMAX];
char expression[symSYMMAX];
char condition[symSYMMAX];
char statement[symSYMMAX];
char block[symSYMMAX];
void startsyminit()
{
  factor[symIDENTIFIER] = 1;
  factor[symNUMBER] = 1;
  factor[symLPAREN] = 1;
  term[symIDENTIFIER] = 1;
  term[symNUMBER] = 1;
  term[symLPAREN] = 1;
  expression[symPLUS] = 1;
  expression[symMINUS] = 1;
  expression[symLPAREN] = 1;
  expression[symIDENTIFIER] = 1;
  expression[symNUMBER] = 1;
  condition[symPLUS] = 1;
  condition[symMINUS] = 1;
  condition[symLPAREN] = 1;
  condition[symIDENTIFIER] = 1;
  condition[symNUMBER] = 1;
  statement[symIDENTIFIER] = 1;
  statement[symCALL] = 1;
  statement[symBEGIN] = 1;
  statement[symIF] = 1;
  statement[symWHILE] = 1;
  statement[symREAD] = 1;
  statement[symWRITE] = 1;
  statement[symDO] = 1;
  block[symCONST] = 1;
  block[symVAR] = 1;
  block[symPROCEDURE] = 1;
  block[symBEGIN] = 1;
}
