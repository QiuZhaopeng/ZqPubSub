#pragma once

#ifndef ZQ_PUBSUBFACTORY_H
#define ZQ_PUBSUBFACTORY_H

#include <memory>
#include "Publisher.h"
#include "Subscriber.h"
#include "SubscriberMap.h"
#include "PublisherMap.h"

namespace ZqPubSub
{
    template<typename T>
    class PubSubFactory
    {
    public:
        static std::shared_ptr<Publisher<T>> CreatePublisher(const char* topicName)
        {
            auto pSubscriberList = SubscriberMap<T>::GetSubscribers(topicName);

            if (pSubscriberList == nullptr)
            {
                SubscriberMap<T>::AddTopic(topicName);
                pSubscriberList = SubscriberMap<T>::GetSubscribers(topicName);
            }

            auto p = new Publisher<T>(topicName, pSubscriberList);
            std::shared_ptr<Publisher<T>> item(p);
            PublisherMap<T>::AddPublisher(topicName, item);

            return item;
        }

        static std::shared_ptr<Subscriber<T>> CreateSubscriber(const char* topicName, size_t maxSize)
        {
            auto p = new Subscriber<T>(topicName, maxSize);
            std::shared_ptr<Subscriber<T>> item(p);
            SubscriberMap<T>::AddSubscriber(topicName, item);

            return item;
        }
    };
}
#endif //ZQ_PUBSUBFACTORY_H