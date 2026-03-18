//works March 28
//Update config file with unique ID

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "config.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


WiFiUDP Udp;

#define BUTTON1_PIN D1
#define BUTTON2_PIN D6
#define LED1_PIN D2
#define LED2_PIN D5

#define LOCAL_PORT 7000

bool lastButton1State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 70; 

unsigned long lastHeartbeatMillis = 0;
const unsigned long heartbeatInterval = 8000; 

void setup() {
  Serial.begin(115200);
  setupWiFi();
  
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  Udp.begin(LOCAL_PORT);
  Serial.println("Setup complete.");
}

void loop() {
  handleButtonInputs();
  sendHeartbeat();
  listenForOSCAndControlLEDs();
}

void listenForOSCAndControlLEDs() {
  int size = Udp.parsePacket();
  if (size > 0) {
    Serial.println("OSC message received.");
    OSCMessage incomingMsg;
    while (size--) {
      incomingMsg.fill(Udp.read());
    }
    if (!incomingMsg.hasError()) {
      incomingMsg.dispatch("/l1", led1Control);
      incomingMsg.dispatch("/l2", led2Control);
    } else {
      Serial.println("Error in OSC message.");
    }
  }
}

void led1Control(OSCMessage &msg) {
  int value = msg.getInt(0);
  Serial.print("LED 1 Control - Value: ");
  Serial.println(value);
  digitalWrite(LED1_PIN, value);
}

void led2Control(OSCMessage &msg) {
  int value = msg.getInt(0);
  Serial.print("LED 2 Control - Value: ");
  Serial.println(value);
  digitalWrite(LED2_PIN, value);
}

void setupWiFi() {
  // Apply static IP configuration before connecting to WiFi
  if (!WiFi.config(staticIP, gateway, subnet, gateway)) { // Using gateway as DNS
    Serial.println("Static IP configuration failed");
  }

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Confirm the IP address
}

void handleButtonInputs() {
  debounceButton(BUTTON1_PIN, &lastButton1State, &lastDebounceTime1, "/b1");
  debounceButton(BUTTON2_PIN, &lastButton2State, &lastDebounceTime2, "/b2");
}

void debounceButton(int pin, bool* lastState, unsigned long* lastDebounceTime, const char* address) {
  bool currentState = digitalRead(pin);
  if (currentState != *lastState && (millis() - *lastDebounceTime) > debounceDelay) {
    *lastDebounceTime = millis();
    *lastState = currentState;
    sendOSCMessage(address, !currentState);
  }
}

void sendOSCMessage(const char* address, int value) {
  OSCMessage msg(address);
  msg.add(deviceID);
  msg.add(value);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  Serial.print("Sending OSC: ");
  Serial.print(address);
  Serial.print(" Value: ");
  Serial.println(value);
}

void sendHeartbeat() {
  if (millis() - lastHeartbeatMillis > heartbeatInterval) {
    lastHeartbeatMillis = millis();
    sendOSCMessage("/h", 1);
    Serial.println("Heartbeat sent.");
  }
}
