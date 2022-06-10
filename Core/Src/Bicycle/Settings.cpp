//
// Created by Alex2772 on 6/10/2022.
//

#include <cassert>
#include "Settings.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_flash.h"

struct SettingsWithCrc {
    Settings settings;
    std::uint32_t crc;
};


__attribute__((__section__(".user_data"))) SettingsWithCrc gSettingsFlash;

Settings& Settings::storage() noexcept {
    static Settings s = [] {
        if (gSettingsFlash.settings.crc() == gSettingsFlash.crc) {
            return gSettingsFlash.settings;
        }

        return Settings();
    }();
    return s;
}

std::uint32_t Settings::crc() const noexcept {
    std::uint32_t accumulator = 0;
    auto begin = reinterpret_cast<const std::uint8_t*>(this);
    auto end = reinterpret_cast<const std::uint8_t*>(this + 1);
    for (auto it = begin; it != end; ++it) {
        accumulator += *it;
        accumulator <<= 1;
    }
    return accumulator;
}


void Settings::save()  const noexcept {
    SettingsWithCrc settingsWithCrc = {
        *this,
        crc()
    };


    // unlock memory
    if (HAL_FLASH_Unlock() != HAL_OK) {
        assert(0);
    }

    FLASH_Erase_Sector(FLASH_SECTOR_5, FLASH_VOLTAGE_RANGE_3);

    auto sourceBegin = reinterpret_cast<std::uint32_t*>(&settingsWithCrc);
    auto sourceEnd = sourceBegin + (sizeof(settingsWithCrc) + sizeof(std::uint32_t) - 1) / sizeof(std::uint32_t);

    for (std::uint32_t dst = reinterpret_cast<std::uint32_t>(&gSettingsFlash), *src = sourceBegin; src != sourceEnd; ++src, dst += sizeof(std::uint32_t)) {
        HAL_FLASH_Program(TYPEPROGRAM_WORD, dst, *src);
    }

    //HAL_FLASH_Lock();

    auto flashCrc = gSettingsFlash.settings.crc();
    auto myCrc = crc();
    assert(flashCrc == myCrc);
    storage() = *this;
}

