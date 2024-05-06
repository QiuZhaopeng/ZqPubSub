#ifndef ZQ_SENSORSERVICE_H
#define ZQ_SENSORSERVICE_H

#include "BaseService.h"
#include <string>
#include <thread>
#include <functional>
#include "PubSubFactory.h"

namespace ZqPubSub {

/*
* SensorService class for services (threads)
*/
class SensorService : public BaseService
{
public:
    /*
     *  Constructor funtion of SensorService which publishes data of sensor
     *  @param[in] topic  Data Topic Name
     *  @param[in] serviceName  Service Name
     *  @param[in] ratioData This is a data related parameter used only for demo
     */
    SensorService(const std::string& topic, const std::string& m_serviceName, int ratioData);

    /*
     *  Destructor funtion of SensorService
     */
    virtual ~SensorService() {};
    void Run() override final;
    void Stop() override final;
    void Task_Func();

private:
    std::shared_ptr<Publisher<int>> m_intPublisher; ///< Sensor data publisher
    int m_ratioData; ///< For demo only, no real meaning
};

}

#endif // ZQ_SENSORSERVICE_H