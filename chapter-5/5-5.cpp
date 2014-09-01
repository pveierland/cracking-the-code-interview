#include "bit.hpp"

#include <gmock/gmock.h>
using namespace ::testing;

template <typename T>
inline constexpr
T get_hamming_distance(const T a, const T b)
{
  return bit::count(a ^ b);
}

TEST(get_hamming_distance, validate)
{
  ASSERT_EQ(2, get_hamming_distance(0b11101, 0b01111));
}
