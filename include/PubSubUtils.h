#pragma once
#ifndef ZQ_PUBSUBUTILS_H
#define ZQ_PUBSUBUTILS_H

#include<list>
#include<mutex>
#include "Subscriber.h"
#include "Publisher.h"

namespace ZqPubSub {

    template<typename T> class Publisher;

    template<typename T>
    struct ThreadSafeList
    {
        std::list<T> m_List;
        std::mutex m_Mutex;
    };

    template<typename T>
    using PublisherList = ThreadSafeList<std::shared_ptr<Publisher<T>>>;

    template<typename T>
    using SubscriberList = ThreadSafeList<std::shared_ptr<Subscriber<T>>>;
}

#endif // ZQ_PUBSUBUTILS_H