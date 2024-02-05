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
        /*
        *  Constructor funtion of PUblisher
        *  @param[in] topicName  Topic Name
        *  @param[in] pSubscriberList pointer object of a subscriber list
        */
        Publisher(const char* topicName, std::shared_ptr<SubscriberList<T>> pSubscriberList)
        {
            m_topicName = topicName;
            m_pSubscriberList = pSubscriberList;
        }

        ~Publisher()
        {
        }

        /* write message data to each subscriber of the corresponding topic  */
        void Publish(const T& data)
        {
            if (m_pSubscriberList == nullptr)
            {
                return;
            }

            // RAII principle: use lock_guard instead of mutex.lock()...mutex.unlock()
            std::lock_guard<std::mutex> lg(m_pSubscriberList->m_Mutex);
            for (auto itr : m_pSubscriberList->m_List)
            {
                auto sub = itr.get();
                sub->PushData(data);
            }
        }

    private:
        const char* m_topicName;           ///< Topic name of the published message 
        std::shared_ptr<SubscriberList<T>> m_pSubscriberList; ///< Pointer containing a list of the subscribers of the topic
    };
}
#endif  // ZQ_PUBLISHER_H