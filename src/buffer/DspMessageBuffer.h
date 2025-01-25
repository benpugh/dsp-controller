#pragma once

#include <Arduino.h>
#include "../protocol/DspProtocol.h"

class DspMessageBuffer {
private:
    byte buffer[DspProtocolConstants::MESSAGE_SIZE];
    uint8_t count = 0;
    
    enum class State {
        WAITING_FOR_START,
        READING_MESSAGE
    } state = State::WAITING_FOR_START;

    bool isValidSequence(byte inByte, uint8_t position) {
        // Define expected byte sequence
        static const byte EXPECTED_SEQUENCE[] = {
            DspProtocolConstants::START_BYTES[0],
            DspProtocolConstants::START_BYTES[1],
            DspProtocolConstants::START_BYTES[2],
            DspProtocolConstants::HEADER,
            DspProtocolConstants::IDENTIFIER
        };
        
        // Only validate the first 5 bytes
        return position >= 5 || inByte == EXPECTED_SEQUENCE[position];
    }

public:
    void clear() {
        count = 0;
        state = State::WAITING_FOR_START;
    }

    bool processByte(byte inByte) {
        switch (state) {
            case State::WAITING_FOR_START:
                if (inByte == DspProtocolConstants::START_BYTES[0]) {
                    buffer[0] = inByte;
                    count = 1;
                    state = State::READING_MESSAGE;
                }
                return false;

            case State::READING_MESSAGE:
                // Check if the incoming byte matches expected sequence
                if (!isValidSequence(inByte, count)) {
                    // If we receive a new start byte, start over
                    if (inByte == DspProtocolConstants::START_BYTES[0]) {
                        buffer[0] = inByte;
                        count = 1;
                    } else {
                        // Otherwise, go back to waiting for start
                        clear();
                    }
                    return false;
                }

                buffer[count++] = inByte;
                if (count == DspProtocolConstants::MESSAGE_SIZE) {
                    state = State::WAITING_FOR_START;
                    count = 0;
                    return true;
                }
                return false;
        }
        return false;
    }

    const byte* getMessage() const {
        return buffer;
    }
}; 