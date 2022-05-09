//
// Created by Alex2772 on 9/16/2021.
//

#include <Bicycle/Device/AM2320.h>
#include "DeviceManager.h"


DeviceManager::DeviceManager() = default;

void DeviceManager::init() {
    importDevice<AM2320>();
}
