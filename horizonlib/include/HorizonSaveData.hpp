#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "AcnhTypes.hpp"
#include "crypto/HorizonCrypto.hpp"
#include "crypto/MurmurHash3.h"
#include "crypto/SeadRand.hpp"

struct MurmurHashSection {
    size_t offset;
    size_t length;
};

struct VillagerData {
  public:
    AcnhPersonalData personal;
    AcnhSaveHeader personalHeader;
};

class HorizonSaveData {
  public:
    AcnhMainData main;
    AcnhSaveHeader mainHeader;

    std::vector<VillagerData> villagers;

    /// Loads main.dat and mainHeader.dat
    int load(const std::string& saveDir);

    /// Saves main.dat and mainHeaer.dat
    int save(const std::string& saveDir, bool encrypt = true);

    /// Returns true when savedata has been loaded;
    bool loaded();

  private:
    bool isLoaded;

    void updateChecksums();

    template <typename TData, typename THeader>
    int load(const std::string& baseFile, TData& dest, THeader& header) {
        const std::string dataPath = baseFile + ".dat";
        const std::string headerPath = baseFile + "Header.dat";

        std::ifstream dataFs(dataPath, std::ios::binary);
        if (not dataFs.good()) {
            // TODO: Print name
            std::cout << "Couldn't load data!" << std::endl;
            return 1;
        }

        dataFs.read((char*)&dest, sizeof(dest));

        if (not dest.header.isEncrypted()) {
            // TODO: Generate fake mainHeader
        } else {
            std::ifstream headerFs(headerPath, std::ios::binary);
            if (not headerFs.good()) {
                // TODO: Print name
                std::cout << "Couldn't load header!" << std::endl;
                return 1;
            }

            headerFs.read((char*)&header, sizeof(header));
            headerFs.close();

            // Get key and IV from mainHeader
            std::array<uint8_t, 0x10> key = getKeyOrIV(header.crypto.data(), 0);
            std::array<uint8_t, 0x10> iv = getKeyOrIV(header.crypto.data(), 2);

            aesCryptCtr((uint8_t*)&dest, key, iv, sizeof(dest), (uint8_t*)&dest);
        }
        return 0;
    }
};
