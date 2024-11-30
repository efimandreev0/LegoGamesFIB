#include "guess.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
std::string guess::ext(const std::vector<uint8_t>& data, bool isDir) {
    struct MagicEntry {
        std::vector<uint8_t> magic;
        int offset;
        std::string extension;
    };

    std::vector<MagicEntry> exts = {
        // fib v1 audio
        {{0x01, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00}, 0, (isDir ? "bwav" : ".bwav")},
        // fib v2 audio
        {{0xF2, 0xFF, 0xFF, 0xFF, 0x2C, 0x00, 0x00, 0x00}, 8, (isDir ? "bwav" : ".bwav")},

        // fib v1 3DS graphics / overlaps with fib v1 PSP audio
        {{0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00}, 0, (isDir ? "bwav" : ".bwav")},
        // fib v2, LEGO Batman 3 3DS graphics
        {{0x00, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF2, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00}, 0, (isDir ? "btga" : ".btga")},
        // fib v2, LEGO Movie Vita graphics
        {{0x00, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xF2, 0xFF, 0xFF, 0xFF, 0x1C, 0x00, 0x00, 0x00}, 0, (isDir ? "btga" : ".btga")},

        // fib v1 PSP graphics
        {{0x50, 0x00, 0x00, 0x00}, 8, (isDir ? "btga" : ".btga")},

        // Mostly guesses based on games w/ csv files
        {{0x04, 0x00, 0x00, 0x00}, 8, (isDir ? "bxaml" : ".bxaml")},
        {{0x08, 0x00, 0x00, 0x00}, 8, (isDir ? "bxls" : ".bxls")}, // converted xls files
        {{0x10, 0x00, 0x00, 0x00}, 8, (isDir ? "fnskl" : ".fnskl")},
        {{0x1C, 0x00, 0x00, 0x00}, 8, (isDir ? "fnanm" : ".fnanm")}, // Used in both, overlaps w/ fnmdl
        // {{0x1C, 0x00, 0x00, 0x00}, 8, (isDir ? "fnmdl" : ".fnmdl")}, // LEGO Pirates 3DS
        {{0x24, 0x00, 0x00, 0x00}, 8, (isDir ? "fnmdl" : ".fnmdl")}, // LEGO HP 5-7 3DS
        {{0x28, 0x00, 0x00, 0x00}, 8, (isDir ? "fnmdl" : ".fnmdl")},
        {{0x3C, 0x00, 0x00, 0x00}, 8, (isDir ? "lvl" : ".lvl")},   // LEGO Pirates 3DS
        {{0x44, 0x00, 0x00, 0x00}, 8, (isDir ? "lvl" : ".lvl")},   // LEGO HP 5-7 3DS

        {{'C', 'a', 'm', 'e', 'r', 'a', 'F', 'o', 'l', 'l', 'o', 'w'}, 0, (isDir ? "cam" : ".cam")},
        {{'l', 'e', 'C', 'a', 'm', 'e', 'r', 'a', 'F', 'o', 'l', 'l', 'o', 'w'}, 0, (isDir ? "cam" : ".cam")},
        {{'N', 'C', 'S', 'C'}, 0, (isDir ? "ncsc" : ".ncsc")},

        // More guesses for fib v2 stuff.
        {{0xF0, 0xFF, 0xFF, 0xFF}, 8, (isDir ? "fnanm" : ".fnanm")},
        {{0xF1, 0xFF, 0xFF, 0xFF, 0x38, 0x00, 0x00, 0x00}, 8, (isDir ? "lvl" : ".lvl")},
        {{0xF1, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00}, 8, (isDir ? "lvl" : ".lvl")},

        {{0xF2, 0xFF, 0xFF, 0xFF, 0x04, 0x00, 0x00, 0x00}, 8, (isDir ? "bxaml" : ".bxaml")},
        {{0xF2, 0xFF, 0xFF, 0xFF, 0x08, 0x00, 0x00, 0x00}, 8, (isDir ? "bxls" : ".bxls")},
        {{0xF2, 0xFF, 0xFF, 0xFF, 0x10, 0x00, 0x00, 0x00}, 8, (isDir ? "fnskl" : ".fnskl")},
        {{0xF2, 0xFF, 0xFF, 0xFF, 0x24, 0x00, 0x00, 0x00}, 8, (isDir ? "fnmdl" : ".fnmdl")},
        {{0xF2, 0xFF, 0xFF, 0xFF, 0x28, 0x00, 0x00, 0x00}, 8, (isDir ? "fnmdl" : ".fnmdl")},

        {{'L', 'O', 'C', 'A'}, 0, (isDir ? "loc" : ".loc")},
        {{0x31, 0x0d, 0x0a, 0x30, 0x30, 0x3A}, 0, (isDir ? "srt" : ".srt")},


        // PS Vita compiled shaders
        {{'G', 'X', 'P', 0x00, 0x01, 0x05, 0x50, 0x02}, 0, (isDir ? "gxp" : ".gxp")},

        // Common image formats.
        {{'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.'}, -18, (isDir ? "tga" : ".tga")},
        {{0x89, 0x50, 0x4E, 0x47}, 0, (isDir ? "png" : ".png")},
        {{0xFF, 0xD8, 0xFF, 0xDB}, 0, (isDir ? "jpg" : ".jpg")},
        {{0xFF, 0xD8, 0xFF, 0xE0}, 0, (isDir ? "jpg" : ".jpg")},
        {{0xFF, 0xD8, 0xFF, 0xE1}, 0, (isDir ? "jpg" : ".jpg")},
        {{0x42, 0x4D}, 0, (isDir ? "bmp" : ".bmp")},

        // Common audio formats.
        {{'W', 'A', 'V', 'E'}, 8, (isDir ? "wav" : ".wav")},
        {{'O', 'g', 'g', 'S'}, 0, (isDir ? "ogg" : ".ogg")},
    };

    for (const auto& entry : exts) {
        if (data.size() >= entry.offset + entry.magic.size() &&
            std::equal(entry.magic.begin(), entry.magic.end(), data.begin() + entry.offset)) {
            return entry.extension;
        }
    }

    return (isDir ? "unknown" : ".bin");
}