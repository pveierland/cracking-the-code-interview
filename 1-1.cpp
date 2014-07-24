#include <array>
#include <iostream>
#include <limits>
#include <string>

bool is_characters_unique_with_lookup(const std::string& input)
{
	std::array<bool, std::numeric_limits<unsigned char>::max()> lookup{};
	
	for (const char c : input)
	{
		const unsigned char value = static_cast<unsigned char>(c);
		
		if (lookup[value])
		{
			return false;
		}
		else
		{
			lookup[value] = true;
		}
	}
	
	return true;
}

bool is_characters_unique_without_lookup(const std::string& input)
{
	const std::string::size_type length = input.length();
	
	for (std::string::size_type i = 0; i != length; i++)
	{
		for (std::string::size_type j = i + 1; j < length; j++)
		{
			if (input[i] == input[j])
			{
				return false;
			}
		}
	}
	
	return true;
}

int main() {
	std::cout << is_characters_unique_with_lookup("abc") << std::endl;
	std::cout << is_characters_unique_with_lookup("abcde") << std::endl;
	std::cout << is_characters_unique_with_lookup("abcdea") << std::endl;
	
	std::cout << is_characters_unique_without_lookup("abc") << std::endl;
	std::cout << is_characters_unique_without_lookup("abcde") << std::endl;
	std::cout << is_characters_unique_without_lookup("abcdea") << std::endl;
	
	return 0;
}
