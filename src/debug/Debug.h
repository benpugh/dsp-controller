#pragma once

#include <Arduino.h>
#include <cstdint>

// Forward declarations
struct CAN_message_t;

class DebugUtils {
public:
    static void printTimestamp();
    static void printCanMsg(const CAN_message_t &msg);
    static void printDspCmd(const char* cmdType, const byte* cmd, uint8_t len);
    static void printStateChange(const char* parameter, int oldValue, int newValue);
};

struct DebugStats {
    uint32_t canMsgsReceived;
    uint32_t dspCmdsSent;
    uint32_t dspResponsesReceived;
    uint32_t lastPrintTime;
    
    void print();
};

#ifdef DEBUG
    #define DEBUG_PRINT(msg) do { DebugUtils::printTimestamp(); Serial.println(msg); } while(0)
    #define DEBUG_PRINTF(fmt, ...) do { DebugUtils::printTimestamp(); Serial.printf(fmt, __VA_ARGS__); } while(0)
    #define DEBUG_INIT(baud) do { Serial.begin(baud); } while(0)
#else
    #define DEBUG_PRINT(msg)
    #define DEBUG_PRINTF(fmt, ...)
    #define DEBUG_INIT(baud)
    
    inline void DebugUtils::printTimestamp() {}
    inline void DebugUtils::printCanMsg(const CAN_message_t&) {}
    inline void DebugUtils::printDspCmd(const char*, const uint8_t*, uint8_t) {}
    inline void DebugUtils::printStateChange(const char*, int, int) {}
    inline void DebugStats::print() {}
#endif 