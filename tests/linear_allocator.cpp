
#include <gmock/gmock.h>
#include <vector>

#include "allocator/linear_allocator.hpp"
#include "allocator/stl_allocator.hpp"

using namespace ::testing;
using namespace ::cpp;

template<typename T>
using Vector = std::vector<T, STLAllocator<T, LinearAllocator>>;

static constexpr std::size_t TestBegin = 0;
static constexpr std::size_t TestEnd = 1024;
static constexpr std::size_t TestLength = TestEnd - TestBegin;
static constexpr std::size_t TestBufferLength = TestLength * 1000;

class LinearAllocatorTest : public TestWithParam<std::size_t>
{
protected:
    using T = int;

    LinearAllocatorTest() :
        buffer{ new char[TestBufferLength*sizeof(T)] },
        v{LinearAllocator{begin(), end()}}
    {}

    virtual ~LinearAllocatorTest()
    {
        delete[] buffer;
    }

    std::size_t max_elements() const
    {
        return TestBufferLength;   
    }

    char* begin()
    {
        return buffer;
    }

    char* end()
    {
        return begin() + TestBufferLength*sizeof(T);
    }

    char* buffer = nullptr;
    Vector<T> v;
};

TEST_P(LinearAllocatorTest, defaultConstruction)
{
    auto f = [this]{
        Vector<int> v{LinearAllocator{begin(), end()}};
    };
    EXPECT_NO_THROW(f());
}

TEST_P(LinearAllocatorTest, pushNoThrow)
{
    for(std::size_t i = TestBegin; i < TestEnd; ++i)
    {
        SCOPED_TRACE(v.get_allocator().dump());
        EXPECT_NO_THROW(v.push_back(i));
    }
}

INSTANTIATE_TEST_CASE_P(
    LinearAllocatorMainTest,
    LinearAllocatorTest,
    Range(TestBegin, TestEnd)
);

