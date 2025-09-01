#include "AFileSystem.h"
#include "AButton.h"
#include "AAmbient.h"
#include "ABLE.h"

#include "ADebug.h"

void setup()
{
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Starting Ambient light system");
  #endif

  buttonSetup();

  ambient::PrepareData();

  ble::BLESetup();
}

void loop()
{
  if (ble::doConnect == true)
  {
    if (ble::connectToServer())
    {
      #ifdef PREVENT_AUTO_ENABLE_GLITCH
      //Prevent Main LED Module glitch, when it turn on after power on, even if it was disabled
      if(ambient::statments::bIsEnabled == false)
      {
        ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::offID;
        uint8_t count = 0;
        while(count < 5)
        {
          ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);
          #ifdef DEBUG
          Serial.println("Try prevent Main LED Module glitch");
          #endif
          ++count;
        }
      }
      #endif

      //sync mode for case when SPORT was enabled before last turn off
      #ifdef SPORT_MODE
      if(FileSystem::save.bDisabledInSport)
      {
        if(ambient::statments::bIsEnabled == true)
        {
          ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::offID;

          ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);
          #ifdef DEBUG
          Serial.println("Try turn off Main LED Module for SPORT sync");
          #endif
        }

        auto mode = ambient::statments::vAllModes[ambient::statments::currentMode];
        ble::pRemoteCharacteristic->writeValue(mode, ambient::msgArraySize);

        FileSystem::save.bDisabledInSport = false;
        FileSystem::WriteSettings();
        
        if(ambient::statments::bIsEnabled == true)
        {
          ambient::statments::onOffArray[ambient::statments::onOffIDPos] = ambient::statments::onID;
          ble::pRemoteCharacteristic->writeValue(ambient::statments::onOffArray, ambient::msgArraySize);
        }

      }
      #endif

      ambient::SetSpeed();
    }
    #ifdef DEBUG
    else
      Serial.println("We have failed to connect to the server");
    #endif
  }

  if (ble::connected)
  {
    button.loop();
    #ifdef SPORT_MODE
    buttonSport.loop();
    #endif

    #ifdef BUTTON_INDICATOR
    if(ambient::statments::bIsEnabled)
    {
      #ifdef BUTTON_INDICATOR_ADAPTIVE_BRIGHTNESS
      ledc_set_duty(ledc_chanel.speed_mode, ledc_chanel.channel, dutyOnLevel);
      ledc_update_duty(ledc_chanel.speed_mode, ledc_chanel.channel);
      #else 
      digitalWrite(INDICATION_PIN, HIGH);
      #endif
    }
    else
    {
      #ifdef BUTTON_INDICATOR_ADAPTIVE_BRIGHTNESS
      ledc_set_duty(ledc_chanel.speed_mode, ledc_chanel.channel, dutyOffLevel);
      ledc_update_duty(ledc_chanel.speed_mode, ledc_chanel.channel);
      #else 
      digitalWrite(INDICATION_PIN, LOW);
      #endif
    }
    #endif
  }
  else if (ble::doReconnect)
  {
    #ifdef BUTTON_INDICATOR
    #ifdef BUTTON_INDICATOR_ADAPTIVE_BRIGHTNESS
    ledc_set_duty(ledc_chanel.speed_mode, ledc_chanel.channel, dutyOffLevel);
    ledc_update_duty(ledc_chanel.speed_mode, ledc_chanel.channel);
    #else 
    digitalWrite(INDICATION_PIN, LOW);
    #endif
    #endif
    ble::connectToServer();
  }
}