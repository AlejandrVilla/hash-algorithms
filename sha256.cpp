#include <iostream>
#include <chrono>
#include <ctime>
#include "sha256.hpp"
#include <fstream>

int main(int argc, char ** argv) 
{
	std::string dir = argv[1];
	std::ifstream file(dir);

	std::string text;
	std::getline(file, text);

	for(int i = 1 ; i < argc ; i++) 
	{
		SHA256 sha;
		sha.update(text);
		std::array<uint8_t, 32> digest = sha.digest();

		std::cout << SHA256::toString(digest) << std::endl;
	}

	return EXIT_SUCCESS;
}