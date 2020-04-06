#pragma once

#include <memory>

#include <HorizonSaveData.hpp>

#define AppState AppData::getInstance()

class AppData {
  private:
    // static AppData* instance;

    AppData();

  public:
    std::unique_ptr<HorizonSaveData> savedata;

    static AppData* getInstance();
};
