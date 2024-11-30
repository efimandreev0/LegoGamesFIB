#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
class guess
{
public:
	static std::string ext(const std::vector<uint8_t>& data, bool isDir = false);
};

