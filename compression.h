#pragma once
#include <vector>
class compression
{
public:
	static std::vector<uint8_t> refpack(const std::vector<uint8_t>& srcBuffer);
};

