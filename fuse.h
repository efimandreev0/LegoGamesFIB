#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include <Windows.h>
#include "utils.h"
#include "guess.h"
#include "compression.h"

struct Header {
	char magic[8]; //0x0, [8], FUSE1.00
	uint32_t count; //0x8, [4]
	uint32_t field0c; //0x0c, [4]
	uint32_t offset; //0x10, [4], toc offset
};
struct Entry {
	uint32_t CRC; //0x0, [4], CRC32 maybe...
	uint32_t offset; //0x4, [4]
	uint32_t flagNSize; //0x8, [4], Size >> 5, flag & 3
};
enum Compression {
	None, //0
	Refpack, //1
	Unknown1, //2
	Inflate //3
};
namespace fs = std::filesystem;
class fuse
{
public:
	static void read(std::string file);
	static void write(std::string file);
	static void list(std::string file);
};

