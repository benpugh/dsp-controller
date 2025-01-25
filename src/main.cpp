#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "debug/Debug.h"
#include "controller/DspController.h"

// Forward declarations
class DspController;

// Global variables
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
DspController* controller = nullptr;

void canAudioCallback(const CAN_message_t &msg) {
    if (controller) controller->handleCanAudioMsg(msg);
}

void canPowerCallback(const CAN_message_t &msg) {
    if (controller) controller->handleCanPowerMsg(msg);
}

void setup() {
    DEBUG_INIT(Config::Hardware::DEBUG_BAUD);
    DEBUG_PRINT("DSP Controller Starting...");
    
    Serial1.begin(Config::Hardware::SERIAL_BAUD, SERIAL_8N2);
    
    can1.begin();
    can1.setBaudRate(Config::Hardware::CAN_BAUD);
    can1.setMaxMB(2);
    can1.setMB(MB0, RX, STD);
    can1.setMB(MB1, RX, STD);
    can1.enableMBInterrupts();
    can1.onReceive(MB0, canAudioCallback);
    can1.onReceive(MB1, canPowerCallback);
    can1.setMBFilter(REJECT_ALL);
    can1.setMBFilter(MB0, Config::Hardware::CAN_AUDIO_ID);
    can1.setMBFilter(MB1, Config::Hardware::CAN_POWER_ID);
    
    controller = new DspController();
    
    DEBUG_PRINT("Initialization complete");
}

void loop() {
    can1.events();
    if (controller) controller->update();
}