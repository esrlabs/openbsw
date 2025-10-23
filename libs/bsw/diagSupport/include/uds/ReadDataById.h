#ifndef GUARD_CB229E4D_1D5E_456C_8929_DFFE5DC5EB6D
#define GUARD_CB229E4D_1D5E_456C_8929_DFFE5DC5EB6D

#include "diag/IDataHandler.h"
#include "diag/IReadDataRequest.h"

#include <uds/async/AsyncDiagJobHelper.h>
#include <uds/base/AbstractDiagJob.h>
#include <uds/connection/IncomingDiagConnection.h>

#include <estd/object_pool.h>

namespace uds
{

class ReadDataById : public AbstractDiagJob
{
protected:
    class Request : public ::diag::IReadDataRequest
    {
    public:
        Request(
            ReadDataById& parent,
            IncomingDiagConnection& connection,
            PositiveResponse& positiveResponse,
            uint16_t dataId)
        : _parent(parent)
        , _connection(connection)
        , _positiveResponse(positiveResponse)
        , _dataId(dataId)
        {}

        uint16_t dataId() const override { return _dataId; }

        void sendResponse(uint8_t responseCode) override
        {
            _parent.sendResponse(*this, responseCode);
        }

        void appendData(::estd::slice<uint8_t const> data) override
        {
            _positiveResponse.appendData(data.data(), data.size());
        }

        IncomingDiagConnection& connection() { return _connection; }

    private:
        ReadDataById& _parent;
        IncomingDiagConnection& _connection;
        PositiveResponse& _positiveResponse;
        uint16_t _dataId;
    };

public:
    using ReadDataHandlerType = ::diag::IDataHandler<::diag::IReadDataRequest>;

    ReadDataById(ReadDataHandlerType& readDataHandler, ::estd::object_pool<Request>& requestPool);

protected:
    DiagReturnCode::Type
    verify(uint8_t const* const request, uint16_t const requestLength) override;

    DiagReturnCode::Type process(
        IncomingDiagConnection& connection,
        uint8_t const request[],
        uint16_t requestLength) override;

private:
    using ResultType = typename ReadDataHandlerType::Result;

    void sendResponse(Request& request, uint8_t responseCode);

    ReadDataHandlerType& _readDataHandler;
    ::estd::object_pool<Request>& _requestPool;
};

namespace declare
{

template<size_t N>
class ReadDataById : public ::uds::ReadDataById
{
public:
    static constexpr size_t REQUEST_POOL_SIZE = N;

    ReadDataById(::diag::IDataHandler<::diag::IReadDataRequest>& readDataHandler)
    : ::uds::ReadDataById(readDataHandler, _requestPool)
    {
        static_assert(N > 0, "N must be greater than 0");
    }

private:
    ::estd::declare::object_pool<Request, N> _requestPool;
};

} // namespace declare

} // namespace uds

#endif // GUARD_CB229E4D_1D5E_456C_8929_DFFE5DC5EB6D
