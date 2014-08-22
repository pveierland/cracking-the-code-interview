#include <cstddef>
#include <limits>
#include <type_traits>

namespace bitmanip
{
  // Log2 from: http://stackoverflow.com/a/18233009
  template<std::size_t N, std::size_t P = 0>
  inline constexpr typename std::enable_if<(N <= 1), std::size_t>::type Log2()
  {
    return P;
  }

  template<std::size_t N, std::size_t P = 0>
  inline constexpr typename std::enable_if<!(N <= 1), std::size_t>::type Log2()
  {
    return Log2<N / 2, P + 1>();
  }

  template <typename ValueType>
  inline
  typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
  constexpr get_num_positive_bits()
  {
    return Log2<std::numeric_limits<ValueType>::max() &
              ~(std::numeric_limits<ValueType>::max() >> 1)>();
  }

  template <typename ValueType>
  inline
  typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
  get_num_set_bits(const ValueType value)
  {
    std::size_t count = 0;

    for (std::size_t i = 0; i != get_num_positive_bits<ValueType>(); ++i)
    {
      if (value & (1 << i))
      {
        ++count;
      }
    }

    return count;
  }
}
