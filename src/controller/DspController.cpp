#include "DspController.h"

DspController::DspController() {
    pinMode(Config::Hardware::RELAY_PIN, OUTPUT);
    digitalWrite(Config::Hardware::RELAY_PIN, LOW);
}

uint8_t DspController::adjustVolForDsp(uint8_t huVol) {
    if (huVol > Config::Volume::MAX_HU) {
        huVol = Config::Volume::MAX_HU;
    }
    uint8_t dspVol = (uint8_t)(huVol * Config::Volume::SCALING_FACTOR + 0.5);
    return min(dspVol, Config::Volume::MAX_DSP);
}

uint8_t DspController::adjustBassForDsp(uint8_t huBass) {
    if (huBass > Config::Bass::MAX_HU) {
        huBass = Config::Bass::MAX_HU;
    }
    int8_t bassOffset = huBass - 9;
    uint8_t dspBass = (uint8_t)((bassOffset - Config::Bass::MIN_HU) * 
                     Config::Bass::SCALING_FACTOR + Config::Bass::MIN_DSP);
    return min(dspBass, Config::Bass::MAX_DSP);
}

void DspController::startReset() {
    DEBUG_PRINT("Starting reset sequence");
    
    state.isResetting = true;
    state.resetStartTime = millis();
    state.isPowered = false;
    digitalWrite(Config::Hardware::RELAY_PIN, LOW);
    clearRxBuffer();
}

void DspController::completeReset() {
    state.targetVolume = Config::Volume::DEFAULT_TARGET;
    state.currentVolume = DspState::UNINIT_VALUE;
    state.targetBass = Config::Bass::DEFAULT_TARGET;
    state.currentBass = DspState::UNINIT_VALUE;
    state.lastVolTxTime = 0;
    state.lastBassTxTime = 0;
    state.isResetting = false;

    DEBUG_PRINT("Reset complete");
}

void DspController::clearRxBuffer() {
    rxBuffer.clear();
    messageBuffer.clear();
    Serial1.clear();
}

void DspController::handleCanAudioMsg(const CAN_message_t &msg) {
    DEBUG_PRINTF("CAN 0x%03X | ", msg.id);
    for (uint8_t i = 0; i < msg.len; i++) {
        DEBUG_PRINTF("%02X ", msg.buf[i]);
    }
    DEBUG_PRINT("");

    uint8_t targetVolume = adjustVolForDsp(msg.buf[0]);
    uint8_t targetBass = adjustBassForDsp(msg.buf[3] - 10);
    DEBUG_PRINTF("Audio message - Volume: %d -> %d, Bass: %d -> %d\n", 
        msg.buf[0], targetVolume, msg.buf[3] - 10, targetBass);

    adjustAndSendCommand(DspProtocolConstants::VOL_CMD, state.currentVolume, targetVolume, "Volume");
    adjustAndSendCommand(DspProtocolConstants::BASS_CMD, state.currentBass, targetBass, "Bass");
}

void DspController::handleCanPowerMsg(const CAN_message_t &msg) {
    DEBUG_PRINTF("CAN 0x%03X | ", msg.id);
    for (uint8_t i = 0; i < msg.len; i++) {
        DEBUG_PRINTF("%02X ", msg.buf[i]);
    }
    DEBUG_PRINT("");
    
    bool newPowerState = (msg.buf[1] == 2);
    if (newPowerState != state.isPowered) {
        if (newPowerState) {
            clearRxBuffer();
            state.currentVolume = DspState::UNINIT_VALUE;
            state.currentBass = DspState::UNINIT_VALUE;
            state.lastVolTxTime = 0;
            state.lastBassTxTime = 0;
            digitalWrite(Config::Hardware::RELAY_PIN, HIGH);
            state.isPowered = true;
            DEBUG_PRINT("Power ON");
        } else {
            DEBUG_PRINT("Power OFF");
            startReset();
        }
    }
}

void DspController::sendDspCommand(const DspCmdType& cmdType, uint8_t value) {
    DspCommand cmd(cmdType, value);
    protocol.sendCommand(cmd);
    
    if (cmdType.primary == DspProtocolConstants::VOL_CMD.primary) {
        state.lastVolTxTime = millis();
    } else {
        state.lastBassTxTime = millis();
    }
}

void DspController::update() {
    if (state.isResetting) {
        if (millis() - state.resetStartTime >= Config::Hardware::RESET_DELAY) {
            completeReset();
        }
        return;
    }

    if (state.isPowered) {
        if (Serial1.available()) {
            byte inByte = Serial1.read();
            
            if (messageBuffer.processByte(inByte)) {
                if (protocol.isValidResponse(messageBuffer.getMessage(), 
                    DspProtocolConstants::MESSAGE_SIZE)) {
                    processDspResponse(messageBuffer.getMessage());
                }
            }
        }
        
        uint32_t currentTime = millis();
        
        if (state.currentVolume != state.targetVolume && 
            currentTime - state.lastVolTxTime > Config::Hardware::RETRY_INTERVAL) {
            sendDspCommand(DspProtocolConstants::VOL_CMD, state.targetVolume);
        }
        
        if (state.currentBass != state.targetBass && 
                currentTime - state.lastBassTxTime > Config::Hardware::RETRY_INTERVAL) {
            sendDspCommand(DspProtocolConstants::BASS_CMD, state.targetBass);
        }

        state.stats.print();
    }
}

void DspController::processDspResponse(const byte* msg) {
    if (msg[5] == DspProtocolConstants::VOL_CMD.primary) {
        uint8_t oldVol = state.currentVolume;
        state.currentVolume = msg[9];
        if (oldVol != state.currentVolume) {
            DEBUG_PRINTF("DSP Volume changed %d -> %d", oldVol, state.currentVolume);
        }
    }
    else if (msg[5] == DspProtocolConstants::BASS_CMD.primary) {
        uint8_t oldBass = state.currentBass;
        state.currentBass = msg[9];
        if (oldBass != state.currentBass) {
            DEBUG_PRINTF("DSP Bass changed %d -> %d", oldBass, state.currentBass);
        }
    }
}

void DspController::adjustAndSendCommand(const DspCmdType& cmdType, uint8_t& currentValue, 
                                       uint8_t targetValue, const char* paramName) {
    if (state.isPowered && currentValue != targetValue) {
        DEBUG_PRINTF("State: %s changed %d -> %d", paramName, currentValue, targetValue);
        clearRxBuffer();
        sendDspCommand(cmdType, targetValue);
        currentValue = targetValue;
    }
} 