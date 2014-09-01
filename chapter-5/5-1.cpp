#include "bit.hpp"

#include "gmock/gmock.h"
using namespace ::testing;

template <typename T>
inline constexpr
T insert_bit_pattern(const T   N,
                     const T   M,
                     const int i,
                     const int j)
{
  return (N & ~bit::mask<T>(i, j)) | ((M << i) & bit::mask<T>(i, j));
}

TEST(insert_bit_pattern, validate)
{
  ASSERT_EQ(insert_bit_pattern(0b10000000000, 0b10011, 2, 6), 0b10001001100);
}
