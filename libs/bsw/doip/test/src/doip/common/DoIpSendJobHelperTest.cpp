// Copyright 2025 Accenture.

#include "doip/common/DoIpSendJobHelper.h"
#include "doip/common/DoIpTestHelpers.h"

#include <etl/array.h>
#include <util/estd/assert.h>

#include <gmock/gmock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

TEST(DoIpSendJobTest, TestPrepareHeaderBuffer)
{
    {
        ::etl::array<uint8_t, 8U> destBuffer;
        uint8_t expectedHeader[] = {0x03, 0xfc, 0x80, 0x02, 0x00, 0x00, 0x02, 0x10};
        EXPECT_TRUE(is_equal(
            expectedHeader,
            DoIpSendJobHelper::prepareHeaderBuffer(destBuffer, 0x03, 0x8002U, 0x210)));
    }
    {
        ::estd::AssertHandlerScope scope(::estd::AssertExceptionHandler);
        ::etl::array<uint8_t, 7> destBuffer;
        EXPECT_THROW(
            DoIpSendJobHelper::prepareHeaderBuffer(destBuffer, 0x03, 0x8002U, 0x210),
            ::estd::assert_exception);
    }
}

} // namespace test
} // namespace doip
