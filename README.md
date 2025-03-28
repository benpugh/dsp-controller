# DSP Controller

A Teensy-based controller that translates CAN bus audio commands from a car stereo into serial commands for a DSP-408.

## Requirements

- PlatformIO
- Teensy 4.1
- FlexCAN_T4 library

## Building

This project uses PlatformIO for building. To build:

1. Install PlatformIO
2. Clone this repository
3. Open in PlatformIO
4. Build and upload to Teensy 4.1

## Logging System

The DSP Controller uses a two-tier logging system:

### Logging Levels

- **User Logging**: Human-readable operational events (actions, power, audio)
- **Debug Logging**: Technical details (CAN messages, UART communication)

### Controlling Logging

In `platformio.ini`, configure logging flags:

```ini
build_flags = 
    -D USER_LOGGING    # Enable user-level operational logs
    -D DEBUG_LOGGING   # Enable detailed technical debugging
```

Remove or comment out flags to disable specific logging levels.

### User-Level Logging Categories

- `USER_ACTION`: System actions/operations
- `USER_POWER`: Power state changes
- `USER_AUDIO`: Audio parameter changes
- `USER_CONFIRM`: Parameter confirmations
- `USER_RETRY`: Command retry attempts
- `USER_ADJUST`: Parameter adjustments
- `USER_STATS`: System statistics

### Debug-Level Logging

- `DEBUG_LOG`: General debug messages
- `DEBUG_CAN_RX`: CAN message logging
- `DEBUG_UART_RX/TX`: UART communication
- `DEBUG_HEX`: Hexadecimal data dumps

### Usage Examples

```cpp
// User-level logging
USER_ACTION("DSP Controller initialized");
USER_POWER("DSP powered ON");
USER_AUDIO_F("Volume", oldVolume, newVolume);

// Debug-level logging
DEBUG_LOG("Processing message...");
DEBUG_CAN_RX(msg.id, msg.buf, msg.len);
DEBUG_UART_TX(buffer, length);
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details 
