#include "AppData.hpp"

AppData::AppData() {
    savedata = std::make_unique<HorizonSaveData>();
}

AppData* AppData::getInstance() {
    static AppData* instance = nullptr;
    if (!instance) instance = new AppData();
    return instance;
}
