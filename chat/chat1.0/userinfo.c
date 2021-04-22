#include "userinfo.h"

void userinfo_clear(struct userinfo *p)
{
    bzero(p->id, 8);
    bzero(p->pswd, 8);
    bzero(p->name, 8);
}
