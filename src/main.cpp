#include <iostream>
#include <thread>
#include <vector>
#include "PubSubFactory.h"
#include "SensorService.h"
#include "ControlService.h"

int main()
{
    std::cout << "Start pubsub demo!" << std::endl;
    
    std::vector<std::shared_ptr<ZqPubSub::BaseService>> servicesVec; ///< A vector of services references in this demo

    // 2 Sensor Services for IMU and Laser respectively
    auto s1 = std::make_shared<ZqPubSub::SensorService>("data_imu", "IMU", 11);
    auto s2 = std::make_shared<ZqPubSub::SensorService>("data_laser", "LIDAR", 3);

    // 2 Control Services for receiving IMU and Laser respectively
    auto s3 = std::make_shared<ZqPubSub::ControlService>("data_imu", "Drive");
    auto s4 = std::make_shared<ZqPubSub::ControlService>("data_laser", "Perception");

    // put all the services' pointers into vector
    servicesVec.push_back(s1);
    servicesVec.push_back(s2);
    servicesVec.push_back(s3);
    servicesVec.push_back(s4);

    // Run each service in servicesVec, remember Run() is virtual function
    for (const auto& s : servicesVec)
    {
        s->Run();
    }

    // Let main thread run for a long time
    std::this_thread::sleep_for(std::chrono::milliseconds(6000));


    // Stop each service in servicesVec, remember Stop() is virtual function
    for (const auto& s : servicesVec)
    {
        s->Stop();
    }

    return 0;
}