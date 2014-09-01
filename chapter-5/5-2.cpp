#include "gmock/gmock.h"
using namespace ::testing;

#include <cmath>
#include <iostream>

std::string get_binary_representation(double value)
{
  constexpr auto max_iterations = 32;
  auto iteration_count = 0;

  std::string output{};
  output.reserve(max_iterations);

  output = "0.";

  do
  {
    const auto divisor = std::exp2(-(iteration_count + 1));

    if (value >= divisor)
    {
      output += '1';
      value -= divisor;
    }
    else
    {
      output += '0';
    }

    if (iteration_count != max_iterations - 2)
    {
      iteration_count++;
    }
    else
    {
      return "ERROR";
    }
  } while (value != 0.0);

  return output;
}

TEST(print_binary_representation, validate)
{
  EXPECT_THAT(get_binary_representation(0.750), Eq("0.11"));
  EXPECT_THAT(get_binary_representation(0.500), Eq("0.1"));
  EXPECT_THAT(get_binary_representation(0.250), Eq("0.01"));
  EXPECT_THAT(get_binary_representation(0.125), Eq("0.001"));

  EXPECT_THAT(get_binary_representation(0.72), Eq("ERROR"));
  EXPECT_THAT(get_binary_representation(M_PI), Eq("ERROR"));
}
