#include "utils.h"
std::string utils::getPath(const std::string& filePath) {
    std::size_t lastDotPos = filePath.find_last_of('.');
    return filePath.substr(0, lastDotPos);

}
int utils::findIndex(std::vector<std::string>& hashes, const std::string& target) {
    auto it = std::find(hashes.begin(), hashes.end(), target);
    if (it != hashes.end()) {
        return std::distance(hashes.begin(), it);
    }
    return -1;
}
std::vector<std::string> utils::readAllLines(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;

    if (!file) {
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}
bool utils::createDirectory(const std::string& path) {
#ifdef _WIN32
    if (_mkdir(path.c_str()) == 0 || errno == EEXIST) {
        return true;
    }
#else
    if (mkdir(path.c_str(), 0777) == 0 || errno == EEXIST) {
        return true;
    }
#endif
    return false;
}
std::string utils::getmagic(const std::string& file) {
    char magic[5];
    std::ifstream f(file, std::ios::binary);
    f.read(magic, 4);
    magic[4] = '\0';
    f.close();
    return magic;
}
void utils::createPath(const std::string& filePath) {
    std::string path = filePath;
    std::size_t lastSlash = path.find_last_of('/');

    if (lastSlash != std::string::npos) {
        path = path.substr(0, lastSlash);
    }
    else {
        return;
    }

    std::stringstream ss(path);
    std::string directory;
    std::string currentPath;

    while (std::getline(ss, directory, '/')) {
        if (!directory.empty()) {
            currentPath += directory + "/";
            createDirectory(currentPath);
        }
    }
}
std::string utils::getRelativePath(const fs::path& path, const fs::path& basePath) {
    fs::path relative = fs::relative(path, basePath);
    return relative.string(); // Convert to string
}
std::vector<std::string> utils::getFiles(const std::string& folder) {
    std::vector<std::string> fileList;

    fs::path pathToFolder(folder);

    try {
        if (fs::exists(pathToFolder) && fs::is_directory(pathToFolder)) {
            for (const auto& entry : fs::recursive_directory_iterator(pathToFolder)) {
                if (fs::is_regular_file(entry)) {
                    fileList.push_back(entry.path().string()); // Добавление пути к файлу в список
                }
            }
        }
        else {
            std::cerr << "Папка не существует или не является директорией: " << folder << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return fileList; // Возврат списка файлов
}
std::vector<std::string> utils::getFullPaths(const std::string& directory) {
    std::vector<std::string> fullPaths;
    std::string search_path = directory + "\\*";

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(search_path.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName = findFileData.cFileName;
            if (fileName != "." && fileName != "..") {
                std::string fullPath = directory + "\\" + fileName;
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    fullPath += "\\";
                }

                fullPaths.push_back(fullPath);
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    return fullPaths;
}
std::vector<std::string> utils::trimPaths(const std::vector<std::string>& paths, size_t startPos) {
    std::vector<std::string> trimmedPaths; // Вектор для хранения обрезанных путей

    for (const auto& path : paths) {
        if (startPos < path.length()) {
            trimmedPaths.push_back(path.substr(startPos)); // Обрезаем строку и добавляем в новый вектор
        }
        else {
            trimmedPaths.push_back(""); // Если startPos больше или равен длине, добавляем пустую строку
        }
    }

    return trimmedPaths; // Возвращаем новый вектор
}
std::vector<char> utils::readFileBytes(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file: " + filePath);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Error reading file: " + filePath);
    }

    return buffer;
}
bool utils::contains(const std::string& fullString, const std::string& substring) {
    return fullString.find(substring) != std::string::npos;
}
std::string utils::getFileNameWithoutExtension(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("\\/");
    size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    size_t lastDot = filePath.find_last_of('.');
    size_t end = (lastDot == std::string::npos) ? filePath.size() : lastDot;

    return filePath.substr(start, end - start);
}
uint32_t utils::hexStringToUInt32(const std::string& hexStr) {
    uint32_t value;
    std::stringstream ss;

    if (hexStr.compare(0, 2, "0x") == 0) {
        ss << std::hex << hexStr.substr(2);
    }
    else {
        ss << std::hex << hexStr;
    }

    ss >> value;
    return value;
}
std::string utils::replace(const std::string& input) {
    std::string result = input;
    for (size_t i = 0; i < result.size(); ++i) {
        if (result[i] == '\\') {
            result[i] = '/';
        }
    }
    return result;
}

void utils::writeStringsToFile(const std::vector<std::string>& lines, const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& line : lines) {
        outFile << line << std::endl;
    }

    outFile.close();
    std::cout << "Successfully written to file: " << filename << std::endl;
}