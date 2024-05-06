#ifndef ZQ_BASESERVICE_H
#define ZQ_BASESERVICE_H

#include<thread>
#include<functional>
#include<string>

namespace ZqPubSub {
/*
* Interface class for services (threads)
*/
class BaseService
{
public:
    /*
    *  Constructor funtion of BaseService
    *  @param[in] serviceName  Service Name
    */
    BaseService(const std::string& serviceName) : m_serviceName(serviceName), m_isRunning(false) {};
    virtual ~BaseService() {};
    virtual void Run() = 0; ///< Start to run the service
    virtual void Stop() = 0; ///< Stop the service
    std::string getName() const {return m_serviceName;}
    bool IsRunning() {return m_isRunning;}

protected:
    std::thread m_thread; ///< Composition of a thread object
    std::string m_serviceName; ///< service name
    bool m_isRunning;  ///< bool indicating whether the service is running
};

}

#endif // ZQ_BASESERVICE_H