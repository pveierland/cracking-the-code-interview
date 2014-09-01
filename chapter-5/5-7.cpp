
#include "bit.hpp"

#include "gmock/gmock.h"
using namespace ::testing;

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

namespace
{
  template <typename T>
  class problem_data
  {
  public:
    problem_data(const T N)
      : data_(N + 1)
    {
      std::iota(data_.begin(), data_.end(), 0);
      std::shuffle(data_.begin(), data_.end(),
        std::default_random_engine{std::random_device{}()});

      missing_ = data_.back();
      data_.pop_back();
    }

    T access(const T i, const unsigned int j) const noexcept
    {
      return data_[i] & (T{1} << j);
    }

    T missing() const noexcept
    {
      return missing_;
    }

    T N() const noexcept
    {
      return data_.size();
    }

    auto size() const noexcept
    {
      return data_.size();
    }

  private:
    std::vector<T> data_;
    T              missing_;
  };
}

template <typename T>
auto find_missing_sequence_element(const problem_data<T>& input)
{
  const auto input_width = bit::width(input.N());
  std::vector<int> counters(input_width);

  for (std::uint32_t i = 0; i != input.size(); ++i)
  {
    for (unsigned int j = 0; j != input_width; ++j)
    {
      counters[j] += input.access(i, j) ? 1 : -1;
    }
  }

  std::uint32_t result = 0;

  for (unsigned int j = 0; j != input_width; ++j)
  {
    const int expected_remainder = (input.N() + 1) % (1 << (j + 1));    
    const int expected_num_zero  = std::min(1 << j, expected_remainder);
    const int expected_num_ones  = expected_remainder - expected_num_zero;
    const int diff               = counters[j] + expected_num_zero - expected_num_ones;

    if (diff == -1)
    {
      result |= 1 << j;
    }
  }

  if (result != input.missing())
  {
    throw std::logic_error("find_missing_sequence_element logic failed");
  }

  return result;
}

TEST(find_missing_sequence_element, validates)
{
  for (int i = 1; i <= 1000; ++i)
  {
    problem_data<int> data{i};
    find_missing_sequence_element(data);
  }
}
