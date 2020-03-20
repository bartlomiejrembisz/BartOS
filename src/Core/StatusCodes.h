#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#include "CoreDefs.h"

#define STATUS_CODE_LIST(d) \
    d(0, SUCCESS, "success")   \
    d(1, FAILURE, "failure")

#define DEFINE_STATUS_CODE(statusNum, statusCode, statusCodeStr) const StatusCode JOIN(STATUS_CODE_, statusCode) = statusNum;


namespace ROS
{

using StatusCode = uint8_t;

STATUS_CODE_LIST(DEFINE_STATUS_CODE)

} // namespace ROS

#endif // STATUS_CODES_H