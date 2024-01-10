#include <iostream>
#include <thread>
#include <vector>
#include "PubSubFactory.h"

int main()
{
    std::cout << "Start pubsub demo!" << std::endl;

    auto pub = ZqPubSub::PubSubFactory<int>::CreatePublisher("hello");
    auto pub_task = [&pub]() {
        int i = 0;
        while (i < 10)
        {
            auto data = 11 * i;
            pub->Publish(data);
            std::cout << "--- Publisher publishes: " << data << std::endl;
            i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        };
    auto t1 = std::thread::thread(pub_task);

    auto sub = ZqPubSub::PubSubFactory<int>::CreateSubscriber("hello", 20);
    for (int j = 0; j < 10; j++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        std::vector<int> res;
        sub->GetAllData(res);
        if (!res.empty())
        {
            for (auto& item : res)
            std::cout << "+++ Subscriber receives: " << item << std::endl;
        }
    }
    
    t1.join();
    return 0;
}