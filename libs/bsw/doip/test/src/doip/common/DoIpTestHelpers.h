// Copyright 2025 Accenture.

#pragma once

#include <async/TestContext.h>
#include <tcp/socket/AbstractSocketMock.h>

#include <gmock/gmock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

class DoIpExpectSendHelper
{
public:
    template<size_t N>
    DoIpExpectSendHelper(::tcp::AbstractSocketMock& socketMock, uint8_t const (&message)[N])
    : _socketMock(socketMock), _message(message), _size(N), _unsentSize(N)
    {}

    Sequence& getSequence() { return _seq; }

    DoIpExpectSendHelper& send(uint32_t size);
    DoIpExpectSendHelper& flush();
    DoIpExpectSendHelper& verify(::async::TestContext& testContext);
    void dataSentAndVerify(::async::TestContext& testContext);

private:
    Sequence _seq;
    ::tcp::AbstractSocketMock& _socketMock;
    uint8_t const* _message;
    uint32_t _size;
    uint32_t _unsentSize;
};

class DoIpExpectReadHelper
{
public:
    template<size_t N>
    DoIpExpectReadHelper(::tcp::AbstractSocketMock& socketMock, uint8_t const (&message)[N])
    : _socketMock(socketMock), _message(message), _size(N), _unreadSize(N)
    {}

    Sequence& getSequence() { return _seq; }

    DoIpExpectReadHelper& read(uint32_t size);
    DoIpExpectReadHelper& skip(uint32_t size);
    void dataReceivedAndVerify(::async::TestContext& testContext);

private:
    Sequence _seq;
    ::tcp::AbstractSocketMock& _socketMock;
    uint8_t const* _message;
    uint32_t _size;
    uint32_t _unreadSize;
};

} // namespace test
} // namespace doip
