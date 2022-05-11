#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace kt {
    using LcdToKtPayload = std::array<std::uint8_t, 13>;
    using KtToLcdPayload = std::array<std::uint8_t, 12>;

    struct Config {
        /**
         * (P1) Number of magnets.
         */
        enum class NumberOfMagnets: std::uint8_t {
            OF_REDUCTOR = 87,
            OF_FORWARD = 46,
        } p1NumberOfMagnets = NumberOfMagnets::OF_REDUCTOR;

        enum class SpeedPulseSignal: std::uint8_t {
            PER_REVOLUTION_0 = 0, // forward
            PER_REVOLUTION_1 = 1, // reductor
            PER_REVOLUTION_2 = 2,
            PER_REVOLUTION_3 = 3,
            PER_REVOLUTION_4 = 4,
            PER_REVOLUTION_5 = 5,
            PER_REVOLUTION_6 = 6,
        } p2SpeedPulseSignal = SpeedPulseSignal::PER_REVOLUTION_1;


        enum class WheelSize: std::uint8_t {
            INCH_10 = 0x0e,
            INCH_12 = 0x02,
            INCH_16 = 0x04,
            INCH_20 = 0x08,
            INCH_22 = 0x0c,
            INCH_24 = 0x10,
            INCH_26 = 0x14,
            INCH_27_5 = 0x18,
            C700 = 0x18,
        } wheelSize = WheelSize::INCH_27_5;

        /**
         * (P3) Imitate torque control.
         * (true) for REDUCTOR, (false) for forward
         */
        bool p3ImitateTorqueControl = true;

        /**
         * (P4) Throttle startup setting.
         */
        enum class ThrottleStartup: std::uint8_t {
            /**
             * Motor will start from place.
             */
            ON_ZERO = 0,

            /**
             * Motor will start only if some speed is picked up.
             */
            ON_SPEED = 1,
        } p4ThrottleStartup = ThrottleStartup::ON_ZERO;

        /**
         * (P5) Battery charge monitoring.
         * This parameter is determined by the battery characteristics, ordinary 24V lithium is generally is 4-11,
         * 36V lithium is between 5_15
         */
        std::uint8_t p5BatteryChargeMonitoring = 14;

        /**
         * (C1) PAS sensor mode.
         */
        std::uint8_t c1PasSensorMode = 0;

        /**
         * (C2) Motor phase classification.
         */
        std::uint8_t c2MotorPhaseClassification = 0;

        std::uint8_t c3PasRatio = 0;
        std::uint8_t c4ThrottleFunction = 0;
        std::uint8_t c5ControllerMaximumCurrent = 02;
        std::uint8_t c12ControllerMinimumVoltage = 0;
        std::uint8_t c13BrakingStrength = 0;
        std::uint8_t c14PowerAssistTuning = 3;

        std::uint8_t maxSpeed = 60;

        bool enableFrontLight = false;

        enum class AssistLevel: std::uint8_t {
            DISABLED = 0,
            L1 = 1,
            L2 = 2,
            L3 = 3,
            L4 = 4,
            L5 = 5,
            WALK = 6
        } assistLevel = AssistLevel::L5;
    };
    namespace impl {
        union Byte {
            struct {
                bool b0: 1;
                bool b1: 1;
                bool b2: 1;
                bool b3: 1;
                bool b4: 1;
                bool b5: 1;
                bool b6: 1;
                bool b7: 1;
            };
            std::uint8_t b;
        };
        static_assert(sizeof(Byte) == 1, "Invalid byte size");
    }

    inline void calculateCrc(LcdToKtPayload& input) noexcept {
        input[5] = input[1 ] ^
                   input[2 ] ^
                   input[3 ] ^
                   input[4 ] ^
                   input[6 ] ^
                   input[7 ] ^
                   input[8 ] ^
                   input[9 ] ^
                   input[10] ^
                   input[11];
    }

    inline LcdToKtPayload configToPlayload(const Config& c) noexcept {
        std::array<impl::Byte, 13> r = {};
        r[0].b = c.p5BatteryChargeMonitoring;

        r[1].b = static_cast<uint8_t>(c.assistLevel);
        r[1].b7 = c.enableFrontLight;

        r[3].b = static_cast<uint8_t>(c.p1NumberOfMagnets);

        auto maxSpeedMinus10 = c.maxSpeed - 10;
        r[2].b = maxSpeedMinus10 << 3 | static_cast<std::uint8_t>(c.wheelSize) >> 2;
        r[4].b = static_cast<std::uint8_t>(c.wheelSize) << 6;
        r[4].b5 = maxSpeedMinus10 >> 5;
        r[4].b4 = static_cast<bool>(c.p4ThrottleStartup);
        r[4].b3 = c.p3ImitateTorqueControl;
        r[4].b |= static_cast<std::uint8_t>(c.p2SpeedPulseSignal);

        // r[5] = crc

        r[6].b = c.c2MotorPhaseClassification | c.c1PasSensorMode << 3;
        r[7].b = c.c5ControllerMaximumCurrent | c.c14PowerAssistTuning << 5 | 0b1000'0000;
        r[8].b = c.c4ThrottleFunction << 4;
        r[9].b = c.c12ControllerMinimumVoltage | 0b0001'0000;
        r[10].b = c.c13BrakingStrength << 2 | 0b000'0001;
        r[11].b = 0x32;
        r[12].b = 0x0e;

        auto& asPayload = reinterpret_cast<LcdToKtPayload&>(r);
        calculateCrc(asPayload);
        return asPayload;
    }

    struct KtToLcdDispatched {
        std::int8_t motorTemperatureCelsius;
        std::uint16_t motorPower;
        std::uint16_t period;
        bool throttle;
    };
    inline std::optional<KtToLcdDispatched> dispatch(const KtToLcdPayload& payload) noexcept {
        if (payload[0] != 0x41) {
            return std::nullopt;
        }
        if (payload[2] != 0) {
            return std::nullopt;
        }

        KtToLcdDispatched out;
        out.period = std::uint16_t(payload[3]) * 256 + payload[4];
        out.motorPower = static_cast<std::int8_t>(payload[8]) * 13;
        out.motorTemperatureCelsius = static_cast<std::uint8_t>(payload[9]) + 15;
        out.throttle = !(payload[7] & 1);
        return out;
    }
}