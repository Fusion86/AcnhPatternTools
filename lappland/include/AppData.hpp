#pragma once

#include <memory>

#include <HorizonSaveData.hpp>

class AppData {
  private:
    // static AppData* instance;

    AppData();

  public:
    std::unique_ptr<HorizonSaveData> savedata;

    static AppData* getInstance();
};

class UsesAppData {
  public:
    AppData* getData();
};
