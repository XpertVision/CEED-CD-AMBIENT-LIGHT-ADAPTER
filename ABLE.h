#ifndef _A_BLE_H_
#define _A_BLE_H_

#include "BLEDevice.h"
#include <string>

#include "ADebug.h"

namespace ble
{
  // The remote service we wish to connect to.
  static BLEUUID serviceUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
  // The characteristic of the remote service we are interested in.
  static BLEUUID charUUID("0000ffe1-0000-1000-8000-00805f9b34fb");

  extern esp_bd_addr_t espBLEAddress;
  extern uint8_t espBLEAddrType;
  extern bool doConnect;
  extern bool connected;
  extern bool doReconnect;
  extern BLERemoteCharacteristic* pRemoteCharacteristic;
  extern BLEAdvertisedDevice* myDevice;
  extern BLEScan* pBLEScan;

  void BLESetup() noexcept;

  static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) noexcept;

  class MyClientCallback : public BLEClientCallbacks
  {
    void onConnect(BLEClient *pclient) noexcept {};

    void onDisconnect(BLEClient *pclient) noexcept
    {
      connected = false;
      doReconnect = true;
      #ifdef DEBUG
      Serial.println("onDisconnect");
      #endif
    };
    public:
    MyClientCallback() noexcept = default;
    MyClientCallback(const MyClientCallback&) noexcept = default;
    MyClientCallback(MyClientCallback&&) noexcept = delete;
    MyClientCallback& operator=(const MyClientCallback&) noexcept = default;
    MyClientCallback& operator=(MyClientCallback&&) noexcept = delete;
  };

  static MyClientCallback myClientCallback;

  bool connectToServer() noexcept;

  //Scan for BLE servers and find the first one that advertises the service we are looking for.
  class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
  {
    //Called for each advertising BLE server.
    void onResult(BLEAdvertisedDevice advertisedDevice) noexcept;
    public:
    MyAdvertisedDeviceCallbacks() noexcept = default;
    MyAdvertisedDeviceCallbacks(const MyAdvertisedDeviceCallbacks&) noexcept = default;
    MyAdvertisedDeviceCallbacks(MyAdvertisedDeviceCallbacks&&) noexcept = delete;
    MyAdvertisedDeviceCallbacks& operator=(const MyAdvertisedDeviceCallbacks&) noexcept = default;
    MyAdvertisedDeviceCallbacks& operator=(MyAdvertisedDeviceCallbacks&&) noexcept = delete;
  };
}
#endif _A_BLE_H_