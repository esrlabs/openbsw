// Copyright 2025 Accenture.

#include "doip/server/DoIpServerTransportLayerParameters.h"

#include <gmock/gmock.h>

namespace doip
{
namespace test
{
using namespace ::testing;

TEST(DoIpServerTransportLayerParametersTest, TestAll)
{
    DoIpServerTransportLayerParameters cut(
        1234U,    // initialInactivityTimeout
        5689123U, // generalInactivityTimeout
        3432U,    // aliveCheckTimeout
        456789U   // maxPayloadLength
    );
    ASSERT_EQ(1234U, cut.getInitialInactivityTimeout());
    ASSERT_EQ(5689123U, cut.getGeneralInactivityTimeout());
    ASSERT_EQ(3432U, cut.getAliveCheckTimeout());
    ASSERT_EQ(456789U, cut.getMaxPayloadLength());
}

} // namespace test
} // namespace doip
