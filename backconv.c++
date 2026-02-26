#include <vector>
#include <cstdint>
#include <execution>
#include <algorithm>
#include <memory>
#include <array>
#include <ranges>

// Modern pixel structure
struct Pixel {
    uint8_t r, g, b, a;
};

struct Parameters {
    std::array<Pixel, 4> p; // 4 pixels per character
};

struct Context {
    std::vector<uint8_t> textbuffer;    // characters
    std::vector<uint8_t> attrbuffer;    // attributes
    std::vector<Parameters> parameters; // parameters for each character+attribute
    int width;
    int height;
};

// Modern function to convert characters to pixels
std::vector<Pixel> backconvert(const Context& context, int x1, int y1, int x2, int y2) {
    const int pixelWidth = 2;
    const int pixelHeight = 2;
    const int outWidth = (x2 - x1) * pixelWidth;
    const int outHeight = (y2 - y1) * pixelHeight;

    std::vector<Pixel> framebuffer(outWidth * outHeight);

    auto index2D = [outWidth](int x, int y) { return y * outWidth + x; };

    // Parallel processing of rows
    std::for_each(std::execution::par, 
                  std::begin(std::views::iota(y1, y2)), 
                  std::end(std::views::iota(y1, y2)), 
                  [&](int y) {
        for (int x = x1; x < x2; ++x) {
            int n = context.textbuffer[x + y * context.width]
                  + 256 * context.attrbuffer[x + y * context.width];
            
            const auto& pix = context.parameters[n].p;

            framebuffer[index2D((x - x1) * 2,     (y - y1) * 2)]     = pix[1];
            framebuffer[index2D((x - x1) * 2 + 1, (y - y1) * 2)]     = pix[0];
            framebuffer[index2D((x - x1) * 2,     (y - y1) * 2 + 1)] = pix[3];
            framebuffer[index2D((x - x1) * 2 + 1, (y - y1) * 2 + 1)] = pix[2];
        }
    });

    return framebuffer;
}
