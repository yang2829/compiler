#ifndef IDPROCLIST_H
#define IDPROCLIST_H

#include <stdlib.h>

struct proclist
{
  char name[36];
  int used;
  struct proclist *next;
};

struct proclist * newProc(char *name, int flag)
{
  struct proclist *p=malloc(sizeof(struct proclist));
  strcpy(p->name, name);
  p->used = flag;
  p->next = NULL;
  return p;
}

int procexist(struct proclist* start, char* pname, int flag)
{
  for (;start != NULL; start = start->next) {
    if (strcmp(pname, start->name) == 0) {
      if (start->used == 1) {
        return 2;
      }
      start->used = flag;
      return 1;
    }
  }
  return 0;
}

void procfree(struct proclist* start)
{
  struct proclist* temp = start;
  start = start->next;
  temp->next = NULL;
  while (start != NULL) {
    printf("free: %s\n", start->name);
    temp = start;
    start = start->next;
    temp->next = NULL;
    free(temp);
  }
}

struct idlist
{
    char name[36];
    int sym;
    int level;
    struct idlist *pre;
} *idlistTop=NULL;

struct idlist * newid(char *name, int S, int L)
{
    struct idlist *p=malloc(sizeof(struct idlist));
    strcpy(p->name, name);
    p->sym = S;
    p->level = L;
    p->pre = NULL;
    return p;
}

void idpush(struct idlist *p)
{
    if (idlistTop != NULL)
        p->pre = idlistTop;
    idlistTop = p;
}

void idpop(int L)
{
    while (idlistTop != NULL && idlistTop->level > L) {
        struct idlist *tempid = idlistTop;
        idlistTop = idlistTop->pre;
        tempid->pre = NULL;
        free(tempid);
    }
}

struct idlist * idexist(char* idname)
{
    struct idlist* top = idlistTop;
    for (;top != NULL; top = top->pre) {
        if (strcmp(idname, top->name) == 0) {
            return top;
        }
    }
    return NULL;
}

#endif
