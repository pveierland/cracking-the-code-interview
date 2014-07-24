#include <array>
#include <cstring>
#include <iostream>
#include <limits>
#include <utility>

bool is_permutation(const std::string& a, const std::string& b)
{
	if (a.length() != b.length()) return false;
	
	std::array<int, std::numeric_limits<unsigned char>::max()> lookup{};
	
	for (const unsigned char c : a)
	{
		++lookup[c];
	}
	
	for (const unsigned char c : b)
	{
		if (lookup[c] > 0)
		{
			--lookup[c];
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

int main()
{
	std::cout << is_permutation("alpha", "bravo") << std::endl;
	std::cout << is_permutation("charlie", "delta") << std::endl;
	std::cout << is_permutation("rocket boys", "october sky") << std::endl;
	std::cout << is_permutation("doctorwho", "torchwood") << std::endl;
}
