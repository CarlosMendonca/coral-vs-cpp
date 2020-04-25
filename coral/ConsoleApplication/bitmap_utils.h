#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace coral {

    std::vector<uint8_t> decode_bmp(
        const uint8_t* input,
        int row_size,
        int width,
        int height,
        int channels,
        bool top_down);

    std::vector<uint8_t> read_bmp(
        const std::string& input_bmp_name,
        int* width,
        int* height,
        int* channels);
}