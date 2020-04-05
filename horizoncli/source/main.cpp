#include <filesystem>

#include <HorizonSaveData.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>

#include "CLI11.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "util.hpp"

std::filesystem::path savedataDirectory;
std::filesystem::path output;
std::filesystem::path input;
bool saveDecrypted = false;

void print() {
    std::unique_ptr<HorizonSaveData> savedata = std::make_unique<HorizonSaveData>();

    if (savedata->load(savedataDirectory) != 0) return;
    fmt::print("Loaded savedata: {}\n", savedataDirectory);

    fmt::print("\nName: {}\n", savedata->main.character.string.str());
    fmt::print("Island: {}\n", savedata->main.island.string.str());

    std::cout << "\nNormal patterns:" << std::endl;
    for (auto& pattern : savedata->main.designPatterns) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }

    std::cout << "\nPro patterns:" << std::endl;
    for (auto& pattern : savedata->main.proDesignPatterns) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }

    std::cout << "\nAbel Sisters patterns:" << std::endl;
    for (auto& pattern : savedata->main.ableSisters) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }
}

void import() {
    std::unique_ptr<HorizonSaveData> savedata = std::make_unique<HorizonSaveData>();

    if (savedata->load(savedataDirectory) != 0) return;
    fmt::print("Loaded savedata: {}\n", savedataDirectory);

    int width, height, channels;
    uint8_t* image = stbi_load(input.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (image == nullptr) {
        fmt::print("Couldn't load input {}\n", input);
        return;
    }

    if (channels != 3 && channels != 4) {
        std::cout << "Images need to have either 3 or 4 channels!" << std::endl;
        stbi_image_free(image);
        return;
    }

    if (width % 32 != 0 || height % 32 != 0) {
        std::cout << "Images need to be at least 32x32 and a power of 2!" << std::endl;
        stbi_image_free(image);
        return;
    }

    bool hasAlpha = channels == 4;
    size_t rows = height / 32;
    size_t cols = width / 32;
    size_t designOffset = 0;
    std::vector<AcnhColor> palette;

    // A DesignPattern is exactly 32x32 large.
    // This means that we have to cut larger images in multiple parts,
    // and create multiple DesignPatterns for those.
    //
    // Example:
    // 64x64 image
    // -----------   <-- y = 0
    // | 1  | 2  |   <-- parts 1 and 2
    // -----------   <-- y = 32
    // | 3  | 4  |   <-- parts 3 and 4
    // -----------   <-- y = 64
    // 0   32   64

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            size_t part = row * rows + col; // 1 part = 32x32, so 128x128 images have 16 parts
            size_t y_off = row * 32;
            size_t x_off = col * 32;

            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 32; x++) {
                    // auto rgb = image.get_pixel(x_off + x, y_off + y);
                    // AcnhColor color{rgb.red, rgb.green, rgb.blue}; // HACK: Ugly
                    uint8_t* srcPtr = image + (x_off + x + (y_off + y) * width) * channels;
                    AcnhColor color{
                        srcPtr[0],
                        srcPtr[1],
                        srcPtr[2],
                    };

                    int idx = 0xF; // Transparent by default
                    if (!hasAlpha || srcPtr[3] != 0) {
                        // Get color palette index if pixel is NOT transparent
                        idx = getOrInsertColor(palette, color);
                    }
                    savedata->main.designPatterns[designOffset + part].setPixel(x, y, idx);
                }
            }
        }
    }

    // Update palette in pattern
    // TODO: Write non-shit code (build AcnhColor[] once and then set for all)
    for (size_t j = 0; j < rows * cols; j++) {
        for (size_t i = 0; i < 15; i++) {
            AcnhColor color{};
            if (palette.size() > i) color = palette[i];
            savedata->main.designPatterns[designOffset + j].palette[i] = color;
        }
    }

    stbi_image_free(image);

    std::filesystem::create_directories(output);
    savedata->save(output, !saveDecrypted);
}

void extract() {
    std::filesystem::path outDirDesignPatterns(output / "Design Patterns");
    std::filesystem::path outDirProDesignPatterns(output / "Pro Design Patterns");
    std::filesystem::path outDirAbleSistersPatterns(output / "Able Sisters Design Patterns");

    // Create directories
    auto dirs = {outDirAbleSistersPatterns, outDirDesignPatterns, outDirProDesignPatterns};
    for (const auto& dir : dirs) {
        std::string cxc = dir;
        std::filesystem::create_directories(dir);
    }

    std::unique_ptr<HorizonSaveData> savedata = std::make_unique<HorizonSaveData>();

    if (savedata->load(savedataDirectory) != 0) return;
    fmt::print("Loaded savedata: {}\n", savedataDirectory);

    for (size_t i = 0; i < savedata->main.designPatterns.size(); i++) {
        const auto& pattern = savedata->main.designPatterns[i];
        const int res = pattern.getResolution();
        const auto imageData = pattern.getRgbaImage();
        std::string filename = outDirDesignPatterns / fmt::format("{} - {}.png", i, pattern.name);
        stbi_write_png(filename.c_str(), res, res, 4, imageData.get(), res * 4);
    }

    // for (size_t i = 0; i < savedata->main.proDesignPatterns.size(); i++) {
    //     const auto& pattern = savedata->main.proDesignPatterns[i];
    //     const int res = pattern.getResolution();
    //     const auto imageData = pattern.getRgbaImage();
    //     std::string filename = outDirProDesignPatterns / fmt::format("{} - {}.png", i, pattern.name);
    //     stbi_write_png(filename.c_str(), res, res, 4, imageData.get(), res * 4);
    // }

    const auto& pattern = savedata->main.townFlag;
    const int res = pattern.getResolution();
    const auto imageData = pattern.getRgbaImage();
    std::string filename = output / fmt::format("Townflag - {}.png", pattern.name);
    stbi_write_png(filename.c_str(), res, res, 4, imageData.get(), res * 4);

    // for (size_t i = 0; i < savedata->main.ableSisters.size(); i++) {
    //     const auto& pattern = savedata->main.ableSisters[i];
    //     const int res = pattern.getResolution();
    //     const auto imageData = pattern.getRgbaImage();
    //     std::string filename = outDirAbleSistersPatterns / fmt::format("{} - {}.png", i, pattern.name);
    //     stbi_write_png(filename.c_str(), res, res, 4, imageData.get(), res * 4);
    // }
}

void decrypt() {
    std::unique_ptr<HorizonSaveData> savedata = std::make_unique<HorizonSaveData>();

    if (savedata->load(savedataDirectory) != 0) return;
    fmt::print("Loaded savedata: {}\n", savedataDirectory);

    std::filesystem::create_directories(output);
    savedata->save(output, false);
}

int main(int argc, char** argv) {
    CLI::App app{"Horizonlib CLI"};
    app.require_subcommand(1);

    auto cmd_print = app.add_subcommand("print", "Print savedata info.")->parse_complete_callback(print);
    cmd_print->add_option("-s", savedataDirectory, "Directory containing savedata.")->required();

    auto cmd_import = app.add_subcommand("import", "Import BMP into savedata.")->parse_complete_callback(import);
    cmd_import->add_option("-s", savedataDirectory, "Directory containing savedata.")->required();
    cmd_import->add_option("-i", input, "BMP to import.")->required();
    cmd_import->add_option("-o", output, "Directory where to save the decrypted savedata.")->required();
    cmd_import->add_flag("-d", saveDecrypted, "Don't encrypt savedata");

    auto cmd_extract = app.add_subcommand("extract", "Extract BMPs from savedata.")->parse_complete_callback(extract);
    cmd_extract->add_option("-s", savedataDirectory, "Directory containing savedata.")->required();
    cmd_extract->add_option("-o", output, "Directory where to dump the extracted images.")->required();

    auto cmd_decrypt = app.add_subcommand("decrypt", "Decrypt savedata.")->parse_complete_callback(decrypt);
    cmd_decrypt->add_option("-s", savedataDirectory, "Directory containing savedata.")->required();
    cmd_decrypt->add_option("-o", output, "Directory where to save the decrypted savedata.")->required();

    CLI11_PARSE(app, argc, argv);

    return 0;
}
