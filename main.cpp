// FUSE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "fuse.h"
#include "utils.h"

bool isDirectory(const std::string& path) {
	std::filesystem::path filePath(path);

	if (std::filesystem::exists(filePath)) {
		return std::filesystem::is_directory(filePath);
	}

	return false; // Если путь не существует, тоже возвращаем false
}


int main(int argc, char** argv) {
	if (argc > 1) {
		if (isDirectory(argv[1])) {
			fuse::write(argv[1]);
		}
		else {
			fuse::read(argv[1]);
		}
	}
	else {
		std::cout << "USAGE: Drag N Drop .fib archive to unpack and folder to pack.";
	}
	//fuse::read("C:\\Users\\five\\source\\repos\\FUSE\\lego_emmet_3ds.fib");
	//fuse::write("C:\\Users\\five\\source\\repos\\FUSE\\lego_emmet_3ds");
	/*std::string magic = utils::getmagic(argv[1]);
	if (magic == "FUSE") {
		fuse::read(argv[1]);
	}
	else if (magic == "LOCA") {

	}*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
