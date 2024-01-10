#pragma once
#ifndef ZQ_SUBSCRIBERMAP_H
#define ZQ_SUBSCRIBERMAP_H

#include <unordered_map>
#include "PubSubUtils.h"
#include <mutex>

namespace ZqPubSub {

    template<typename T> class PubSubFactory;

    template<typename T>
    class SubscriberMap
    {
        friend class PubSubFactory<T>;

    private:
        static void AddSubscriber(const char* topicName, std::shared_ptr<Subscriber<T>> sharedPtr)
        {
            m_Mutex.lock();
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
                subList->m_Mutex.lock();
                subList->m_List.push_back(sharedPtr);
                subList->m_Mutex.unlock();
            }
            m_Mutex.unlock();
        }

        static void AddTopic(const char* topicName)
        {
            m_Mutex.lock();
            auto itr = m_SubscriberMap.find(topicName);
            if (itr == m_SubscriberMap.end())
            {
                auto pList = std::make_shared<SubscriberList<T>>();
                m_SubscriberMap.insert({ topicName, pList });
            }
            m_Mutex.unlock();
        }

        static std::shared_ptr<SubscriberList<T>> GetSubscribers(const char* topicName)
        {
            std::shared_ptr<SubscriberList<T>>  res = nullptr;
            m_Mutex.lock();
            auto itr = m_SubscriberMap.find(topicName);
            if (itr != m_SubscriberMap.end())
            {
                res = itr->second;
            }
            m_Mutex.unlock();
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