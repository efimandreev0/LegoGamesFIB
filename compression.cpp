#include "compression.h"
std::vector<uint8_t> compression::refpack(const std::vector<uint8_t>& srcBuffer) {
    std::vector<uint8_t> dstBuffer;
    size_t position = 0;

    while (position < srcBuffer.size()) {
        uint8_t b = srcBuffer[position];
        position++;

        int readCount = 0;

        if (b >= 0xFC) {
            readCount = b - 0xFC;
            for (size_t i = position; i < position + readCount; i++) {
                if (i < srcBuffer.size()) {
                    dstBuffer.push_back(srcBuffer[i]);
                }
            }
            position += readCount;
            break;
        }
        if (b >= 0xE0) {
            readCount = ((b & 0x1F) + 1) * 4;
            for (size_t i = position; i < position + readCount; i++) {
                if (i < srcBuffer.size()) {
                    dstBuffer.push_back(srcBuffer[i]);
                }
            }
            position += readCount;
        }
        else {
            int rawCount = 0;
            int offset = 0;

            if (b >= 0xC0) {
                uint8_t b2 = srcBuffer[position++];
                uint8_t b3 = srcBuffer[position++];
                uint8_t b4 = srcBuffer[position++];

                rawCount = (b & 0x18) >> 3;
                readCount = ((b & 0x06) << 7) + b4 + 5;
                offset = ((b & 1) << 1) + (b2 << 8) + b3 + 1;
            }
            else if (b >= 0x80) {
                uint8_t b2 = srcBuffer[position++];
                uint8_t b3 = srcBuffer[position++];

                rawCount = b2 >> 6;
                readCount = (b & 0x3F) + 4;
                offset = ((b2 & 0x3F) << 8) + b3 + 1;
            }
            else {
                uint8_t b2 = srcBuffer[position++];

                rawCount = (b & 0x0C) >> 2;
                readCount = ((b & 0x70) >> 4) + 3;
                offset = ((b & 0x03) << 8) + b2 + 1;
            }

            for (size_t i = position; i < position + rawCount; i++) {
                if (i < srcBuffer.size()) {
                    dstBuffer.push_back(srcBuffer[i]);
                }
            }
            position += rawCount;

            for (int i = 0; i < readCount; i++) {
                if (dstBuffer.size() >= offset) {
                    dstBuffer.push_back(dstBuffer[dstBuffer.size() - offset]);
                }
            }
        }
    }

    return dstBuffer;
}