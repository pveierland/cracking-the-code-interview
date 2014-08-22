#include "bitmanip.h"

#include <gmock/gmock.h>
using namespace ::testing;

#include <type_traits>

template <typename ValueType>
typename std::enable_if<std::is_integral<ValueType>::value, ValueType>::type
get_hamming_distance(const ValueType a, const ValueType b)
{
	return bitmanip::get_num_set_bits(a ^ b);
}

TEST(get_hamming_distance, validate)
{
	ASSERT_EQ(2, get_hamming_distance(0b11101, 0b01111));
}
