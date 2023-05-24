#ifndef OSCHandler_H
#define OSCHandler_H

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
#endif

#include <OSCMessage.h>
#include <WiFiUdp.h>

#include <string>

#define SEND_TYPED_MESSAGE_DECLARATION(type) int sendMessage(const std::string &name, type message)

#define SEND_TYPED_MESSAGE(type)                           \
    SEND_TYPED_MESSAGE_DECLARATION(type) {                 \
        OSCMessage msg(name.c_str());                      \
        msg.add(message);                                  \
        int beginStatus = Udp.beginPacket(outIp, outPort); \
        if (beginStatus == 0) { /* Problem occurred! */    \
            this->Udp.stop();                              \
            Serial.println("UDP connection failed");       \
            return -1;                                     \
        }                                                  \
        msg.send(Udp);                                     \
        Udp.endPacket();                                   \
        msg.empty();                                       \
        return 0;                                          \
    }

// Reset the Arduino.
void (*resetFunc)() = nullptr;

class OSCHandler {
public:
    /// @brief Construct a new OSCHandler object
    /// @param ip IP address of the receiver as an array of 4 uint8_t
    /// @param outPort Outgoing port
    /// @param localPort Local port
    OSCHandler(uint8_t ip[4], unsigned int outPort, unsigned int localPort)
        : outIp(ip[0], ip[1], ip[2], ip[3]), outPort(outPort), localPort(localPort){};

    /// @brief Begin the OSC connection
    /// @param ssid SSID of the WiFi network
    /// @param password Password of the WiFi network
    void begin(const std::string &ssid, const std::string &password) {
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

    SEND_TYPED_MESSAGE(int)
    SEND_TYPED_MESSAGE(float)
    SEND_TYPED_MESSAGE(double)
    SEND_TYPED_MESSAGE(const char *)

    // send string message by converting it to a const char*
    SEND_TYPED_MESSAGE_DECLARATION(std::string) {
        return sendMessage(name, message.c_str());
    }

private:
    WiFiUDP Udp;
    const IPAddress outIp;
    const unsigned int outPort;
    const unsigned int localPort;
};

#endif