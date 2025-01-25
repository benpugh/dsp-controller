#include "Debug.h"
#include <FlexCAN_T4.h>

#ifdef DEBUG

void DebugUtils::printTimestamp() {
    unsigned long currentTime = millis();
    Serial.printf("[%10lu ms] ", currentTime);
}

void DebugUtils::printCanMsg(const CAN_message_t &msg) {
    printTimestamp();
    Serial.printf("CAN 0x%03X | ", msg.id);
    for (uint8_t i = 0; i < msg.len; i++) {
        Serial.printf("%02X ", msg.buf[i]);
    }
    Serial.println();
}

void DebugUtils::printDspCmd(const char* cmdType, const byte* cmd, uint8_t len) {
    printTimestamp();
    Serial.printf("DSP %s | ", cmdType);
    for (uint8_t i = 0; i < len; i++) {
        Serial.printf("%02X ", cmd[i]);
    }
    Serial.println();
}

void DebugUtils::printStateChange(const char* parameter, int oldValue, int newValue) {
    printTimestamp();
    Serial.printf("State: %s changed %d -> %d\n", parameter, oldValue, newValue);
}

void DebugStats::print() {
    if (millis() - lastPrintTime > 60000) {  // Print every minute
        DebugUtils::printTimestamp();
        Serial.printf("Stats - CAN: %lu, DSP Cmds: %lu, DSP Resp: %lu\n",
            canMsgsReceived, dspCmdsSent, dspResponsesReceived);
        lastPrintTime = millis();
    }
}

#endif 