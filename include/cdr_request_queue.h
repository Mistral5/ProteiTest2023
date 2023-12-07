#ifndef INCLUDE_CDR_REQUEST_QUEUE_H_
#define INCLUDE_CDR_REQUEST_QUEUE_H_

#include "cdr_request.h"

#include <list>
#include <algorithm>

class CdrRequestQueue;

class CdrRequestQueueDestroyer {
private:
    CdrRequestQueue *p_instance_;
public:
    ~CdrRequestQueueDestroyer();

    void Init(CdrRequestQueue *);
};

class CdrRequestQueue {
public:
    static CdrRequestQueue &GetInstance();

    CdrRequestQueue(const CdrRequestQueue &) = delete;

    CdrRequestQueue &operator=(CdrRequestQueue &) = delete;

    bool Contains(const CdrRequest &) const;

    bool empty();

    bool push_back(CdrRequest &);

    void pop_front();

    CdrRequest &front();

    void set_max_size(uint16_t);

private:
    static CdrRequestQueue *p_instance_;
    static CdrRequestQueueDestroyer destroyer_;

    CdrRequestQueue();

    ~CdrRequestQueue() {};

    friend class CdrRequestQueueDestroyer;

    std::list<CdrRequest> queue_;
    uint16_t curr_size_;
    uint16_t max_size_;
};

//#include <mutex>
//std::mutex g_lock;
//
//void threadFunction()
//{
//	g_lock.lock();
//
//	std::cout << "entered thread " << std::this_thread::get_id() << std::endl;
//	std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
//	std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
//
//	g_lock.unlock();
//}

#endif // INCLUDE_CDR_REQUEST_QUEUE_H_