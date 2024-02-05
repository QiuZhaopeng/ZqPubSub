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
        
        /* Pop the last message in the queue  */
        T Pop();

        /* Push a new message into the queue  */
        void Push(const T& msg);

        /* Get the size of the queue  */
        size_t Size();

        /* Pop all the messages in the queue to a vector  */
        void PopAllMessages(std::vector<T>& msgsInQueue);

        /* Pop the message in the queue that is newly received within a given timeout */
        void PopMessageWithTimeout(bool& timeOut, T& msg, int timeout_ms) { /* TODO */ };

    private:
        size_t m_maxSize;                          ///< max queue size
        std::queue<T> m_msgQueue;       ///< queue containing messages
        mutable std::mutex m_mutex;       ///< mutex for protecting queue data access
        std::condition_variable m_cv;        ///< condition variable object
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
            std::cout << "waiting for new inbound message" << std::endl;
            m_cv.wait(lock);
        }
        T data = m_msgQueue.front();
        m_msgQueue.pop();
        return data;
    }

    template<typename T> 
    void MessageQueue<T>::PopAllMessages(std::vector<T>& msgsInQueue)
    {
        std::queue<T> tempQueue;
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            tempQueue = m_msgQueue;
        }

        while (!tempQueue.empty())
        {
            msgsInQueue.push_back(tempQueue.front());
            tempQueue.pop();
            std::lock_guard<std::mutex> lg(m_mutex);
            m_msgQueue.pop();
        }
    }

    template<typename T>
    size_t MessageQueue<T>::Size()
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        auto size = m_msgQueue.size();
        return size;
    }
}
#endif // ZQ_MESSAGEQUEUE_H