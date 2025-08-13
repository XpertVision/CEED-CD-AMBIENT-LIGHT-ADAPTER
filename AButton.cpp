#include <limits.h>
#include "AButton.h"
#include "AFileSystem.h"
#include "AAmbient.h"
#include "ABLE.h"

#include "ADebug.h"

Button2 button;

#ifdef SPORT_MODE
Button2 buttonSport;

void clickedSportOnce(Button2& btn) noexcept
{
  if(ambient::statments::bIsEnabled && !ambient::mode::bIsSportMode)
  {
    ble::pRemoteCharacteristic->writeValue(ambient::colors::colorRed, ambient::msgArraySize);
    ambient::mode::bIsSportMode = true;
  }
  else if(ambient::statments::bIsEnabled && ambient::mode::bIsSportMode)
  {
    if(ambient::mode::bIsSportModeRejected)
    {
      ambient::mode::bIsSportMode = false;
      ambient::mode::bIsSportModeRejected = false;
    }
    else
    {
      auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
      ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);
      ambient::mode::bIsSportMode = false;
    }
  }
  else if(!ambient::statments::bIsEnabled)
  {
    if(ambient::mode::bIsSportMode)
    {
      auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
      ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);

      ambient::mode::bIsSportMode = false;
      ambient::mode::bIsSportModeRejected = false;
    }
    else
    {
      ble::pRemoteCharacteristic->writeValue(ambient::colors::colorRed, ambient::msgArraySize);
      ambient::mode::bIsSportMode = true;
    }
  }
}

#endif

void buttonSetup() noexcept
{
  #ifdef BUTTON_INDICATOR
  pinMode(INDICATION_PIN, OUTPUT);
  #endif

  button.begin(BUTTON_PIN);
  button.setLongClickTime(1000);
  button.setDoubleClickTime(500);

  button.setLongClickDetectedRetriggerable(false);

  button.setClickHandler(clickedOnce);
  button.setDoubleClickHandler(clickedDouble);
  button.setLongClickDetectedHandler(clickedLong);

  #ifdef SPORT_MODE
  buttonSport.begin(BUTTON_SPORT_PIN);
  buttonSport.setLongClickTime(INT_MAX);//Make long click impossible
  buttonSport.setDoubleClickTime(1);//Make double click impossible
  buttonSport.setLongClickDetectedRetriggerable(false);
  buttonSport.setClickHandler(clickedSportOnce);
  #endif
}

void clickedOnce(Button2& btn) noexcept
{
  if(!ambient::statments::bIsEnabled)
  {
    ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::onID;
    ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);
    
    FileSystem::save.bIsEnabled = ambient::statments::bIsEnabled = true;
	  FileSystem::WriteSettings();
  }
  else
  {
    #ifdef SPORT_MODE
    if(ambient::mode::bIsSportMode && !ambient::mode::bIsSportModeRejected)
    {
      auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
      ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);
      ambient::mode::bIsSportModeRejected = true;
      return;
    }
    #endif

    ++ambient::statments::currentMode;
    if(ambient::statments::currentMode == ambient::statments::vAllModes.size())
      ambient::statments::currentMode = 0;

    auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
    ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);

	  FileSystem::save.modeID = ambient::statments::currentMode;
	  FileSystem::WriteSettings();
  }
}

void clickedLong(Button2& btn) noexcept
{
  if(!ambient::statments::bIsEnabled)
  {
    ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::onID;
    ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);

    FileSystem::save.bIsEnabled = ambient::statments::bIsEnabled = true;
	  FileSystem::WriteSettings();
  }
  else
  {
    ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::offID;
    ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);

    FileSystem::save.bIsEnabled = ambient::statments::bIsEnabled = false;
	  FileSystem::WriteSettings();
  }
}

void clickedDouble(Button2& btn) noexcept
{
  if(!ambient::statments::bIsEnabled)
  {
    ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::onID;
    ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);

    FileSystem::save.bIsEnabled = ambient::statments::bIsEnabled = true;
	  FileSystem::WriteSettings();
  }
  else
  {
    if(ambient::brightness::brightnessValue >= 70)
      ambient::brightness::brightnessValue = 0x03;
    else if(ambient::brightness::brightnessValue == 0x03)
      ambient::brightness::brightnessValue += 2;
    else if(ambient::brightness::brightnessValue == 0x05)
      ambient::brightness::brightnessValue += 5;
    else
      ambient::brightness::brightnessValue += 15;

    ambient::brightness::brightnessArray[ambient::brightness::brightnessIDPos] = ambient::brightness::brightnessValue;
    ble::pRemoteCharacteristic->writeValue(ambient::brightness::brightnessArray, ambient::msgArraySize);

	  FileSystem::save.brightness = ambient::brightness::brightnessValue;
	  FileSystem::WriteSettings();
  }
}