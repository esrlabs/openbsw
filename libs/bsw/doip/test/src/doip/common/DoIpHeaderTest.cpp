// Copyright 2025 Accenture.

#include "doip/common/DoIpHeader.h"

#include <estd/slice.h>

#include <gtest/gtest.h>

using namespace ::testing;
using namespace ::doip;

namespace
{
DoIpHeader const& as_header(::estd::slice<uint8_t const> bytes)
{
    return bytes.reinterpret_as<DoIpHeader const>()[0];
}
} // namespace

TEST(DoIpHeaderTest, checkProtocolVersion)
{
    uint8_t const correct[] = {0x02, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    EXPECT_TRUE(checkProtocolVersion(as_header(correct), 0x02));

    uint8_t const invalidInverse[] = {0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    EXPECT_FALSE(checkProtocolVersion(as_header(invalidInverse), 0x02));

    uint8_t const correctInverseIncorrectVersion[]
        = {0x02, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    EXPECT_FALSE(checkProtocolVersion(as_header(correctInverseIncorrectVersion), 0x03));
}

TEST(DoIpHeaderTest, read_fields)
{
    uint8_t const bytes[]    = {0x02, 0xFD, 0x03, 0x75, 0x12, 0x34, 0x56, 0x78};
    DoIpHeader const& header = as_header(bytes);

    EXPECT_EQ(0x02U, header.protocolVersion);
    EXPECT_EQ(0x375U, header.payloadType);
    EXPECT_EQ(0x12345678U, header.payloadLength);
}
