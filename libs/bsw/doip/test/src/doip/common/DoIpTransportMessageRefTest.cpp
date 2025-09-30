// Copyright 2025 Accenture.

#include "doip/common/DoIpTransportMessageRef.h"

#include <transport/TransportMessage.h>
#include <transport/TransportMessageProcessedListenerMock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

TEST(DoIpTransportMessageRefTest, TestAll)
{
    ::transport::TransportMessage message;
    ::transport::TransportMessageProcessedListenerMock processedListenerMock;
    DoIpTransportMessageRef cut(message, &processedListenerMock, 2U);
    EXPECT_EQ(&message, &cut.getMessage());
    EXPECT_EQ(&processedListenerMock, cut.getNotificationListener());
    EXPECT_EQ(1U, cut.releaseRef());
    cut.setRef();
    cut.setRef();
    EXPECT_EQ(2U, cut.releaseRef());
    EXPECT_EQ(1U, cut.releaseRef());
    EXPECT_EQ(0U, cut.releaseRef());
}

} // namespace test
} // namespace doip
