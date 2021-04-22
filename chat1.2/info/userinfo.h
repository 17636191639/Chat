#ifndef USERINFO_H
#define USERINFO_H

struct userinfo
{
    char id[8];
    char pswd[8];
    char name[8];
};
void userinfo_clear(struct userinfo *p);

#endif // USERINFO_H
