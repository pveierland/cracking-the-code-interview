#include "bit.hpp"

#include "gmock/gmock.h"
using namespace ::testing;

template <typename T>
inline constexpr
T swap_even_with_odd_bits(const T value)
{
  return ((value << 1) & bit::even_pattern<T>()) |
         ((value >> 1) & bit::odd_pattern<T>());
}

TEST(swap_even_with_odd_bits, verify)
{
  EXPECT_EQ(0b00001100U, swap_even_with_odd_bits(0b00001100U));
  EXPECT_EQ(0b00001100U, swap_even_with_odd_bits(0b00001100U));
  EXPECT_EQ(0b00000011U, swap_even_with_odd_bits(0b00000011U));
  EXPECT_EQ(0b00001001U, swap_even_with_odd_bits(0b00000110U));
  EXPECT_EQ(0b01010101U, swap_even_with_odd_bits(0b10101010U));
  EXPECT_EQ(0b10101010U, swap_even_with_odd_bits(0b01010101U));
}
