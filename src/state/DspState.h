#include <Arduino.h>

struct DspState {
    // Special initialization values
    static const uint8_t UNINIT_VALUE = 255;
    
    // DSP (output) values
    uint8_t currentVolume = UNINIT_VALUE;
    uint8_t targetVolume = 0;
    uint8_t currentBass = UNINIT_VALUE;
    uint8_t targetBass = 30; // Changed back to 30 as requested (from 0)
    
    bool isPowered = false;
    uint32_t lastVolTxTime = 0;
    uint32_t lastBassTxTime = 0;

    // Reset handling
    bool isResetting = false;
    uint32_t resetStartTime = 0;
};