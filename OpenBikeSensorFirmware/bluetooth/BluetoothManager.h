#ifndef OBS_BLUETOOTH_BLUETOOTHMANAGER_H
#define OBS_BLUETOOTH_BLUETOOTHMANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEDescriptor.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <list>

#include "_IBluetoothService.h"
#include "DistanceService.h"
#include "HeartRateService.h"

#define SERVICE_DEVICE_UUID "0000180A-0000-1000-8000-00805F9B34FB"
#define SERVICE_DEVICE_CHAR_SYSTEMID_UUID "00002a23-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_SYSTEMID_VALUE_HEX {0x0B, 0xBA, 0x2D, 0xFE, 0xFF, 0x1A, 0x9E, 0xA0}
#define SERVICE_DEVICE_CHAR_SYSTEMID_VALUE_HEX_LEN 8
#define SERVICE_DEVICE_CHAR_MODELNUMBER_STRING_UUID "00002a24-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_MODELNUMBER_STRING_VALUE "H7"
#define SERVICE_DEVICE_CHAR_SERIALNUMBER_STRING_UUID "00002a25-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_SERIALNUMBER_STRING_VALUE "20182DBA0B"
#define SERVICE_DEVICE_CHAR_FIRMWAREREVISON_UUID "00002a26-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_FIRMWAREREVISON_VALUE "1.4.0"
#define SERVICE_DEVICE_CHAR_HARDWAREREVISION_UUID "00002a27-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_HARDWAREREVISION_VALUE "39044024.10"
#define SERVICE_DEVICE_CHAR_SOFTWAREREVISION_UUID "00002a28-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_SOFTWAREREVISION_VALUE "H7 3.1.0"
#define SERVICE_DEVICE_CHAR_MANUFACTURERNAME_STRING_UUID "00002a29-0000-1000-8000-00805f9b34fb"
#define SERVICE_DEVICE_CHAR_MANUFACTURERNAME_STRING_VALUE "Polar Electro Oy"

class BluetoothManager {
public:
  void init();
  void activateBluetooth();
  void deactivateBluetooth();
  void disconnectDevice();

  void newSensorValue(int8_t value);

  void writeToHeartRateService(uint8_t *data, size_t length);

private:
  BLEService *createDeviceInfoService();
  BLEService *createHeartRateService();

  std::list<IBluetoothService*> services;

  BLEServer *pServer;
  BLEService *pDeviceInfoService;
  BLEService *pHeartRateService;
  BLECharacteristic *pHeartRateCharacteristic;

};

#endif