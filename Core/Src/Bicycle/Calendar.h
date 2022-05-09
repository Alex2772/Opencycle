#pragma once

#include <cstdint>

struct Calendar {
    std::uint32_t hours;
    std::uint32_t minutes;
    std::uint32_t seconds;

    static Calendar get();
    void set();
};