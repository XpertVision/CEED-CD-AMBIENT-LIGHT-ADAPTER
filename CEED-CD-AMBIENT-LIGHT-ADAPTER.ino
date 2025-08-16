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
      digitalWrite(INDICATION_PIN, HIGH);
    else
      digitalWrite(INDICATION_PIN, LOW);
    #endif
  }
  else if (ble::doReconnect)
  {
    #ifdef BUTTON_INDICATOR
    digitalWrite(INDICATION_PIN, LOW);
    #endif
    ble::connectToServer();
  }
}