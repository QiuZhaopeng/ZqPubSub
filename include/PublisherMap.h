#pragma once
#ifndef ZQ_PUBLISHERMAP_H
#define ZQ_PUBLISHERMAP_H

#include <unordered_map>
#include <mutex>
#include "PubSubUtils.h"

namespace ZqPubSub {

    template<typename T> class PubSubFactory;

    template<typename T>
    class PublisherMap
    {
        friend class PubSubFactory<T>;

    private:
        static void AddPublisher(const char* topicName, std::shared_ptr<Publisher<T>> sharedPtr)
        {
            m_Mutex.lock();
            auto itr = m_PublisherMap.find(topicName);
            if (itr == m_PublisherMap.end())
            {
                auto pubList = std::make_shared<PublisherList<T>>();
                pubList.get()->m_List.push_back(sharedPtr);
                m_PublisherMap.insert({ topicName, pubList });
            }
            else
            {
                auto subList = itr->second.get();
                subList->m_Mutex.lock();
                subList->m_List.push_back(sharedPtr);
                subList->m_Mutex.unlock();
            }
            m_Mutex.unlock();
        }
        static void Add(const char* topicName)
        {
            m_Mutex.lock();
            auto itr = m_PublisherMap.find(topicName);
            if (itr == m_PublisherMap.end())
            {
                auto pList = std::make_shared<PublisherList<T>>();
                m_PublisherMap.insert({ topicName, pList });
            }
            m_Mutex.unlock();
        }

        static std::shared_ptr<PublisherList<T>> Get(const char* topicName)
        {
            std::shared_ptr<PublisherList<T>>  res = nullptr;
            m_Mutex.lock();
            auto itr = m_PublisherMap.find(topicName);
            if (itr != m_PublisherMap.end())
            {
                res = itr->second;
            }
            m_Mutex.unlock();
            return  res;
        }

        static void Delete(const char* topicName)
        {
            // TODO
        }

        static std::unordered_map<std::string, std::shared_ptr<PublisherList<T>>> m_PublisherMap;
        static std::mutex m_Mutex;
    };

    template<typename T>
    std::unordered_map<std::string, std::shared_ptr<PublisherList<T>>> PublisherMap<T>::m_PublisherMap;

    template<typename T>
    std::mutex  PublisherMap<T>::m_Mutex;
}
#endif // ZQ_PUBLISHERMAP_H