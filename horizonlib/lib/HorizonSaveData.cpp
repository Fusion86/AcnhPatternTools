#include "HorizonSaveData.hpp"

#include <ctime>
#include <iomanip>
#include <list>
#include <memory>
#include <string>

#define CHECK_OK(x)                                                                                \
    if (x != 0) return 1;

int HorizonSaveData::load(const std::string& saveDir) {
    CHECK_OK(load(saveDir + "/main", main, mainHeader));

    // TODO: Don't hardcode villager count
    for (int i = 0; i < 1; i++) {
        VillagerData data;
        CHECK_OK(load(saveDir + "/Villager0/personal", data.personal, data.personalHeader));
        villagers.push_back(data);
    }

    isLoaded = true;
    return 0;
}

int HorizonSaveData::save(const std::string& saveDir, bool encrypt) {
    auto writeData = [](std::string outFile, char* data, size_t size) {
        std::ofstream of(outFile);
        if (not of.good()) {
            std::cout << "Couldn't open output file!" << std::endl;
            return 1;
        }

        of.write(data, size);
        of.close();
        return 0;
    };

    updateChecksums();

    if (encrypt) {
        // Generate 128 random uints which will be used for params
        auto rng = SeadRand(std::time(0));
        for (size_t i = 0; i < mainHeader.crypto.size(); i++) {
            mainHeader.crypto[i] = rng.getU32();
        }
        writeData(saveDir + "/mainHeader.dat", (char*)&mainHeader, sizeof(mainHeader));

        // Get key and IV from mainHeader
        std::array<uint8_t, 0x10> key = getKeyOrIV(mainHeader.crypto.data(), 0);
        std::array<uint8_t, 0x10> iv = getKeyOrIV(mainHeader.crypto.data(), 2);

        // We don't want to encrypt in place so we create a new buffer
        auto data = std::make_unique<uint8_t[]>(sizeof(main));
        aesCryptCtr((uint8_t*)&main, key, iv, sizeof(main), data.get());
        writeData(saveDir + "/main.dat", (char*)data.get(), sizeof(main));
    } else {
        writeData(saveDir + "/main.dat", (char*)&main, sizeof(main));
    }

    return 0;
}

bool HorizonSaveData::loaded() {
    return isLoaded;
}

void HorizonSaveData::updateChecksums() {
    const std::list<MurmurHashSection> sections = {
        {0x000110, 0x1D6D5C}, {0x1D6E70, 0x323C0C}, {0x4FAB90, 0x035AFC}, {0x530690, 0x0362BC},
        {0x566A60, 0x035AFC}, {0x59C560, 0x0362BC}, {0x5D2930, 0x035AFC}, {0x608430, 0x0362BC},
        {0x63E800, 0x035AFC}, {0x674300, 0x0362BC}, {0x6AA6D0, 0x035AFC}, {0x6E01D0, 0x0362BC},
        {0x7165A0, 0x035AFC}, {0x74C0A0, 0x0362BC}, {0x782470, 0x035AFC}, {0x7B7F70, 0x0362BC},
        {0x7EE340, 0x035AFC}, {0x823E40, 0x0362BC}, {0x85A100, 0x26899C}};
    auto ptr = (uint8_t*)&main;

    for (auto section : sections) {
        uint32_t hash;
        MurmurHash3_x86_32(&ptr[section.offset + 4], section.length, 0, &hash);
        *(uint32_t*)&ptr[section.offset] = hash;
    }
}
