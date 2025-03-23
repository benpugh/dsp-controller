#include "Debug.h"
#include <FlexCAN_T4.h>

// Initialize global stats tracking
LoggingStats gLoggingStats = {0};

// =============================================================================
// DEBUG-LEVEL LOGGING FUNCTIONS
// =============================================================================

#ifdef DEBUG_LOGGING
// Print a CAN message with ID and data bytes
void debugPrintCanMessage(uint32_t id, const uint8_t* data, uint8_t len) {
    Serial.printf("[%lu] CAN RX %03lX | ", getTimestamp(), id);
    for (uint8_t i = 0; i < len; i++) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
    
    // Update stats
    gLoggingStats.canMessagesReceived++;
}

// Print UART data with prefix (RX or TX)
void debugPrintUartData(const char* prefix, const uint8_t* data, uint8_t len) {
    Serial.printf("[%lu] %s | ", getTimestamp(), prefix);
    for (uint8_t i = 0; i < len; i++) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
    
    // Update stats based on prefix
    if (strcmp(prefix, "UART TX") == 0) {
        gLoggingStats.dspCommandsSent++;
    } else if (strcmp(prefix, "UART RX") == 0) {
        gLoggingStats.dspResponsesReceived++;
    }
}

// General hex dump for any binary data
void debugPrintHexDump(const char* prefix, const uint8_t* data, uint8_t len) {
    Serial.printf("[%lu] %s | ", getTimestamp(), prefix);
    for (uint8_t i = 0; i < len; i++) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
}
#endif

// =============================================================================
// USER-LEVEL LOGGING FUNCTIONS
// =============================================================================

#ifdef USER_LOGGING
// Print current logging statistics
void printLoggingStats() {
    USER_STATS("CAN msgs: %lu, DSP cmds: %lu, DSP resp: %lu", 
        gLoggingStats.canMessagesReceived,
        gLoggingStats.dspCommandsSent,
        gLoggingStats.dspResponsesReceived);
}

// Update and check if stats should be printed
void updateAndCheckStats(uint32_t currentTime) {
    static uint32_t lastStatsPrintTime = 0;
    if (currentTime - lastStatsPrintTime > 60000) { // Every minute
        printLoggingStats();
        lastStatsPrintTime = currentTime;
    }
}
#endif 