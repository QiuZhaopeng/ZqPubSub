#ifndef ZQ_CONTROLSERVICE_H
#define ZQ_CONTROLSERVICE_H

#include "BaseService.h"
#include <string>
#include <thread>
#include <functional>
#include "PubSubFactory.h"

namespace ZqPubSub {

/*
* ControlService class
*/
class ControlService : public BaseService
{
public:
    /*
     *  Constructor funtion of ControlService which receives data from sensor
     *  @param[in] topic  Data Topic Name
     *  @param[in] serviceName  Service Name
     */
    ControlService(const std::string& topic, const std::string& m_serviceName);
    /*
     *  Destructor funtion of ControlService
     */
    virtual ~ControlService() {};
    void Run() override final;
    void Stop() override final;
    void Control_Task_Func();

private:

    std::shared_ptr<Subscriber<int>> m_intSubscriber;
};

}

#endif // ZQ_CONTROLSERVICE_H