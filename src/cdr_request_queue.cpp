#include "include/cdr_request_queue.h"

CdrRequestQueue *CdrRequestQueue::p_instance_ = nullptr;
CdrRequestQueueDestroyer CdrRequestQueue::destroyer_;

CdrRequestQueueDestroyer::~CdrRequestQueueDestroyer() {
    delete p_instance_;
}

void CdrRequestQueueDestroyer::Init(CdrRequestQueue *p) {
    p_instance_ = p;
}

CdrRequestQueue &CdrRequestQueue::GetInstance() {
    if (!p_instance_) {
        p_instance_ = new CdrRequestQueue();
        destroyer_.Init(p_instance_);
    }
    return *p_instance_;
}

CdrRequestQueue::CdrRequestQueue()
        : max_size_(0), curr_size_(0) {}

bool CdrRequestQueue::Contains(const CdrRequest &cdr_request) const {
    return (std::find(queue_.begin(), queue_.end(), cdr_request) != queue_.end());
}

bool CdrRequestQueue::push_back(CdrRequest &cdr_request) {
    if (curr_size_ < max_size_) {
        queue_.push_back(cdr_request);
        ++curr_size_;
        return true;
    }
    return false;
}

void CdrRequestQueue::pop_front() {
    if (curr_size_) {
        queue_.pop_front();
        --curr_size_;
    }
}

CdrRequest &CdrRequestQueue::front() {
    return queue_.front();
}

void CdrRequestQueue::set_max_size(uint16_t max_size) {
    max_size_ = max_size;
}

bool CdrRequestQueue::empty() {
    return queue_.empty();
}