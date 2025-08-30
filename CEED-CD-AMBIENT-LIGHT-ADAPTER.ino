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
    if (ble::connectToServer(true))
    {
      ambient::SetSpeed();
      ble::doConnect = false;
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