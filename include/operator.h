#ifndef INCLUDE_OPERATOR_H_
#define INCLUDE_OPERATOR_H_

#include <include/cdr_request_queue.h>
#include <include/config_reader.h>
#include <include/logger.h>
#include <include/cdr.h>

class Operator {
public:
    Operator();

private:
    void Start();
};

#endif // INCLUDE_OPERATOR_H_