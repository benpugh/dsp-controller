#pragma once

#include <cstdint>

struct Config {
    // Hardware Configuration
    struct Hardware {
        static const uint8_t RELAY_PIN = 14;
        static const uint32_t RETRY_INTERVAL = 500;    // ms
        static const uint32_t RESET_DELAY = 100;       // ms
        
        // Serial Configuration
        static const uint32_t SERIAL_BAUD = 57600;
        static const uint32_t DEBUG_BAUD = 115200;
        
        // CAN Configuration
        static const uint32_t CAN_BAUD = 125000;
        static const uint16_t CAN_AUDIO_ID = 0x30F;
        static const uint16_t CAN_POWER_ID = 0x302;
    };

    // Generic Audio Control Configuration Template
    template<uint8_t MinHU, uint8_t MaxHU, uint8_t MinDSP, uint8_t MaxDSP, uint8_t DefaultTarget>
    struct AudioControl {
        static const uint8_t MIN_HU = MinHU;
        static const uint8_t MAX_HU = MaxHU;
        static const uint8_t MIN_DSP = MinDSP;
        static const uint8_t MAX_DSP = MaxDSP;
        static constexpr float SCALING_FACTOR = 
            (float)(MAX_DSP - MIN_DSP) / (float)(MAX_HU - MIN_HU);
        static const uint8_t DEFAULT_TARGET = DefaultTarget;
    };

    // Volume Control Configuration
    using Volume = AudioControl<0, 38, 0, 66, 0>;
    
    // Bass Control Configuration
    using Bass = AudioControl<0, 18, 0, 60, 30>;

    // Buffer Configuration
    struct Buffer {
        static const uint8_t CIRCULAR_BUFFER_SIZE = 32;
    };
}; 