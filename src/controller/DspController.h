#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "../debug/Debug.h"
#include "../protocol/DspProtocol.h"
#include "../buffer/CircularBuffer.h"
#include "../buffer/DspMessageBuffer.h"
#include "../state/DspState.h"
#include "../Config.h"

class DspController {
private:
    DspState state;
    DspProtocol protocol;
    CircularBuffer rxBuffer;
    DspMessageBuffer messageBuffer;
    
    uint8_t adjustVolForDsp(uint8_t huVol);
    uint8_t adjustBassForDsp(uint8_t huBass);
    
    void clearRxBuffer();
    void startReset();
    void completeReset();
    void sendDspCommand(const DspCmdType& cmdType, uint8_t value);
    void processDspResponse(const byte* msg);
    void adjustAndSendCommand(const DspCmdType& cmdType, uint8_t& currentValue, 
                          uint8_t targetValue, const char* paramName);

public:
    DspController();
    
    void handleCanAudioMsg(const CAN_message_t &msg);
    void handleCanPowerMsg(const CAN_message_t &msg);
    void update();
    
    // Other public methods as needed
}; 