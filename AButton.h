#ifndef _A_BUTTON_H_
#define _A_BUTTON_H_

#include "Button2.h"
#include "AModes.h"

#ifdef BUTTON_INDICATOR
#define INDICATION_PIN   4
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