#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266WiFi.h> // Ensure the WiFi library is included for IPAddress type

// WiFi Network Settings
const char* ssid = "BIGART";     // Your WiFi network name
const char* password = "KNO1ernow";    // Your WiFi password

// Device Specific Settings
const int deviceID = 38; // Manually set device ID for each device

// Calculated Static IP based on Device ID
// Note: Adjust the static IP according to your network requirements
IPAddress staticIP(192, 168, 0, 100 + deviceID);
IPAddress gateway(192, 168, 0, 1); // Default gateway for your network
IPAddress subnet(255, 255, 255, 0); // Subnet mask for your network
IPAddress dns(192, 168, 0, 1); // DNS server, often the same as the gateway

// OSC Settings
const IPAddress outIp(192, 168, 0, 100); // IP address of the OSC server (e.g., TouchDesigner)
const unsigned int outPort = 7005; // Port on which the OSC server is listening
const unsigned int localPort = 7000; // Local port for receiving OSC messages

#endif // CONFIG_H
