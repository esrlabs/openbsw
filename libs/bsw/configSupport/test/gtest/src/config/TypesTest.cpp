// Copyright 2025 Accenture.

#include "config/Types.h"

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

struct TestBaseA
{};

struct TestBaseB
{
    TestBaseB(uint32_t a, uint32_t b) : _a(a), _b(b) {}

    uint32_t _a;
    uint32_t _b;
};

struct TestService : TestBaseA
{
    TestService(TestBaseB& baseB, uint32_t c, uint32_t d)
    : _baseB(baseB), _c(c), _d(d) {}

    TestBaseB& _baseB;
    uint32_t _c;
    uint32_t _d;
};

struct TestUnused
{};

TEST(TypesTest, testBasics)
{
    using CutType = ::config::Types<TestService, TestBaseA>;
    EXPECT_EQ(2, size_t(CutType::Sizeof::value));
    EXPECT_EQ(0, size_t(CutType::template IndexOf<TestService>::value));
    EXPECT_EQ(1, size_t(CutType::template IndexOf<TestBaseA>::value));
    EXPECT_EQ(2, size_t(CutType::template IndexOf<TestUnused>::value));
    EXPECT_TRUE(bool(::std::is_same<CutType::template TypeOf<0>::Type, TestService>::value));
    EXPECT_TRUE(bool(::std::is_same<CutType::template TypeOf<1>::Type, TestBaseA>::value));
    EXPECT_TRUE(bool(CutType::template IsContained<TestService>::value));
    EXPECT_TRUE(bool(CutType::template IsContained<TestBaseA>::value));
    EXPECT_FALSE(bool(CutType::template IsContained<TestBaseB>::value));
}

TEST(TypesTest, testIndicesOf)
{
    using CutType = ::config::Types<uint8_t, uint8_t, int8_t, int8_t, uint8_t>;
    EXPECT_TRUE(bool(::std::is_same<::std::index_sequence<0, 1, 4>, typename CutType::template IndicesOf<uint8_t>::Type>::value));
}

TEST(TypesTest, testIsUnique)
{
    using UniqueType = ::config::Types<TestService, TestBaseA>;
    EXPECT_TRUE(bool(UniqueType::IsUnique::value));
    using NonUniqueType = ::config::Types<TestService, TestBaseA, TestService>;
    EXPECT_FALSE(bool(NonUniqueType::IsUnique::value));
}

TEST(TypesTest, testAppend)
{
    using HeadType = ::config::Types<TestService, TestBaseA>;
    using AppendType = ::config::Types<TestBaseB>;
    using CombinedType = HeadType::template Append<AppendType>::Type;
    EXPECT_EQ(3, size_t(CombinedType::Sizeof::value));
    EXPECT_EQ(0, size_t(CombinedType::template IndexOf<TestService>::value));
    EXPECT_EQ(1, size_t(CombinedType::template IndexOf<TestBaseA>::value));
    EXPECT_EQ(2, size_t(CombinedType::template IndexOf<TestBaseB>::value));
    EXPECT_EQ(3, size_t(CombinedType::template IndexOf<TestUnused>::value));
}

template<typename T>
struct TestPred
{
    static constexpr bool value = std::is_base_of<TestBaseA, T>::value;
};

TEST(TypesTest, testFilter)
{
    using FullType = ::config::Types<TestService, TestBaseA, TestBaseB>;
    using FilteredType = typename FullType::template Filter<TestPred>::Type;
    EXPECT_EQ(2, size_t(FilteredType::Sizeof::value));
    EXPECT_EQ(0, size_t(FilteredType::template IndexOf<TestService>::value));
    EXPECT_EQ(1, size_t(FilteredType::template IndexOf<TestBaseA>::value));
    EXPECT_EQ(2, size_t(FilteredType::template IndexOf<TestBaseB>::value));
}

template<typename... Ts>
struct TestClass
{};

TEST(TypesTest, testClassed)
{
    using ClassedType = typename ::config::Types<TestService, TestBaseA>::template Classed<TestClass>::Type;
    EXPECT_TRUE(bool(::std::is_same<ClassedType, TestClass<TestService, TestBaseA>>::value));
}

TEST(TypesTest, testClassedArgs)
{
    using ClassedArgsType = typename ::config::Types<TestService, TestBaseA>::template ClassedArgs<TestClass>::Type;
    EXPECT_TRUE(bool(::std::is_same<ClassedArgsType, ::config::Types<TestClass<TestService>, TestClass<TestBaseA>>>::value));
}

} // namespace
