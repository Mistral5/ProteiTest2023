#include "include/call_center.h"

Cdr::Cdr
        (
                CdrRequest &cdr_request,
                boost::posix_time::ptime end_dt,
                std::string call_status,
                boost::posix_time::ptime response_dt,
                uint8_t operator_id
        ) : CdrRequest(cdr_request),
            end_dt_(end_dt),
            call_status_(call_status),
            response_dt_(response_dt),
            operator_id_(operator_id) {}

boost::posix_time::ptime Cdr::get_end_dt() const {
    return end_dt_;
}

std::string Cdr::get_call_status() const {
    return call_status_;
}

boost::posix_time::ptime Cdr::get_response_dt() const {
    return response_dt_;
}

uint8_t Cdr::get_operator_id() const {
    return operator_id_;
}

std::ostream &operator<<(std::ostream &stream, const Cdr &cdr) {

    const boost::date_time::time_facet<boost::posix_time::ptime, char> *time_facet_ =
            new boost::date_time::time_facet<boost::posix_time::ptime, char>("%Y-%m-%d %H:%M:%S.%f");

    stream.imbue(std::locale(std::locale::classic(), time_facet_));

    stream << cdr.start_dt_
           << ',' << cdr.call_id_
           << ',' << cdr.phone_number_
           << ',' << cdr.end_dt_
           << ',' << cdr.call_status_;

    if (!cdr.response_dt_.is_not_a_date_time())
        stream << ',' << cdr.response_dt_
               << ',' << (unsigned) cdr.operator_id_
               << ',' << cdr.end_dt_ - cdr.response_dt_;

    return stream;
}
