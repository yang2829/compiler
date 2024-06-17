/********************** resword.h ******************/
#include <stdlib.h>
#define RESWORDMAX 13
char reswords[RESWORDMAX][10] =
  {
  "BEGIN","CALL","CONST","DO","END","IF","PROCEDURE",
  "PROGRAM","READ","THEN","VAR","WHILE","WRITE"
  };
int isResword(char *s)
{
  int i;
  for (i=0; i<RESWORDMAX; i++)
  {
    if (strcmp(s, reswords[i])==0) return i;
  }
  return -1;
}
