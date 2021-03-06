#include "bitmap_utils.h"

namespace coral {

    std::vector<uint8_t> decode_bmp(
        const uint8_t* input,
        int row_size,
        int width,
        int height,
        int channels,
        bool top_down) {

        std::vector<uint8_t> output(height * width * channels);
        
        for (int i = 0; i < height; i++) {
            int src_pos;
            int dst_pos;

            for (int j = 0; j < width; j++) {
                if (!top_down) {
                    src_pos = ((height - 1 - i) * row_size) + j * channels;
                }
                else {
                    src_pos = i * row_size + j * channels;
                }

                dst_pos = (i * width + j) * channels;

                switch (channels) {
                case 1:
                    output[dst_pos] = input[src_pos];
                    break;
                case 3:
                    // BGR -> RGB
                    output[dst_pos] = input[src_pos + 2];
                    output[dst_pos + 1] = input[src_pos + 1];
                    output[dst_pos + 2] = input[src_pos];
                    break;
                case 4:
                    // BGRA -> RGBA
                    output[dst_pos] = input[src_pos + 2];
                    output[dst_pos + 1] = input[src_pos + 1];
                    output[dst_pos + 2] = input[src_pos];
                    output[dst_pos + 3] = input[src_pos + 3];
                    break;
                default:
                    std::cerr
                        << "ERROR: Unexpected number of channels '"
                        << channels
                        << "'."
                        << std::endl;
                    std::abort();
                    break;
                }
            }
        }

        return output;
    }

    std::vector<uint8_t> read_bmp(
        const std::string& input_bmp_name,
        int* width,
        int* height,
        int* channels) {

        int begin, end;

        std::ifstream file(input_bmp_name, std::ios::in | std::ios::binary);

        if (!file) {
            std::cerr
                << "ERROR: Input file '"
                << input_bmp_name
                << "' not found."
                << std::endl;
            std::abort();
        }

        begin = file.tellg();
        file.seekg(0, std::ios::end);
        end = file.tellg();
        size_t len = end - begin;

        std::vector<uint8_t> img_bytes(len);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(img_bytes.data()), len);

        const int32_t header_size = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 10));
        *width  = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 18));
        *height = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 22));
        
        const int32_t bpp = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 28));
        *channels = bpp / 8;

        // There may be padding bytes when the width is not a multiple of
        //   4 bytes; 8 * channels == bits per pixel
        const int row_size = (8 * *channels * *width + 31) / 32 * 4;

        // If height is negative, data layout is top down
        //   otherwise, it's bottom up
        bool top_down = (*height < 0);

        // Decode image, allocating tensor once the image size is known
        const uint8_t* bmp_pixels = &img_bytes[header_size];
        return decode_bmp(
            bmp_pixels,
            row_size,
            *width,
            abs(*height),
            *channels,
            top_down);
    }
}