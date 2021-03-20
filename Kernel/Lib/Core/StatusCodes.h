#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#include "CoreDefs.h"

#define STATUS_CODE_LIST(d) \
    d(0, SUCCESS,           "success")    \
    d(1, FAILURE,           "failure")    \
    d(2, INVALID_PARAMETER, "invalid parameter")    \
    d(3, ALREADY_MAPPED,    "already mapped")   \
    d(4, RESERVED,          "reserved") \
    d(5, NOT_PRESENT,       "not present")  \
    d(6, NOT_FOUND,         "not found")

#define DEFINE_STATUS_CODE(statusNum, statusCode, statusCodeStr) const StatusCode JOIN(STATUS_CODE_, statusCode) = statusNum;

#define DEFINE_STATUS_CODE_STRING(statusNum, statusCode, statusCodeStr) statusCodeStr,

namespace BartOS
{

using StatusCode = uint8_t;

STATUS_CODE_LIST(DEFINE_STATUS_CODE)

inline const char *StatusCodeToString(const StatusCode statusCode)
{
    static const char *pStatusCodeStringList[] = { STATUS_CODE_LIST(DEFINE_STATUS_CODE_STRING) };

    return pStatusCodeStringList[statusCode];
}

} // namespace BartOS

#endif // STATUS_CODES_H