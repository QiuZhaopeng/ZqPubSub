#include <iostream>
#include "SensorService.h"

namespace ZqPubSub {

    SensorService::SensorService(const std::string& topic, const std::string& serviceName, int ratioData)
    : BaseService(serviceName)
    {
        m_intPublisher = ZqPubSub::PubSubFactory<int>::CreatePublisher(topic.c_str());
        m_ratioData = ratioData;
    }

    /* a simulated function for now, simulating data collection in real case*/
   void SensorService::Task_Func()
   {
       int i = 0;
       while (m_isRunning)
       {
           auto data = m_ratioData * i;
           m_intPublisher->Publish(data);
           std::cout << "[" << getName()  << "] publishes: " << data << std::endl;
           i++;
           std::this_thread::sleep_for(std::chrono::milliseconds(50 * m_ratioData));
           m_isRunning = (i < 10); 
       }
   }

   void SensorService::Run()  
   {
       m_isRunning = true;
       std::cout << "--- Sensor Service " << this->getName() << " is running !" << std::endl;
       m_thread = std::thread(&SensorService::Task_Func, this);
   }

   void SensorService::Stop()
   {
       m_isRunning = false;
       m_thread.join();
   }
}