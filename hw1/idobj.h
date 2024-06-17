/******************* idobj.h ******************/
struct idobjTag
{
  char name[36];
  int sym;
  int attr;
  int level;
};
char * idobjToString(struct idobjTag *p)
{
  static char idobj_str[512];
  sprintf(idobj_str,
    "  name:\"%s\"\tsym:%d\tattr:%d\tlevel:%d", 
    p->name, p->sym, p->attr,p->level);
  return idobj_str;
}
