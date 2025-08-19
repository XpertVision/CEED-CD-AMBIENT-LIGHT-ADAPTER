#include "ABLE.h"
#include "AFileSystem.h"
#include "AAmbient.h"
#include "AModes.h"

namespace ble
{
  #ifdef AUTO_PAIR
  esp_bd_addr_t espBLEAddress;
  #else
  esp_bd_addr_t espBLEAddress { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  #endif
  uint8_t espBLEAddrType = 0;
  bool doConnect   = false;
  bool connected   = false;
  bool doReconnect = false;
  BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;
  BLEAdvertisedDevice* myDevice                  = nullptr;
  BLEScan* pBLEScan                              = nullptr;

  #ifdef DEBUG
  static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) noexcept
  {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.write(pData, length);
    Serial.println();
  }
  #endif

  bool connectToServer(bool bIsFirstTry) noexcept
  {
    #ifdef DEBUG
    #ifdef AUTO_PAIR
    if(myDevice && !FileSystem::config.bIsConfigured)
    {
      Serial.print("Forming a connection to ");
      Serial.println(myDevice->getAddress().toString().c_str());
    }
    #endif
    #endif

    BLEClient *pClient = BLEDevice::createClient();

    #ifdef DEBUG
    Serial.println(" - Created client");
    #endif

    pClient->setClientCallbacks(&myClientCallback);

    #ifdef AUTO_PAIR
    if(FileSystem::config.bIsConfigured)
    #else
    if(true)
    #endif
    {
      #ifdef DEBUG
      Serial.print("Forming a connection to ");
      Serial.println(BLEAddress(espBLEAddress).toString().c_str());
      #endif
      if(!pClient->connect(BLEAddress(espBLEAddress), static_cast<esp_ble_addr_type_t>(espBLEAddrType)))
        return false;

      #ifdef DEBUG
      Serial.println("Connected to server by fast way");
      #endif
    }
    else if(myDevice)
    {
      if(!pClient->connect(myDevice))
        return false;
      
      #ifdef DEBUG
      Serial.println("Connected to server by regular way");
      #endif
    }
    else
      return false;

    pClient->setMTU(517);  //set client to request maximum MTU from server

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
      #ifdef DEBUG
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      #endif

      pClient->disconnect();
      return false;
    }

    #ifdef DEBUG
    Serial.println(" - Found our service");
    #endif

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr)
    {
      #ifdef DEBUG
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      #endif

      pClient->disconnect();
      return false;
    }

    #ifdef DEBUG
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if (pRemoteCharacteristic->canRead())
    {
      String value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if (pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    #endif

    connected = true;
    doReconnect = false;

    #ifdef AUTO_PAIR
    if(!FileSystem::config.bIsConfigured && myDevice)
    {
      FileSystem::config.SetBLEAddr((const uint8_t*)myDevice->getAddress().getNative());
      FileSystem::config.espBLEAddrType = myDevice->getAddressType();
      FileSystem::config.bIsConfigured  = true;

      FileSystem::WriteConfig();
      #ifdef DEBUG
      Serial.println("Config save done");
      #endif

      FileSystem::config.GetBLEAddr(espBLEAddress);
    }
    #endif

    #ifdef DEBUG
    Serial.println("Ambient light system ON AIR");
    #endif

    //sync mode for case when SPORT was enabled before last turn off
    #ifdef SPORT_MODE
    if(bIsFirstTry && FileSystem::save.bDisabledInSport)
    {
      auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
      ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);

      FileSystem::save.bDisabledInSport = false;
      FileSystem::WriteSettings();
    }
    #endif
    
    return true;
  }

  void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) noexcept
  {
    #ifdef DEBUG
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    #endif

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
	  {
      BLEDevice::getScan()->stop();

      if(myDevice != nullptr)
        delete myDevice;

      myDevice = new BLEAdvertisedDevice(advertisedDevice);

      doConnect = true;
    }
  }

  void BLESetup() noexcept
  {
    BLEDevice::init("");

    #ifdef AUTO_PAIR
    if(FileSystem::config.bIsConfigured)
    {
      #ifdef DEBUG
      Serial.print("Configured: ");
      Serial.println(FileSystem::config.bIsConfigured);
      Serial.print("BLE Addr type: ");
      Serial.println(FileSystem::config.espBLEAddrType);
      #endif
      doConnect = true;
      return;
    }

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(200);
    pBLEScan->setWindow(200);
    pBLEScan->setActiveScan(true);

    while(doConnect == false)
      pBLEScan->start(5);
    #else
    doConnect = true;
    #endif
  }
}