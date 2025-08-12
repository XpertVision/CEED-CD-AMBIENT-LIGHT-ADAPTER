#include "AAmbient.h"
#include "AFileSystem.h"
#include "ABLE.h"

#include "ADebug.h"
#include "AModes.h"

#ifdef SPORT_MODE
bool ambient::mode::bIsSportMode = false;
bool ambient::mode::bIsSportModeRejected = false;
#endif

std::vector<uint8_t*> ambient::statments::vAllModes;
uint8_t ambient::statments::currentMode = 0;
bool ambient::statments::bIsEnabled     = true;

uint8_t ambient::brightness::brightnessValue = 0x05;

void ambient::SetSpeed() noexcept
{
  ble::pRemoteCharacteristic->writeValue(mode::modeSpeedArray, msgArraySize);
}

void ambient::ReadSavedStatments() noexcept
{
  if(!FileSystem::SetupSaveStartUp())
  {
    #ifdef DEBUG
    Serial.println("Init Save Read Error");
    #endif
  }
  else
  {
    statments::currentMode      = FileSystem::save.modeID;
    statments::bIsEnabled       = FileSystem::save.bIsEnabled;
    brightness::brightnessValue = FileSystem::save.brightness;
  }

  #ifdef AUTO_PAIR
  if(!FileSystem::SetupConfigStartUp())
  {
    #ifdef DEBUG
    Serial.println("Init Config Read Error");
    #endif
  }
  else
  {
    if(FileSystem::config.bIsConfigured)
    {
      FileSystem::config.GetBLEAddr(ble::espBLEAddress);
      ble::espBLEAddrType = FileSystem::config.espBLEAddrType;
    }
  }
  #endif
}

void ambient::PrepareData() noexcept
{
  ReadSavedStatments();
  
  //Reserve storage for all modes before add to vector
  statments::vAllModes.reserve(15);
  //colors
  statments::vAllModes.push_back(colors::colorWhite);
  statments::vAllModes.push_back(colors::colorYellow);
  statments::vAllModes.push_back(colors::colorOrrange);
  statments::vAllModes.push_back(colors::colorRed);
  statments::vAllModes.push_back(colors::colorPink);
  statments::vAllModes.push_back(colors::colorKIATheme);
  statments::vAllModes.push_back(colors::colorPurple);
  statments::vAllModes.push_back(colors::colorBlue);
  statments::vAllModes.push_back(colors::colorCyan);
  statments::vAllModes.push_back(colors::colorGreen);
  statments::vAllModes.push_back(colors::colorDarkTurquoise);
  //modes
  statments::vAllModes.push_back(mode::modeForwardDreamingArray);
  statments::vAllModes.push_back(mode::modeForward6ColorBlueArray);
  statments::vAllModes.push_back(mode::modeForward6ColorCyanArray);
  statments::vAllModes.push_back(mode::modeForward6ColorWhiteArray);
}