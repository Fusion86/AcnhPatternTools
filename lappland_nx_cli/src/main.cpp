#include <switch.h>

#include <iostream>
#include <memory>

#include "HorizonSaveData.hpp"

Result fsOpen_DeviceSaveData(FsFileSystem* out, u64 application_id) {
    FsSaveDataAttribute attr{};
    attr.application_id = application_id;
    attr.save_data_type = FsSaveDataType_Device;
    return fsOpenSaveDataFileSystem(out, FsSaveDataSpaceId_User, &attr);
}

int main(int argc, char** argv) {
    consoleInit(NULL);

    std::cout << "Lappland NX CLI" << std::endl;

    FsFileSystem sv;
    int res = fsOpen_DeviceSaveData(&sv, 0x01006F8002326000);
    std::cout << "fsOpen_DeviceSaveData: " << res << std::endl;

    res = fsdevMountDevice("sv", sv);
    std::cout << "fsdevMountDevice: " << res << std::endl;

    // Load savedata
    std::unique_ptr<HorizonSaveData> saveData = std::make_unique<HorizonSaveData>();
    res = saveData->load("sv:/");
    std::cout << "saveData->load: " << res << std::endl;
    std::cout << "Hello " << saveData->main.character << " from " << saveData->main.island << std::endl;

    while (appletMainLoop()) {
        hidScanInput();
        u64 k = hidKeysDown(CONTROLLER_P1_AUTO);
        if (k & KEY_PLUS) break;
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
}
