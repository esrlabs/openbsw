// Copyright 2025 Accenture.

#include "doip/common/DoIpVehicleIdentificationRequestSendJob.h"

#include "doip/common/DoIpConstants.h"
#include "doip/common/DoIpHeader.h"

#include <estd/array.h>
#include <estd/memory.h>

#include <gmock/gmock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

class DoIpVehicleIdentificationRequestSendJobTest : public Test
{
public:
    MOCK_METHOD2(released, void(IDoIpSendJob&, bool));
};

TEST_F(DoIpVehicleIdentificationRequestSendJobTest, TestAll)
{
    DoIpVehicleIdentificationRequestSendJob cut(
        DoIpVehicleIdentificationRequestSendJob::ReleaseCallbackType ::create<
            DoIpVehicleIdentificationRequestSendJobTest,
            &DoIpVehicleIdentificationRequestSendJobTest::released>(*this));
    ASSERT_EQ(2U, cut.getSendBufferCount());
    ASSERT_EQ(uint16_t(DoIpConstants::DOIP_HEADER_LENGTH), cut.getTotalLength());
    uint8_t const expectedHeader[] = {0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
    ::estd::array<uint8_t, 8U> staticBuffer;
    ::estd::slice<uint8_t const> sendBuffer = cut.getSendBuffer(staticBuffer, 0U);
    EXPECT_EQ(staticBuffer.data(), sendBuffer.data());
    EXPECT_TRUE(::estd::memory::is_equal(::estd::slice<uint8_t const>(expectedHeader), sendBuffer));
    // return another buffer
    EXPECT_TRUE(::estd::memory::is_equal(
        ::estd::slice<uint8_t const>(), cut.getSendBuffer(staticBuffer, 1U)));
    // release
    EXPECT_CALL(*this, released(Ref(cut), true));
    cut.release(true);
}

} // namespace test
} // namespace doip
