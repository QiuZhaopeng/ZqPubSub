#pragma once

#ifndef ZQ_PUBLISHER_H
#define ZQ_PUBLISHER_H

#include <memory>
#include "PubSubUtils.h"

namespace ZqPubSub {

    template <typename T>
    class Publisher
    {
    public:
        Publisher(const char* topicName, std::shared_ptr<SubscriberList<T>> pSubscriberList)
        {
            m_topicName = topicName;
            m_pSubscriberList = pSubscriberList;
        }

        ~Publisher()
        {
        }

        void Publish(const T& data)
        {
            if (m_pSubscriberList == nullptr)
            {
                return;
            }

            m_pSubscriberList->m_Mutex.lock();
            for (auto itr : m_pSubscriberList->m_List)
            {
                auto sub = itr.get();
                sub->PushData(data);
            }
            m_pSubscriberList->m_Mutex.unlock();
        }

    private:
        const char* m_topicName;
        std::shared_ptr<SubscriberList<T>> m_pSubscriberList; 
    };
}
#endif  // ZQ_PUBLISHER_H