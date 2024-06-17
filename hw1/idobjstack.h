/********************* idobjstack.h ********************/
#include <stdlib.h>
struct idobjTag *idobjStack[512];
int    idobjTop = 0;
void idobjpush(struct idobjTag *p)
{
  idobjStack[idobjTop++]=p;
}
struct idobjTag * idobjpop()
{
  struct idobjTag *p=idobjStack[--idobjTop];
  return p;
}
struct idobjTag *newIdobj(char name[], int sym,
                          int attr, int level)
{
  struct idobjTag *p=malloc(sizeof(struct idobjTag));
  strcpy(p->name, name);
  p->sym = sym;
  p->attr = attr;
  p->level = level;
  return p;
}
struct idobjTag *getIdobj(char *id)
{
  struct idobjTag *idobj=NULL;
  int i;
  for (i=0; i<idobjTop; i++)
  {
    if (strcmp(idobjStack[i]->name,id)==0)
      return idobjStack[i];
  }
  return idobj;
}
char * idobjstackToString()
{
  static char idobjstack_str[1024];
  int i;
  strcpy(idobjstack_str, "");
  for (i=0; i<idobjTop; i++)
  {
    strcat(idobjstack_str, idobjToString(idobjStack[i]));
    strcat(idobjstack_str, "\n");
  }
  return idobjstack_str;
}
