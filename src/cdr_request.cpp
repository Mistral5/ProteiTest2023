#include <include/cdr_request.h>

CdrRequest::CdrRequest(boost::posix_time::ptime start_dt, uint32_t call_id, uint64_t phone_number)
        : start_dt_(start_dt), call_id_(call_id), phone_number_(phone_number) {}

CdrRequest::CdrRequest(const CdrRequest &cdr_request)
        : start_dt_(cdr_request.start_dt_), call_id_(cdr_request.call_id_), phone_number_(cdr_request.phone_number_) {}

bool CdrRequest::operator==(const CdrRequest &cdr_request) const {
    return cdr_request.phone_number_ == phone_number_;
}

bool CdrRequest::operator!=(const CdrRequest &cdr_request) const {
    return cdr_request.phone_number_ != phone_number_;
}

boost::posix_time::ptime CdrRequest::get_start_dt() const {
    return start_dt_;
}

uint32_t CdrRequest::get_call_id() const {
    return call_id_;
}

uint64_t CdrRequest::get_phone_number() const {
    return phone_number_;
}