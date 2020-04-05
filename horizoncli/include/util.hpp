#pragma once

#include <vector>

#include <AcnhTypes.hpp>

int getOrInsertColor(std::vector<AcnhColor>& palette, const AcnhColor color) {
    // Return existing color, if possible
    for (size_t i = 0; i < palette.size(); i++) {
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
