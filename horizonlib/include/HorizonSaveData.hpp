#pragma once

#include "AcnhTypes.hpp"

struct MurmurHashSection {
    size_t offset;
    size_t length;
};

class HorizonSaveData {
  public:
    AcnhMainData main;
    AcnhSaveHeader mainHeader;

    /// Loads main.dat and mainHeader.dat
    int load(const std::string& saveDir);

    /// Saves main.dat and mainHeaer.dat
    int save(const std::string& saveDir, bool encrypt = true);

    /// Returns true when savedata has been loaded;
    bool loaded();

  private:
    bool isLoaded;

    void updateChecksums();
};
