//
// Created by Alex2772 on 9/16/2021.
//

#include "DeviceManager.h"
#include "Driver/AM2320/AM2320.h"
#include "Driver/KT/Kt.h"


DeviceManager::DeviceManager() = default;

void DeviceManager::init() {
    importDevice<AM2320>(); // temp and humidity sensor
    importDevice<Kt>();     // kuteng e-bike controller
}
