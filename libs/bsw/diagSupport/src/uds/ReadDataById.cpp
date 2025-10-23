#include "uds/ReadDataById.h"

#include <estd/big_endian.h>

namespace uds
{
ReadDataById::ReadDataById(
    ReadDataHandlerType& readDataHandler, ::estd::object_pool<Request>& requestPool)
: AbstractDiagJob(nullptr, 0U, 0U, DiagSession::ALL_SESSIONS())
, _readDataHandler(readDataHandler)
, _requestPool(requestPool)
{}

DiagReturnCode::Type
ReadDataById::verify(uint8_t const* const request, uint16_t const requestLength)
{
    if ((requestLength < 1U) || (request[0] != ServiceId::READ_DATA_BY_IDENTIFIER)
        || (requestLength > 3U))
    {
        return DiagReturnCode::NOT_RESPONSIBLE;
    }
    return DiagReturnCode::OK;
}

DiagReturnCode::Type ReadDataById::process(
    IncomingDiagConnection& connection, uint8_t const request[], uint16_t requestLength)
{
    if (requestLength != 3U)
    {
        return DiagReturnCode::ISO_INVALID_FORMAT;
    }
    if (_requestPool.empty())
    {
        return DiagReturnCode::ISO_BUSY_REPEAT_REQUEST;
    }
    uint16_t const dataId              = ::estd::read_be<uint16_t>(request + 1U);
    PositiveResponse& positiveResponse = connection.releaseRequestGetResponse();
    positiveResponse.appendUint8(ServiceId::READ_DATA_BY_IDENTIFIER);
    positiveResponse.appendUint16(dataId);
    ::async::ModifiableLockType lock;
    Request& req = _requestPool.allocate().construct(*this, connection, positiveResponse, dataId);
    lock.unlock();
    ResultType result = _readDataHandler.handleRequest(req, 0U);
    if (result == ResultType::OK)
    {
        return DiagReturnCode::OK;
    }
    lock.lock();
    _requestPool.release(req);
    return (result == ResultType::BUSY) ? DiagReturnCode::ISO_BUSY_REPEAT_REQUEST
                                        : DiagReturnCode::ISO_REQUEST_OUT_OF_RANGE;
}

void ReadDataById::sendResponse(Request& request, uint8_t responseCode)
{
    IncomingDiagConnection& connection = request.connection();
    ::async::ModifiableLockType lock;
    _requestPool.release(request);
    lock.unlock();
    if (responseCode == Request::ResponseCode::OK)
    {
        connection.sendPositiveResponse(*this);
    }
    else
    {
        connection.sendNegativeResponse(responseCode, *this);
    }
}

} // namespace uds
