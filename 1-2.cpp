#include <cstring>
#include <iostream>
#include <utility>

char* reverse(char* input)
{
	const std::size_t length = std::strlen(input);
	
	for (std::size_t i = 0; i < length / 2; i++)
	{
		std::swap(input[i], input[length - i - 1]);
	}
	
	return input;
}

int main() {
	char text[] = "Hello World!";
	std::cout << reverse(text) << std::endl;
	return 0;
}
