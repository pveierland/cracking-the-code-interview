#include <cstdint>
#include <gmock/gmock.h>

using namespace ::testing;

std::uint32_t insert_bit_pattern(const std::uint32_t N,
                                 const std::uint32_t M,
                                 const int i,
                                 const int j)
{
  const std::uint32_t mask = ((1 << ((j - i) + 1)) - 1) << i;
  return (N & ~mask) | ((M << i) & mask);
}

TEST(insert_bit_pattern, what)
{
  EXPECT_THAT(insert_bit_pattern(0b10000000000, 0b10011, 2, 6),
              Eq(0b10001001100));
}
