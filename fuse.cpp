#include "fuse.h"

void fuse::read(std::string file) {
    std::vector<std::string> hashes = utils::readAllLines("hash.txt");
    std::vector<std::string> names = utils::readAllLines("names.txt");
    std::string fPath = utils::getPath(file);
    CreateDirectoryA(fPath.c_str(), NULL);
    CreateDirectoryA((fPath + "\\!NoName").c_str(), NULL);
    std::ifstream f(file, std::ios::binary);
    if (f) {
        Header h;
        f.read(reinterpret_cast<char*>(&h), sizeof(h));
        std::vector<std::string> paths(h.count);
        f.seekg(h.offset, SEEK_SET);
        std::vector<Entry> entries(h.count);
        f.read(reinterpret_cast<char*>(entries.data()), h.count * sizeof(Entry));
        for (int i = 0; i < h.count; i++) {
            
            f.seekg(entries[i].offset, SEEK_SET);
            size_t size = entries[i].flagNSize >> 5; //if compressed this is a decsize
            Compression flag = (Compression)(entries[i].flagNSize & 3); //compressed or not? == 1 refpack is standart compression of lego games. 
            //At least it or doesn't have compression, or it's refpack.
            std::list <uint8_t> bytes;
            if (flag != Compression::None) {
                bool done = false;
                while (!done) {
                    uint32_t chunk;
                    f.read(reinterpret_cast<char*>(&chunk), 4);
                    std::vector<uint8_t> tmp(chunk);
                    f.read(reinterpret_cast<char*>(tmp.data()), chunk);
                    std::vector<uint8_t> dec = compression::refpack(tmp);
                    bytes.insert(bytes.end(), dec.begin(), dec.end());
                    if (bytes.size() >= size) done = true;
                } //here we are reading and decompressing file while it size < decompressed size
            }
            else {
                std::vector<uint8_t> tmp(size);
                f.read(reinterpret_cast<char*>(tmp.data()), size);
                bytes.insert(bytes.end(), tmp.begin(), tmp.end());
            }
            std::vector<uint8_t> outputBytes(bytes.begin(), bytes.end());
            std::string path = fPath + "\\!NoName\\" + guess::ext(outputBytes, true);
            CreateDirectoryA(path.c_str(), NULL);
            std::string ext = guess::ext(outputBytes);
            std::ostringstream hash;
            hash << "0x" << std::setw(8) << std::setfill('0') << std::hex << entries[i].CRC;
            int index = utils::findIndex(hashes, hash.str());
            std::ostringstream filename;
            filename << path << "\\" << "0x" << std::setw(8) << std::setfill('0') << std::hex << entries[i].CRC << ext;
            if (index > -1) {
                filename.str("");
                filename << fPath << "/" << names[index];
            }
            paths[i] = (filename.str().substr(fPath.length() + 1));
            utils::createPath(filename.str());
            std::ofstream out(filename.str(), std::ios::binary);
            out.write(reinterpret_cast<char*>(outputBytes.data()), outputBytes.size());
            out.close();
        }
        utils::writeStringsToFile(paths, fPath + "\\FilePaths.txt");
    }
    f.close();
}

void fuse::list(std::string fib) {
    std::vector<std::string> hashes = utils::readAllLines("hash.txt");
    std::vector<std::string> names = utils::readAllLines("names.txt");
    std::ifstream f(fib, std::ios::binary);
    f.seekg(0x8);
    uint32_t count;
    f.read(reinterpret_cast<char*>(&count), 4);
    f.seekg(0x10);
    uint32_t tmp;
    f.read(reinterpret_cast<char*>(&tmp), 4);
    f.seekg(tmp, SEEK_SET);
    std::vector<Entry> entries(count);
    f.read(reinterpret_cast<char*>(entries.data()), count * sizeof(Entry));

    std::vector<std::string> paths(count);
    for (int i = 0; i < count; i++) {
        std::ostringstream hash;
        hash << "0x" << std::setw(8) << std::setfill('0') << std::hex << entries[i].CRC;
        int index = utils::findIndex(hashes, hash.str());
        if (index != -1) {
        }
        else {

        }
    }
}

void fuse::write(std::string dir) {
    std::vector<std::string> hashes = utils::readAllLines("hash.txt");
    std::vector<std::string> names = utils::readAllLines("names.txt");
    std::vector<std::string> rel = utils::readAllLines(dir + "\\FilePaths.txt");
    std::ofstream f(dir + ".fib", std::ios::binary);
    f.write("FUSE1.00", 8);

    //std::vector<std::string> full = utils::getFiles(dir);

    uint32_t count = static_cast<uint32_t>(rel.size());
    f.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

    char zeroBytes[8];
    std::fill(std::begin(zeroBytes), std::end(zeroBytes), 0);
    f.write(zeroBytes, sizeof(zeroBytes));

    std::vector<Entry> entries(count);
    for (uint32_t i = 0; i < count; i++) {
        std::vector<char> bytes = utils::readFileBytes(dir + "\\" + rel[i]);
        entries[i].offset = f.tellp();
        entries[i].flagNSize = bytes.size() << 5;

        f.write(bytes.data(), bytes.size());

        if (utils::contains(rel[i], "!NoName")) {
            entries[i].CRC = utils::hexStringToUInt32(utils::getFileNameWithoutExtension(rel[i]));
            //std::cout << entries[i].CRC;
        }
        else {
            int index = utils::findIndex(names, utils::replace(rel[i]));
            entries[i].CRC = utils::hexStringToUInt32(hashes[index]);
        }
    }

    uint32_t tocO = f.tellp();
    f.write(reinterpret_cast<const char*>(entries.data()), entries.size() * sizeof(Entry));
    f.seekp(0x10, SEEK_SET);
    f.write(reinterpret_cast<const char*>(&tocO), sizeof(uint32_t));

    f.close();
}