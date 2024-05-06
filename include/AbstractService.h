#ifndef ZQ_ABSTRACTSERVICE_H
#define ZQ_ABSTRACTSERVICE_H

#include<iostream>
#include<functional>

namespace ZqPubSub {
/*
* Interface class for services (threads)
*/
class AbstractService
{
public:
    AbstractService() {};
    virtual ~AbstractService() {};
    virtual void Run() = 0;
   // virtual void Sleep(unsigned int time_in_ms) = 0;
};

}

#endif // ZQ_ABSTRACTSERVICE_H