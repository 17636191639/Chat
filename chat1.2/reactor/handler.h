#ifndef HANDLER_H
#define HANDLER_H
#include <map>

class Handler
{
public:
    Handler(){}
    virtual ~Handler(){}

    virtual void handleAccept(){}
    virtual void handleRead(){}
    virtual void handleWrite(){}

protected:
    int m_fd; //文件描述符

};
typedef Handler* HandlePtr;
typedef std::map<int, HandlePtr> HandleMap;
#endif // HANDLER_H
