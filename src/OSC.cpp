#include <OSC.h>

// Reset the Arduino.
void (*resetFunc)() = nullptr;

void OSCHandler::begin(const std::string &ssid, const std::string &password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.print("Trying to connect to WIFI ");
    int connectionIterator = 0;

    // lets check every 100ms if we are connected to the WiFi network
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(100);
        Serial.print("=");
        ++connectionIterator;
        
        // if connection takes too long try restarting the board
        if (connectionIterator > 200) {
            resetFunc();
        }
    }
    Serial.print("DONE!\n");

    // light up the LED to indicate that we are connected
    digitalWrite(BUILTIN_LED, HIGH);

    int udpStatus = Udp.begin(localPort);
    if (udpStatus == 0) {
        Serial.println("UDP could not get socket");
    }

    // wait a bit to wait for osc connection to be fully established
    delay(1000);

    Serial.println("Successfully setup an instance of OSCHandler!");
}