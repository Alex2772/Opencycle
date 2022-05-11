#pragma once

#include <cstdint>
#include <functional>

namespace uart {
    void asyncReceive(std::uint8_t* dst, std::size_t bufferSize, std::function<void()> onComplete);
    void asyncReceiveRepeat(std::uint8_t* dst, std::size_t bufferSize);
    void asyncTransmit(const std::uint8_t* src, std::size_t bufferSize);


    template<typename T>
    void asyncReceive(T& data, std::function<void()> onComplete) {
        asyncReceive(reinterpret_cast<std::uint8_t*>(&data), sizeof(data), std::move(onComplete));
    }

    template<typename T>
    void asyncReceiveRepeat(T& data) {
        asyncReceiveRepeat(reinterpret_cast<std::uint8_t*>(&data), sizeof(data));
    }

    template<typename T>
    void asyncTransmit(const T& data) {
        asyncTransmit(reinterpret_cast<const std::uint8_t*>(&data), sizeof(data));
    }
}