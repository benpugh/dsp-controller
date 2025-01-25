#pragma once

#include <Arduino.h>
#include "../debug/Debug.h"

struct DspState {
    // Special initialization values
    static const uint8_t UNINIT_VALUE = 0xFF;
    
    // DSP (output) values
    uint8_t currentVolume = UNINIT_VALUE;
    uint8_t targetVolume = 0;
    uint8_t currentBass = UNINIT_VALUE;
    uint8_t targetBass = 30;
    
    bool isPowered = false;
    uint32_t lastVolTxTime = 0;
    uint32_t lastBassTxTime = 0;

    // Reset handling
    bool isResetting = false;
    uint32_t resetStartTime = 0;
    
    #ifdef DEBUG
        DebugStats stats{};
    #endif
}; 