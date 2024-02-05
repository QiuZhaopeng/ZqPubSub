#pragma once
#ifndef ZQ_PUBLISHERMAP_H
#define ZQ_PUBLISHERMAP_H

#include <unordered_map>
#include <mutex>
#include "PubSubUtils.h"

namespace ZqPubSub {

    template<typename T> class PubSubFactory;

    /* Template class for managing a map containing <topic_name, [publishers]> pairs */
    template<typename T>   
    class PublisherMap
    {
        friend class PubSubFactory<T>;

    private:
        static void AddPublisher(const char* topicName, std::shared_ptr<Publisher<T>> sharedPtr)
        {
            std::lock_guard<std::mutex> lg(m_Mutex);
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
                std::lock_guard<std::mutex> _lg(subList->m_Mutex);
                subList->m_List.push_back(sharedPtr);
            }

        }

        static void Add(const char* topicName)
        {
            std::lock_guard<std::mutex> lg(m_Mutex);
            auto itr = m_PublisherMap.find(topicName);
            if (itr == m_PublisherMap.end())
            {
                auto pList = std::make_shared<PublisherList<T>>();
                m_PublisherMap.insert({ topicName, pList });
            }
        }

        static std::shared_ptr<PublisherList<T>> Get(const char* topicName)
        {
            std::shared_ptr<PublisherList<T>>  res = nullptr;
            std::lock_guard<std::mutex> lg(m_Mutex);
            auto itr = m_PublisherMap.find(topicName);
            if (itr != m_PublisherMap.end())
            {
                res = itr->second;
            }

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