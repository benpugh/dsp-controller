#include "DspProtocol.h"
#include "../debug/Debug.h"

// Initialize static constants
const byte DspProtocolConstants::START_BYTES[3] = {0x55, 0x55, 0x55};
const DspCmdType DspProtocolConstants::VOL_CMD = {0x03, 0x00};
const DspCmdType DspProtocolConstants::BASS_CMD = {0x04, 0x04};

// DspCommand constructor implementation
DspCommand::DspCommand(const DspCmdType& cmd, uint8_t val) 
    : commandType(cmd), value(val) {
    timestamp = millis();
}

// Private methods
byte DspProtocol::calcLrc(const byte *bytes, uint8_t start, uint8_t end) {
    byte lrc = 0;
    for (uint8_t i = start; i <= end; i++) {
        lrc ^= bytes[i];
    }
    return lrc;
}

// Public methods
bool DspProtocol::isValidResponse(const byte* cmd, uint8_t len) {
    return (len == DspProtocolConstants::MESSAGE_SIZE && 
            cmd[0] == DspProtocolConstants::START_BYTES[0] &&
            cmd[1] == DspProtocolConstants::START_BYTES[1] &&
            cmd[2] == DspProtocolConstants::START_BYTES[2] &&
            cmd[3] == DspProtocolConstants::HEADER &&
            cmd[4] == DspProtocolConstants::IDENTIFIER &&
            cmd[10] == DspProtocolConstants::TRAILER &&
            (cmd[5] == DspProtocolConstants::VOL_CMD.primary || 
             cmd[5] == DspProtocolConstants::BASS_CMD.primary) &&
            calcLrc(cmd, 2, 8) == 0);
}

void DspProtocol::sendCommand(const DspCommand& cmd) {
    byte buffer[] = { 
        DspProtocolConstants::START_BYTES[0],
        DspProtocolConstants::START_BYTES[1], 
        DspProtocolConstants::START_BYTES[2],
        DspProtocolConstants::HEADER,
        DspProtocolConstants::IDENTIFIER,
        cmd.commandType.primary,
        cmd.commandType.secondary,
        cmd.value,
        0,
        0,
        DspProtocolConstants::TRAILER 
    };
    
    buffer[9] = calcLrc(buffer, 4, 8);
    Serial1.write(buffer, DspProtocolConstants::MESSAGE_SIZE);
    
    DEBUG_PRINTF("DSP %s | ", cmd.commandType.primary == DspProtocolConstants::VOL_CMD.primary ? "VOL" : "BASS");
    for (uint8_t i = 0; i < DspProtocolConstants::MESSAGE_SIZE; i++) {
        DEBUG_PRINTF("%02X ", buffer[i]);
    }
    DEBUG_PRINT("");
} 