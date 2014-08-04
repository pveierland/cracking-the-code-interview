
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <limits>
#include <random>

using namespace ::testing;

void swap(int& a, int& b)
{
  a ^= b;
  b ^= a;
  a ^= b;
}

void test_swap(const int a, const int b)
{
  int alpha = a;
  int bravo = b;

  swap(alpha, bravo);

  EXPECT_THAT(alpha, Eq(b));
  EXPECT_THAT(bravo, Eq(a));
}

TEST(swap, extremes)
{
  test_swap(0, 0);
  test_swap(-1, 1);
  test_swap(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
  test_swap(std::numeric_limits<int>::max(), std::numeric_limits<int>::min());
}

TEST(swap, random)
{
  std::default_random_engine rnd;
  std::uniform_int_distribution<> dist{std::numeric_limits<int>::min(),
                                       std::numeric_limits<int>::max()};

  for (int i = 0; i < 10000; ++i)
  {
    test_swap(dist(rnd), dist(rnd));
  }
}
