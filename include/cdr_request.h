#ifndef INCLUDE_CDR_REQUEST_H_
#define INCLUDE_CDR_REQUEST_H_

#include <boost/date_time/posix_time/posix_time.hpp>

class CdrRequest {
public:
    CdrRequest(boost::posix_time::ptime, uint32_t, uint64_t);

    CdrRequest(const CdrRequest &);

    bool operator!=(const CdrRequest &) const;

    bool operator==(const CdrRequest &) const;

    boost::posix_time::ptime get_start_dt() const;

    uint32_t get_call_id() const;

    uint64_t get_phone_number() const;

protected:
    boost::posix_time::ptime start_dt_;
    uint32_t call_id_;
    uint64_t phone_number_;
};

#endif // INCLUDE_CDR_REQUEST_H_