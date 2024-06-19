#ifndef RESWORD_H
#define RESWORD_H
#include <stdlib.h>
#define RESWORDMAX 12
char reswords[RESWORDMAX][10] =
  {
  "int","void","string","return","if","else","while",
  "do","scanf","printf","for","endl"
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

# endif
