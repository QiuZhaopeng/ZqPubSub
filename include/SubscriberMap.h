#pragma once
#ifndef ZQ_SUBSCRIBERMAP_H
#define ZQ_SUBSCRIBERMAP_H

#include <unordered_map>
#include "PubSubUtils.h"
#include <mutex>

namespace ZqPubSub {

    template<typename T> class PubSubFactory;


    /* Template class for managing a map containing <topic_name, [subscriber]> pairs */
    template<typename T>
    class SubscriberMap
    {
        friend class PubSubFactory<T>;

    private:
        static void AddSubscriber(const char* topicName, std::shared_ptr<Subscriber<T>> sharedPtr)
        {
            std::lock_guard<std::mutex> lg(m_Mutex);
            auto itr = m_SubscriberMap.find(topicName);
            if (itr == m_SubscriberMap.end())
            {
                auto pList = std::make_shared<SubscriberList<T>>();
                pList.get()->m_List.push_back(sharedPtr);
                m_SubscriberMap.insert({topicName, pList});
            }
            else
            {
                auto subList = itr->second.get();
                std::lock_guard<std::mutex> _lg(subList->m_Mutex);
                subList->m_List.push_back(sharedPtr);
            }
        }

        static void AddTopic(const char* topicName)
        {
            std::lock_guard<std::mutex> lg(m_Mutex);
            auto itr = m_SubscriberMap.find(topicName);
            if (itr == m_SubscriberMap.end())
            {
                auto pList = std::make_shared<SubscriberList<T>>();
                m_SubscriberMap.insert({ topicName, pList });
            }
        }

        static std::shared_ptr<SubscriberList<T>> GetSubscribers(const char* topicName)
        {
            std::shared_ptr<SubscriberList<T>>  res = nullptr;
            std::lock_guard<std::mutex> lg(m_Mutex);
            auto itr = m_SubscriberMap.find(topicName);
            if (itr != m_SubscriberMap.end())
            {
                res = itr->second;
            }

            return  res;
        }

        static void DeleteTopic(const char* topicName)
        {
            // TODO
        }
        static void DeleteSubscriber(const char* topicName)
        {
            // TODO
        }

        static std::unordered_map<std::string, std::shared_ptr<SubscriberList<T>>> m_SubscriberMap;
        static std::mutex m_Mutex;
    };


    template<typename T>
    std::unordered_map<std::string, std::shared_ptr<SubscriberList<T>>> SubscriberMap<T>::m_SubscriberMap;

    template<typename T>
    std::mutex  SubscriberMap<T>::m_Mutex;

}

#endif // ZQ_SUBSCRIBERMAP_H