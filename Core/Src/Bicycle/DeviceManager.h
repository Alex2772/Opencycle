#pragma once

#include <memory>
#include <vector>
#include "IDevice.h"

class DeviceManager {
private:
    std::vector<std::unique_ptr<IDevice>> mDevices;


    template<typename T>
    static void importDevice() {
        std::unique_ptr<IDevice> device = std::make_unique<T>();
        if (device->init()) {
            inst().mDevices.push_back(std::move(device));
        }
    }

    DeviceManager();
public:

    static void init();

    static DeviceManager& inst() {
        static DeviceManager m;
        return m;
    }

    static void updateState(State& state) {
        for (auto& device : inst().mDevices) {
            device->updateState(state);
        }
    }

};


