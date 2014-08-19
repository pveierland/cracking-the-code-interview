#include "gmock/gmock.h"
using namespace ::testing;

#include <iostream>
#include <type_traits>

// Log2 from: http://stackoverflow.com/a/18233009
template<std::size_t N, std::size_t P = 0>
constexpr typename std::enable_if<(N <= 1), std::size_t>::type Log2()
{
  return P;
}

template<std::size_t N, std::size_t P = 0>
constexpr typename std::enable_if<!(N <= 1), std::size_t>::type Log2()
{
  return Log2<N / 2, P + 1>();
}

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_binary_greater(const ValueType value)
{
  if (value == 0)
  {
    throw std::runtime_error("input cannot be zero");
  }

  constexpr std::size_t highest_bit =
    Log2<std::numeric_limits<ValueType>::max() &
       ~(std::numeric_limits<ValueType>::max() >> 1)>();

  for (std::size_t i = 0, first = 0; i != highest_bit; ++i)
  {
    const std::size_t bit_mask = (1 << i);
    const bool        bit_set  = value & bit_mask;

    if (!first && bit_set)
    {
      first = i + 1;
    }
    else if (first && !bit_set)
    {
      return bit_mask | (value & ~(bit_mask - 1)) | ((1 << (i - first)) - 1);
    }
  }

  throw std::runtime_error("no greater number within bound exists");
}

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_binary_lesser(const ValueType value)
{
  if (value == 0)
  {
    throw std::runtime_error("input cannot be zero");
  }

  constexpr int highest_bit =
    Log2<std::numeric_limits<ValueType>::max() &
       ~(std::numeric_limits<ValueType>::max() >> 1)>();

  for (std::size_t i = 0, zero_pos = 0; i != highest_bit; ++i)
  {
    const bool bit_set = value & (1 << i);

    if (!zero_pos)
    {
      if (!bit_set)
      {
        zero_pos = i + 1;
      }
    }
    else if (bit_set)
    {
      return (((1 << (zero_pos - 1)) - 1) << (i - zero_pos))
            | (value & ~((1 << (i + 1)) - 1))
            | (1 << (i - 1));
    }
  }

  throw std::runtime_error("no lesser number within bound exists");
}

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_num_set_bits(const ValueType value)
{
  constexpr std::size_t highest_bit =
    Log2<std::numeric_limits<ValueType>::max() &
       ~(std::numeric_limits<ValueType>::max() >> 1)>();

  std::size_t count = 0;

  for (std::size_t i = 0; i != highest_bit; ++i)
  {
    if (value & (1 << i))
    {
      ++count;
    }
  }

  return count;
}

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_binary_greater_naive(const ValueType value)
{
  const std::size_t num_set_bits = get_num_set_bits(value);

  for (ValueType i = value + 1; i != std::numeric_limits<ValueType>::max(); ++i)
  {
    if (get_num_set_bits(i) == num_set_bits)
    {
      return i;
    }
  }

  throw std::runtime_error("get_binary_greater_naive failed: no greater value exists within bounds");
}

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_binary_lesser_naive(const ValueType value)
{
  const std::size_t num_set_bits = get_num_set_bits(value);

  for (ValueType i = value - 1; i != 0; --i)
  {
    if (get_num_set_bits(i) == num_set_bits)
    {
      return i;
    }
  }

  throw std::runtime_error("get_binary_lesser_naive failed: no lesser value exists");
}

TEST(get_binary_greater, zero)
{
  ASSERT_THROW(get_binary_greater(0), std::runtime_error);
}

TEST(get_binary_greater, max)
{
  ASSERT_THROW(get_binary_greater(std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(get_binary_greater, naive)
{
  for (int i = 1; i <= 1024; ++i)
  {
    ASSERT_EQ(get_binary_greater_naive(i), get_binary_greater(i));
  }
}

TEST(get_binary_lesser, zero)
{
  ASSERT_THROW(get_binary_lesser(0), std::runtime_error);
}

TEST(get_binary_lesser, all_ones)
{
  constexpr int highest_bit =
    Log2<std::numeric_limits<int>::max() &
       ~(std::numeric_limits<int>::max() >> 1)>();

  for (int i = 1; i != highest_bit; ++i)
  {
    ASSERT_THROW(get_binary_lesser((1 << i) - 1), std::runtime_error);
  }
}

TEST(get_binary_lesser, naive)
{
  for (int i = 2; i <= 1024; i++)
  {
    const bool is_all_ones = ((i + 1) & i) == 0;

    if (!is_all_ones)
    {
      ASSERT_EQ(get_binary_lesser_naive(i), get_binary_lesser(i));
    }
  }
}

TEST(get_binary_greater_with_get_binary_lesser, test)
{
  for (int i = 1; i < 1024; ++i)
  {
    ASSERT_EQ(i, get_binary_lesser(get_binary_greater(i)));
  }
}

TEST(get_binary_lesser_with_get_binary_greater, test)
{
  for (int i = 2; i <= 1024; i++)
  {
    const bool is_all_ones = ((i + 1) & i) == 0;

    if (!is_all_ones)
    {
      ASSERT_EQ(i, get_binary_greater(get_binary_lesser(i)));
    }
  }
}
