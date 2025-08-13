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
}  // End of setup.

// This is the Arduino main loop function.
void loop()
{
  // Do first connect after start up
  if (ble::doConnect == true)
  {
    if (ble::connectToServer())
    {
      #ifdef DEBUG
      Serial.println("First connection try is success");
      #endif

      ambient::SetSpeed();
      ble::doConnect = false;
    }
    #ifdef DEBUG
    else
      Serial.println("We have failed to connect to the server; will go to first connect loop");
    #endif
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
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
}  // End of loop