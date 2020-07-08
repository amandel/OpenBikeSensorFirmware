#ifndef OBS_GLOBALS_H
#define OBS_GLOBALS_H

#include <Arduino.h>

#include "config.h"
#include "displays.h"
#include "sensor.h"

// This file should contain declarations of all variables that will be used globally.
// The variables don't have to be set here, but need to be declared.

// Version
extern const char *OBSVersion;

// PINs
extern const int PushButton;

extern int confirmedMeasurements;
extern int numButtonReleased;

extern int buttonState;

extern const char *configFilename;  // <- SD library uses 8.3 filenames
extern Config config;

// Forward declare classes to build (because there is a cyclic dependency between sensor.h and displays.h)
class SSD1306DisplayDevice;
class HCSR04SensorManager;

extern SSD1306DisplayDevice* displayTest;

extern HCSR04SensorManager* sensorManager;

extern String esp_chipid;

#endif

#define MAX_SENSOR_VALUE 255

#define BLUETOOTH_ACTIVATED
#define BLUETOOTH_SERVICE_CLOSEPASS
#define BLUETOOTH_SERVICE_CONNECTION
#define BLUETOOTH_SERVICE_DEVICEINFO
#define BLUETOOTH_SERVICE_DISTANCE
#define BLUETOOTH_SERVICE_HEARTRATE
