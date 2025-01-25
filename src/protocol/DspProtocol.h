#pragma once

#include <Arduino.h>

// Command Type Structure
struct DspCmdType {
    byte primary;
    byte secondary;
};

// Protocol Constants
struct DspProtocolConstants {
    static const byte START_BYTES[3];
    static const byte HEADER = 0xDD;
    static const byte TRAILER = 0xA5;
    static const DspCmdType VOL_CMD;
    static const DspCmdType BASS_CMD;
    static const byte IDENTIFIER = 0xA1;
    static const uint8_t MESSAGE_SIZE = 11;
};

// Command Structure
struct DspCommand {
    DspCmdType commandType;
    uint8_t value;
    uint32_t timestamp;
    
    DspCommand(const DspCmdType& cmd, uint8_t val);
};

// Protocol Handler Class
class DspProtocol {
private:
    static byte calcLrc(const byte *bytes, uint8_t start, uint8_t end);

public:
    bool isValidResponse(const byte* cmd, uint8_t len);
    void sendCommand(const DspCommand& cmd);
}; 