#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <core_pins.h>

// Timestamp function for consistent logging
inline uint32_t getTimestamp() {
    return millis();
}

// Stats tracking structure
struct LoggingStats {
    uint32_t canMessagesReceived;
    uint32_t dspCommandsSent;
    uint32_t dspResponsesReceived;
    // More stats as needed
    
    void reset() {
        canMessagesReceived = 0;
        dspCommandsSent = 0;
        dspResponsesReceived = 0;
    }
};

extern LoggingStats gLoggingStats;

// =============================================================================
// Debug initialization macro
// =============================================================================

#define DEBUG_INIT(baudRate) do { Serial.begin(baudRate); while (!Serial && millis() < 3000) {} } while(0)

// =============================================================================
// USER-LEVEL LOGGING MACROS
// =============================================================================

#ifdef USER_LOGGING
    // User-level logging categories
    #define USER_ACTION(msg, ...)   Serial.printf("[%lu] [ACTION] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_POWER(msg, ...)    Serial.printf("[%lu] [POWER] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_AUDIO(msg, ...)    Serial.printf("[%lu] [AUDIO] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_CONFIRM(msg, ...)  Serial.printf("[%lu] [CONFIRM] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_RETRY(msg, ...)    Serial.printf("[%lu] [RETRY] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_ADJUST(msg, ...)   Serial.printf("[%lu] [ADJUST] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    #define USER_STATS(msg, ...)    Serial.printf("[%lu] [STATS] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    
    // Specialized format for audio parameter changes
    #define USER_AUDIO_F(param, before, after) \
        Serial.printf("[%lu] [AUDIO] %s: %d → %d\n", getTimestamp(), param, before, after)
    
    // Stats checking macro for user-level stats
    #define DEBUG_CHECK_STATS(currentTime) updateAndCheckStats(currentTime)
#else
    // Empty implementations when user logging is disabled
    #define USER_ACTION(msg, ...)   ((void)0)
    #define USER_POWER(msg, ...)    ((void)0)
    #define USER_AUDIO(msg, ...)    ((void)0)
    #define USER_CONFIRM(msg, ...)  ((void)0)
    #define USER_RETRY(msg, ...)    ((void)0)
    #define USER_ADJUST(msg, ...)   ((void)0)
    #define USER_STATS(msg, ...)    ((void)0)
    #define USER_AUDIO_F(param, before, after) ((void)0)
    #define DEBUG_CHECK_STATS(currentTime) ((void)0)
#endif

// =============================================================================
// DEBUG-LEVEL LOGGING MACROS
// =============================================================================

#ifdef DEBUG_LOGGING
    // Debug-level logging
    #define DEBUG_LOG(msg, ...)     Serial.printf("[%lu] " msg "\n", getTimestamp(), ##__VA_ARGS__)
    
    // Protocol-specific debug formats
    #define DEBUG_CAN_RX(id, data, len) debugPrintCanMessage(id, data, len)
    #define DEBUG_UART_RX(data, len)    debugPrintUartData("UART RX", data, len)
    #define DEBUG_UART_TX(data, len)    debugPrintUartData("UART TX", data, len)
    
    // General hex dump for debugging
    #define DEBUG_HEX(prefix, data, len) debugPrintHexDump(prefix, data, len)
#else
    // Empty implementations when debug logging is disabled
    #define DEBUG_LOG(msg, ...)     ((void)0)
    #define DEBUG_CAN_RX(id, data, len) ((void)0)
    #define DEBUG_UART_RX(data, len)    ((void)0)
    #define DEBUG_UART_TX(data, len)    ((void)0)
    #define DEBUG_HEX(prefix, data, len) ((void)0)
#endif

// =============================================================================
// BACKWARD COMPATIBILITY MACROS
// =============================================================================

// These are provided for backward compatibility with existing code
#ifdef USER_LOGGING
    #define DEBUG_PRINTF(...)       Serial.printf(__VA_ARGS__)
    #define DEBUG_PRINT(msg)        Serial.print(msg)
    #define DEBUG_PRINTLN(msg)      Serial.println(msg)
#else
    #define DEBUG_PRINTF(...)       ((void)0)
    #define DEBUG_PRINT(msg)        ((void)0)
    #define DEBUG_PRINTLN(msg)      ((void)0)
#endif

// =============================================================================
// FUNCTION DECLARATIONS
// =============================================================================

// Debug utility functions - only declared when the corresponding logging is enabled
#ifdef DEBUG_LOGGING
void debugPrintCanMessage(uint32_t id, const uint8_t* data, uint8_t len);
void debugPrintUartData(const char* prefix, const uint8_t* data, uint8_t len);
void debugPrintHexDump(const char* prefix, const uint8_t* data, uint8_t len);
#endif

// User logging utility functions - only declared when user logging is enabled
#ifdef USER_LOGGING
void printLoggingStats();
void updateAndCheckStats(uint32_t currentTime);
#endif
