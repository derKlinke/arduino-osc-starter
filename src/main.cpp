#include <OSC.h>

#include <string>

// define the IP address of the receiver here (e.g. your computer)
uint8_t RECEIVER_IP[4] = {10, 7, 50, 195};

// define the SSID and password of the WiFi network here
const std::string SSID          = "SRHGast";
const std::string WLAN_PASSWORD = "SRH-Welcome!";

OSCHandler oscHandler(RECEIVER_IP, 8000, 9000);

void setup() {
    Serial.begin(115200);

    // we need to begin the OSC connection before we can send messages
    // this will also connect to the WiFi network
    // ATTENTION: this will block the program until the connection is established
    //            if the connection takes too long, the board will be restarted
    oscHandler.begin(SSID, WLAN_PASSWORD);
}

void loop() {
    // send a message with the name "test" and the value "test"
    oscHandler.sendMessage("test", "test");
}