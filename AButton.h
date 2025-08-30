#ifndef _A_BUTTON_H_
#define _A_BUTTON_H_

#include "Button2.h"
#include "AModes.h"

#ifdef BUTTON_INDICATOR
#define INDICATION_PIN   4

#ifdef BUTTON_INDICATOR_ADAPTIVE_BRIGHTNESS
#include "driver/ledc.h"

extern ledc_timer_config_t ledc_timer;
extern ledc_channel_config_t ledc_chanel;
extern uint8_t dutyOffLevel;
extern uint8_t dutyOnLevel;
#endif

#endif

#define BUTTON_PIN       3

#ifdef SPORT_MODE
#define BUTTON_SPORT_PIN 2

extern Button2 buttonSport;
void clickedSportOnce(Button2& btn) noexcept;

#endif

extern Button2 button;

void buttonSetup() noexcept;
void clickedOnce(Button2& btn) noexcept;
void clickedDouble(Button2& btn) noexcept;
void clickedLong(Button2& btn) noexcept;

#endif