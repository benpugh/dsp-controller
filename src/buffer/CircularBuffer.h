#pragma once

#include <Arduino.h>
#include "../Config.h"
#include "../debug/Debug.h"

class CircularBuffer {
private:
    static const uint8_t BUFFER_SIZE = Config::Buffer::CIRCULAR_BUFFER_SIZE;
    byte data[BUFFER_SIZE];
    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t count = 0;

public:
    void clear() {
        head = tail = count = 0;
    }
    
    bool push(byte value) {
        if (count >= BUFFER_SIZE) {
            DEBUG_LOG("Buffer overflow!");
            return false;
        }
        data[head] = value;
        head = (head + 1) % BUFFER_SIZE;
        count++;
        return true;
    }
    
    bool pop(byte& value) {
        if (count == 0) return false;
        value = data[tail];
        tail = (tail + 1) % BUFFER_SIZE;
        count--;
        return true;
    }
    
    uint8_t available() const {
        return count;
    }
}; 