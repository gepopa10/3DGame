#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>
#include <time.h>

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255);
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h);
float getRandom(const float a, const float b);

#endif // UTILS_H
