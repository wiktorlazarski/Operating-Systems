#include <iostream>
#include <fstream>

int main()
{
	std::ofstream file;
	file.open("commands");
	
	const int page_size = 16384;
	for(int i = 0; i < 64; i++){
		file << "READ " << i * page_size << std::endl;
	}

	file.close();
}
