#pragma once

#include <cstdint>

struct Settings {
    std::uint8_t frontLightIntensity; // 1-100
    std::uint8_t backLightIntensity; // 1-100

    static const Settings& inst() noexcept {
        return storage();
    }

    void save() const noexcept;

private:
    std::uint32_t crc() const noexcept;
    static Settings& storage() noexcept;
};