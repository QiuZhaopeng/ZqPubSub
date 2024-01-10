#pragma once
#ifndef ZQ_SUBSCRIBER_H
#define ZQ_SUBSCRIBER_H

#include <string>
#include "PubSubFactory.h"
#include "MessageQueue.h"

namespace ZqPubSub {

    template <typename T>
    class Subscriber
    {
        using MsgQueue = ZqPubSub::MessageQueue<T>;

    public:
    
        Subscriber(const char* topicName, size_t maxSize) : 
            m_topicName(topicName),
            m_queue(maxSize),
            m_lastReturnedMsg()
        {}

        ~Subscriber() {
        }
        
        // get one single msg from m_queue of subscriber
        T GetData()
        {
            m_lastReturnedMsg = m_queue.Pop();
            return m_lastReturnedMsg;
        }

        // get all msgs still saved in m_queue of subscriber
        void GetAllData(std::vector<T>& allData)
        {
            m_queue.PopAllMessages(allData);
        }

        void PushData(const T& data)
        {
            m_queue.Push(data);
        }

    private:
    
        MsgQueue m_queue;        ///< queue containing subscribed messages from publisher
        const char* m_topicName; ///< subscribed topic name
        T m_lastReturnedMsg;     ///< latest msg GetData
    };

}
#endif // ZQ_SUBSCRIBER_H