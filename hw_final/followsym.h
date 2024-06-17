/*************** followsym.h ***************/
#include "sym.h"
char factor[symSYMMAX];
char term[symSYMMAX];
char expression[symSYMMAX];
char condition[symSYMMAX];
char statement[symSYMMAX];
char block[symSYMMAX];
void followsyminit()
{
  factor[symPERIOD] = 1;
  factor[symSEMI] = 1;
  factor[symRPAREN] = 1;
  factor[symPLUS] = 1;
  factor[symMINUS] = 1;
  factor[symMUL] = 1;
  factor[symDIV] = 1;
  factor[symEND] = 1;
  factor[symTHEN] = 1;
  factor[symDO] = 1;
  term[symPERIOD] = 1;
  term[symSEMI] = 1;
  term[symRPAREN] = 1;
  term[symPLUS] = 1;
  term[symMINUS] = 1;
  term[symEND] = 1;
  term[symTHEN] = 1;
  term[symDO] = 1;
  expression[symPERIOD] = 1;
  expression[symSEMI] = 1;
  expression[symRPAREN] = 1;
  expression[symEND] = 1;
  expression[symTHEN] = 1;
  expression[symDO] = 1;
  condition[symTHEN] = 1;
  condition[symDO] = 1;
  statement[symPERIOD] = 1;
  statement[symSEMI] = 1;
  statement[symEND] = 1;
  block[symPERIOD] = 1;
  block[symSEMI] = 1;
}
