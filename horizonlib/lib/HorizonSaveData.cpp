#include "HorizonSaveData.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "crypto/HorizonCrypto.hpp"
#include "crypto/MurmurHash3.h"

int HorizonSaveData::load(const std::string& saveDir) {
    std::string headerPath = saveDir + "/mainHeader.dat";
    std::string dataPath = saveDir + "/main.dat";

    // Read savedata
    std::ifstream dataFs(dataPath, std::ios::binary);
    if (not dataFs.good()) {
        std::cout << "Couldn't load data!" << std::endl;
        return 1;
    }

    dataFs.read((char*)&main, sizeof(main));

    // Decrypt data
    // TODO: Check if data is actually encrypted (though usually it is)

    // Read mainHeader
    std::ifstream headerFs(headerPath, std::ios::binary);
    if (not headerFs.good()) {
        std::cout << "Couldn't load header!" << std::endl;
        return 1;
    }

    headerFs.read((char*)&mainHeader, sizeof(mainHeader));
    headerFs.close();

    std::cout << "Header: " << headerPath << std::endl;
    std::cout << "Version: " << mainHeader.version << std::endl;

    if (not mainHeader.version.isSupported()) {
        std::cout << "Unkown version!" << std::endl;
        return 1;
    }

    // Get key and IV from mainHeader
    std::array<uint8_t, 0x10> key = getKeyOrIV(mainHeader.crypto.data(), 0);
    std::array<uint8_t, 0x10> iv = getKeyOrIV(mainHeader.crypto.data(), 2);

    aesCryptCtr((uint8_t*)&main, key, iv, sizeof(main), (uint8_t*)&main);

    return 0;
}

int HorizonSaveData::save(const std::string& saveDir, bool encrypt) {
    auto writeData = [&saveDir](char* data) {
        std::ofstream of(saveDir + "/main.dat");
        if (not of.good()) {
            std::cout << "Couldn't open output file!" << std::endl;
            return 1;
        }

        of.write(data, sizeof(main));
        of.close();
        return 0;
    };

    updateChecksums();

    if (encrypt) {
        // Get key and IV from mainHeader
        std::array<uint8_t, 0x10> key = getKeyOrIV(mainHeader.crypto.data(), 0);
        std::array<uint8_t, 0x10> iv = getKeyOrIV(mainHeader.crypto.data(), 2);

        // We don't want to encrypt in place so we create a new buffer
        auto data = std::make_unique<uint8_t[]>(sizeof(main));
        aesCryptCtr((uint8_t*)&main, key, iv, sizeof(main), data.get());
        writeData((char*)data.get());
    } else {
        writeData((char*)&main);
    }

    return 0;
}

void HorizonSaveData::updateChecksums() {
    const std::list<MurmurHashSection> sections = {{0x1d6e70, 0x323c0c}};
    auto ptr = (uint8_t*)&main;

    for (auto section : sections) {
        uint32_t hash;
        MurmurHash3_x86_32(&ptr[section.offset + 4], section.length, 0, &hash);
        *(uint32_t*)&ptr[section.offset] = hash;
    }
}
