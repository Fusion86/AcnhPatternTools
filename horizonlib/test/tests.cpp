#include "catch.hpp"

#include <filesystem>
#include <iostream>
#include <memory>

#include "fmt/format.h"

#include "AcnhTypes.hpp"
#include "HorizonSaveData.hpp"

static const std::string dir = "/home/woutervisser/Desktop/Animal Crossing New Horizons/11/";

TEST_CASE("Acnh types size") {
    REQUIRE(sizeof(AcnhHeader) == 292);
    REQUIRE(sizeof(AcnhString<10>) == 20);
    REQUIRE(sizeof(AcnhUniqueIdentifier<10>) == 24);
    REQUIRE(sizeof(AcnhColor) == 3);
    REQUIRE(sizeof(AcnhPatternType) == 1);
    REQUIRE(sizeof(DesignPattern) == 680);
    REQUIRE(sizeof(ProDesignPattern) == 2216);
    REQUIRE(sizeof(AcnhMainData) == 11283104);
}

TEST_CASE("Load encrypted savedata") {
    std::unique_ptr<HorizonSaveData> saveData = std::make_unique<HorizonSaveData>();
    REQUIRE(saveData->load(dir) == 0);
    std::cout << "Hello " << saveData->main.character << " from " << saveData->main.island << std::endl;
}

TEST_CASE("Print patterns") {
    std::unique_ptr<HorizonSaveData> saveData = std::make_unique<HorizonSaveData>();
    REQUIRE(saveData->load(dir) == 0);

    fmt::print("\nName: {}\n", saveData->main.character.string.str());
    fmt::print("Island: {}\n", saveData->main.island.string.str());

    std::cout << "\nNormal patterns:" << std::endl;
    for (auto& pattern : saveData->main.designPatterns) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }

    std::cout << "\nPro patterns:" << std::endl;
    for (auto& pattern : saveData->main.proDesignPatterns) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }

    std::cout << "\nAbel Sisters patterns:" << std::endl;
    for (auto& pattern : saveData->main.ableSisters) {
        fmt::print("- {} - {}\n", pattern.name.str(), pattern.creator.string.str());
    }
}

TEST_CASE("Encrypt savedata") {
    std::unique_ptr<HorizonSaveData> saveData = std::make_unique<HorizonSaveData>();
    REQUIRE(saveData->load(dir) == 0);
    std::filesystem::create_directories("enc");
    saveData->save("enc");
    WARN("For now you'll have to manually compare these two files (use checksum)");
}

TEST_CASE("Decrypt savedata") {
    std::unique_ptr<HorizonSaveData> saveData = std::make_unique<HorizonSaveData>();
    REQUIRE(saveData->load(dir) == 0);
    std::filesystem::create_directories("dec");
    saveData->save("dec", false);
    WARN("For now you'll have to manually compare these two files (use checksum)");
}

TEST_CASE("DesignPattern setPixel(x,y)") {
    DesignPattern pattern;
    memset(&pattern.data, 0, sizeof(pattern.data));
    for (int i = 0; i < pattern.getResolution() * pattern.getResolution(); i++) {
        pattern.setPixel(i, i % 2 == 0 ? 0xA : 0xB);
    }
}
