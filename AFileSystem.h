#ifndef _A_FILE_SYSTEM_H_
#define _A_FILE_SYSTEM_H_

#include <cstdint>
#include "FS.h"
#include "LittleFS.h"
#include "FileData.h"
#include "BLEDevice.h"
#include "AModes.h"

namespace FileSystem
{
  struct Save
  {
    Save() noexcept = default;
    Save(const Save&) noexcept = default;
    Save& operator=(const Save&) noexcept = default;

    bool bIsEnabled       = true; 
    uint8_t modeID        = 0;
    uint8_t brightness    = 0x03;
    #ifdef SPORT_MODE
    bool bDisabledInSport = false;
    #endif
  };

  #ifdef AUTO_PAIR
  struct Config
  {
    Config() noexcept = default;
    Config(const Config&) noexcept = default;
    Config& operator=(const Config&) noexcept = default;

    bool bIsConfigured = false;
    esp_bd_addr_t arrBLEAddress { 0, 0, 0, 0, 0, 0 };
    uint8_t espBLEAddrType = 0x00;

    void SetBLEAddr(const esp_bd_addr_t addr) noexcept;
    void GetBLEAddr(esp_bd_addr_t& addr) noexcept;
  };
  #endif
  //SetupSaveStartUp must be called first
  bool SetupSaveStartUp() noexcept;
  bool SetupConfigStartUp() noexcept;

  bool ReadSettings() noexcept;
  bool WriteSettings() noexcept;
  #ifdef AUTO_PAIR
  bool ReadConfig() noexcept;
  bool WriteConfig() noexcept;
  #endif
  extern Save save;
  #ifdef AUTO_PAIR
  extern Config config;
  #endif
  extern FileData saveData;
  #ifdef AUTO_PAIR
  extern FileData configData;
  #endif
}

#endif _A_FILE_SYSTEM_H_