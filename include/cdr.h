#ifndef INCLUDE_CDR_H_
#define INCLUDE_CDR_H_

#include <include/cdr_request.h>

#include <boost/date_time/posix_time/posix_time.hpp>

class Cdr : public CdrRequest {
public:
    Cdr(
            CdrRequest &cdr_request,
            boost::posix_time::ptime end_dt,
            std::string call_status,
            boost::posix_time::ptime response_dt = boost::posix_time::not_a_date_time,
            uint8_t operator_id = NULL
    );

    friend std::ostream &operator<<(std::ostream &, const Cdr &);

    boost::posix_time::ptime get_end_dt() const;

    std::string get_call_status() const;

    boost::posix_time::ptime get_response_dt() const;

    uint8_t get_operator_id() const;

private:
    boost::posix_time::ptime end_dt_;
    std::string call_status_;
    boost::posix_time::ptime response_dt_;
    uint8_t operator_id_;
};

//std::ostream& operator<<(std::ostream& os, const Cdr& d);

//startDt		Server		+	DT поступления вызова.
//callId		Server		+	Идентификатор входящего вызова(Call ID)
//phoneNumber	Server		+	Номер абонента А
//endDt			Oper?			DT завершения вызова
//callStatus	Oper?			Статус вызова(OK или причина ошибки, например timeout)
//responseDt	Oper			DT ответа оператора		(если был или пустое значение)
//operatorId	Oper			Идентификатор оператора	(если был или пустое значение) 
//callDuration	Oper			Длительность разговора	(если был или пустое значение)

#endif // INCLUDE_CDR_H_