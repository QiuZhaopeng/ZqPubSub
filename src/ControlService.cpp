#include <iostream>
#include "ControlService.h"

namespace ZqPubSub {

    ControlService::ControlService(const std::string& topic, const std::string& serviceName)
    : BaseService(serviceName)

    {
        m_intSubscriber = ZqPubSub::PubSubFactory<int>::CreateSubscriber(topic.c_str(), 20);
        m_serviceName = serviceName ;
    }
   void ControlService::Control_Task_Func()
   {
        int j = 0;
        while (m_isRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::vector<int> res;
            m_intSubscriber->GetAllData(res);
            if (!res.empty())
            {
                for (auto& item : res)
                std::cout << "[" << getName()  << "] receives: " << item << std::endl;
            }
            if(++j > 10) { m_isRunning = false;}
        }
   }

   void ControlService::Run()  
   {
       m_isRunning = true;
       std::cout << "+++ Control Service " << this->getName() << " is running !" << std::endl;
       m_thread = std::thread(&ControlService::Control_Task_Func, this);
   } 
   void ControlService::Stop()
   {
       m_isRunning = false;
       m_thread.join();
   }

}