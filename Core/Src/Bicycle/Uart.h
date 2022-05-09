#pragma once

#include <cstdint>
#include <functional>

namespace uart {
    void asyncReceive(std::uint8_t* dst, std::size_t bufferSize, std::function<void()> onComplete);
    void asyncTransmit(const std::uint8_t* src, std::size_t bufferSize);

    template<typename T>
    void asyncReceive(T& data, std::function<void()> onComplete) {
        asyncReceive(reinterpret_cast<std::uint8_t*>(&data), sizeof(data), std::move(onComplete));
    }

    template<typename T>
    void asyncTransmit(const T& data) {
        asyncTransmit(reinterpret_cast<const std::uint8_t*>(&data), sizeof(data));
    }
}