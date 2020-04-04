#pragma once

#include <vector>

#include <AcnhTypes.hpp>

#include "bitmap_image.hpp"

template <typename T>
void imageDrawPattern(bitmap_image& image, const T& pattern) {
    image.clear();
    for (int y = 0; y < pattern.getResolution(); y++) {
        for (int x = 0; x < pattern.getResolution(); x++) {
            image.set_pixel(x, y, pattern.getPixelColor(x, y));
        }
    }
}

template <typename T>
bitmap_image patternToImage(const T& pattern) {
    bitmap_image image(pattern.getResolution(), pattern.getResolution());
    imageDrawPattern(image, pattern);
    return image;
}

int getOrInsertColor(std::vector<AcnhColor>& palette, const AcnhColor color) {
    // Return existing color, if possible
    for (int i = 0; i < palette.size(); i++) {
        if (palette[i] == color) return i;
    }

    // Insert new color in palette, if there's space
    if (palette.size() < 15) {
        palette.push_back(color);
        return palette.size() - 1;
    }

    // No space available for more colors :(
    return -1;
}
