#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <direct.h>
#include <Windows.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

namespace fs = std::filesystem;

class utils
{
public:
	static std::vector<std::string> readAllLines(const std::string& filename);
	static void createPath(const std::string& filePath);
	static std::string getPath(const std::string& filePath);
	static int findIndex(std::vector<std::string>& hashes, const std::string& target);
	static std::string getmagic(const std::string& file);
	static std::vector<std::string> getFiles(const std::string& folder);
	static std::vector<std::string> getFullPaths(const std::string& directory);
	static std::vector<std::string> trimPaths(const std::vector<std::string>& paths, size_t startPos);
	static std::vector<char> readFileBytes(const std::string& filePath);
	static bool contains(const std::string& fullString, const std::string& substring);
	static std::string getFileNameWithoutExtension(const std::string& filePath);
	static uint32_t hexStringToUInt32(const std::string& hexStr);
	static std::string replace(const std::string& input);
	static void writeStringsToFile(const std::vector<std::string>& lines, const std::string& filename);
private:
	static bool createDirectory(const std::string& path);
	static std::string getRelativePath(const fs::path& path, const fs::path& basePath);
};

