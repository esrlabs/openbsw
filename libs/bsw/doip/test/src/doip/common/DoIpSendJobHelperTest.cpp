// Copyright 2025 Accenture.

#include "doip/common/DoIpSendJobHelper.h"

#include <estd/array.h>
#include <estd/memory.h>

#include <gmock/gmock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

TEST(DoIpSendJobTest, TestPrepareHeaderBuffer)
{
    {
        ::estd::array<uint8_t, 8U> destBuffer;
        uint8_t expectedHeader[] = {0x03, 0xfc, 0x80, 0x02, 0x00, 0x00, 0x02, 0x10};
        EXPECT_TRUE(::estd::memory::is_equal(
            expectedHeader,
            DoIpSendJobHelper::prepareHeaderBuffer(destBuffer, 0x03, 0x8002U, 0x210)));
    }
    {
        ::estd::AssertHandlerScope scope(::estd::AssertExceptionHandler);
        ::estd::array<uint8_t, 7> destBuffer;
        EXPECT_THROW(
            DoIpSendJobHelper::prepareHeaderBuffer(destBuffer, 0x03, 0x8002U, 0x210),
            ::estd::assert_exception);
    }
}

} // namespace test
} // namespace doip
