#ifndef ZQ_MESSAGEQUEUE_H
#define ZQ_MESSAGEQUEUE_H

#include<queue>
#include<mutex>
#include<condition_variable>

namespace ZqPubSub {

    /* MessageQueue class 
     * Thread safe container for messages that have been published
     */
    template<typename T>
    class MessageQueue
    {
    public:
        MessageQueue(size_t maxSize = 16) { m_maxSize = maxSize; };
        virtual ~MessageQueue() { };

        T Pop();
        void Push(const T& msg);
        size_t Size();
        void PopAllMessages(std::vector<T>& msgsInQueue);
        void PopMessageWithTimeout(bool& timeOut, T& msg, int timeout_ms) { /* TODO */ };

    private:
        size_t m_maxSize;               ///< max queue size
        std::queue<T> m_msgQueue;       ///< queue containing messages
        mutable std::mutex m_mutex;     ///< mutex for protecting queue data access
        std::condition_variable m_cv;   ///< condition variable object
    };

    template<typename T>
    void MessageQueue<T>::Push(const T& msg)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_msgQueue.size() >= m_maxSize)
            {
                m_msgQueue.pop();
            }
            m_msgQueue.push(msg);
        }
        m_cv.notify_one();
    }

    template<typename T>
    T MessageQueue<T>::Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_msgQueue.empty())
        {
            m_cv.wait(lock);
        }
        T data = m_msgQueue.front();
        m_msgQueue.pop();
        return data;
    }

    template<typename T> 
    void MessageQueue<T>::PopAllMessages(std::vector<T>& msgsInQueue)
    {
        m_mutex.lock();
        std::queue<T> tempQueue = m_msgQueue;
        m_mutex.unlock();
        while (!tempQueue.empty())
        {
            msgsInQueue.push_back(tempQueue.front());
            tempQueue.pop();
            m_mutex.lock();
            m_msgQueue.pop();
            m_mutex.unlock();
        }
    }

    template<typename T>
    size_t MessageQueue<T>::Size()
    {
        m_mutex.lock();
        auto size = m_msgQueue.size();
        m_mutex.unlock();
        return size;
    }
}
#endif // ZQ_MESSAGEQUEUE_H