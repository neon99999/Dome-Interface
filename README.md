# Dome Interface
Code  and schematics for the Big Art Dome 



## Overview

ESP8266 devices send OSC messages for buttons and heartbeat over WiFi. TouchDesigner receives these messages, assigns device IDs, updates a visual interface, and optionally remaps and forwards OSC to other systems.

## Flow

* Device boots → connects to WiFi → requests ID (`/requestID`)
* TouchDesigner assigns ID and sends it back (`/setID`)
* Devices send:

  * `/b1`, `/b2` → button states
  * `/h` → heartbeat
* TouchDesigner:

  * updates UI elements (`controllerXX/...`)
  * looks up routing in `routeTable`
  * forwards remapped OSC (press only)

## Key Components

* **ESP8266 firmware**: sends OSC with `[deviceID, value]`
* **deviceTable (DAT)**: MAC ↔ ID ↔ IP tracking
* **routeTable (DAT)**: maps device IDs to OSC addresses
* **OSC In DAT**: receives device messages
* **OSC Out DATs**:

  * `oscout1` → sends `/setID` back to devices
  * `oscout_forward` → sends remapped OSC out

## Notes

* Only button presses (`value == 1`) are forwarded
* UI still reacts to both press and release
* Routing can be edited live in `routeTable` without touching firmware
