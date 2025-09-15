# CEED-CD-AMBIENT-LIGHT-ADAPTER

## DEMONSTRATION

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/0ETntV_k2UM/0.jpg)](https://www.youtube.com/watch?v=0ETntV_k2UM)

## DISCLAIMER
Everything you do is at your own risk. The author bears no responsibility for any damage or unnecessary financial expenses.

## Supported ambient light kits

This adapter was tested with 2 kits, photos of which are attached in this block, below. However, theoretically, any kit that is controlled by an application from a smartphone, via Bluetooth LE, is compatible. However, the author cannot guarantee this.

<img width="543" height="462" alt="kit_1" src="https://github.com/user-attachments/assets/f617c50a-58e4-400c-8deb-b80e5c06577c" />
<img width="686" height="480" alt="kit_2" src="https://github.com/user-attachments/assets/a32abc1e-e9e8-4ff1-9345-c9292caf00b1" />

## Supported kit's features

The adapter does not support all the features included in the kits. You can only control the backlight strips. Changing their brightness, color, dynamic themes. Pocket and leg area backlights are not supported, but may be added later if there are enough requests.

## Adapter features

Adapter supports those functionality:
1) ON/OFF system
2) Use button indication (optional)
3) Enable red color for ambient when SPORT mode turned ON (optional)
4) Initial one-time auto-pairing with LED Kit (optional)
5) Change color & dynamic mods
6) Change brightness

## Hardware & Software dependencies

The adapter is built on ESP32. Any version is suitable. I recommend using ESP32C3 super mini. If you use another model, make sure that it supports Bluetooth LE, pay attention to what voltage it is powered by (the schema described below is based on a 3.3V power supply). A microcontroller frequency of 160 MHz is enough.

For flashing uses Arduino IDE.

Software dependencies:
1) Button2 library: https://github.com/LennartHennigs/Button2
2) FileData library: https://github.com/GyverLibs/FileData

## Adapter schema (KIA Proceed CD 2020 example)
The schema below demonstrates an adapter with full functionality for the KIA Proceed CD 2020 vehicle, when we use the ISG button as the control unit. You can use any simple button that works without LIN or CAN protocol. By default, the firmware is configured for INPUT_PULLUP buttons. In 99% of cases, for Kia/Hyundai, it is the same. For other cases, especially other vehicle brands, small changes may be needed.<br>
Explanation:<br>
INPUT_PULLUP means that when you click on the button, the electrical circuit is connected, and power is drained through the button.<br>
If your car's buttons work mirrored (power is drained through the button while it is not clicked), you need to change those code lines in the firmware:
File AButton.cpp<br>
line 78:<pre>button.begin(BUTTON_PIN);</pre> change to this <pre>button.begin(BUTTON_PIN, INPUT_PULLDOWN);</pre>
line 89:<pre>buttonSport.begin(BUTTON_SPORT_PIN);</pre> change to this <pre>buttonSport.begin(BUTTON_SPORT_PIN, INPUT_PULLDOWN);</pre>

Hardware that is needed:
1) ESP32C3 Super Mini (or other ESP32 model with Bluetooth LE)
2) DC-DC convertor from 12V to 3.3v (as example DC-DC Mini560 3.3v)
3) MOSFET Power Switch HW-517 or another that can manage a 12V power line, and with a control voltage of 3.3V. Needs only for the ISG button indicator, in other cases can be enough direct 3.3v from the ESP32 (check the original car electric schema for your indicator)
4) Diod 1N4007 (needed only for SPORT mode support, prevents electrical draining from the car wire to the ESP32 pin)

The schema below was prepared for the KIA Ceed/Proceed/Xceed 2019-2025 (CD generation). Be careful, with unused buttons or indicators, sometimes the power line is still present on original wires, and you need to detach (cut off) those wires before connecting the adapter. For example, even if a car does not ISG system, the car still delivers 12V to this button.

<img width="1920" height="1080" alt="scheme" src="https://github.com/user-attachments/assets/1b9734e5-42b6-4d87-81c1-c61162d51556" />

## How to enable/disable features

All features controls from AModes.h file.
1) If you want to have Sport mode functionality (when you turn on Sport mode - ambient light color sets as RED), you just need to uncomment code line 5 (delete // symbols at the start of the line)
2) If you want to have Indicator functionality, you just need to uncomment code line 11 (delete // symbols at the start of the line)
3) Sometimes main LED control block has own glitch, when after long idle time, system turns ON even if was disabled before. If you face this issue, you just need to uncomment code line 17 (delete // symbols at the start of the line)
4) If you want to have Auto-Pair functionality, you just need to uncomment code line 8 (delete // symbols at the start of the line)
   Auto-pair tries to automatically determine the unique MAC address of the main LED unit and save this data for quick connection. The function, depending on the LED kit, may not work. It is recommended, if possible, not to use auto-pairing, and use an any Bluetooth LE scanner on your phone to find your main LED unit and determine its MAC address. Then write it down here:
ABLE.cpp file, line 11, just put your value instead of default 0xff. <pre>esp_bd_addr_t espBLEAddress { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };</pre>

## Instalation

1) It is strongly recommended to connect the main LED unit and adapter to the ACC line that is not temporarily disconnected by the car when starting the car.
2) Before connecting the adapter to the system, install the LED kit in the car, connect to it through the native application and set the correct length of the strips and other basic settings. (like a Welcome mode when turning on or diode configuration (RGB, BGR, GRB, etc.)) Leave the strips on and connect the adapter. Then turn off the car, delete the application and turn on the car. Now the adapter will connect instead of the application.

## Colors and modes cutomization

You can set the order and number of colors and dynamic modes yourself. To do this, go to the file AAmbient.h:<br>
You can add colors in section "namespace colors", just add what you want by analogies, where RGB color code is 4, 5, 6 position into code part:
<pre>{ 0x7b, 0x00, 0x07, 0xff, 0xff, 0xff, 0x00, 0xff, 0xbf }</pre>
You can add dynamics modes in section "namespace mode", just add what you want by analogies, where mode ID is 4 position into code part:
<pre>{ 0x7b, 0xff, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0xbf }</pre>
Mode ID is just mode number into your mobile kit application.<br>
<b>ALL NAMES FOR NEW MODES AND COLOR MUST BE UNIQUE!!!</b>
Next step is adding this modes and colors to queue. Go to file AAmbient.cpp:<br>
Line 62:<pre>statments::vAllModes.reserve(15);</pre>
where 15 - is a amount of all added colors and modes<br>
After it just put all this colors and modes in order what you want (from first to last) with command:<br>
<pre>
  //FOR COLORS
  statments::vAllModes.push_back(colors::colorWhite);
  //FOR MODES
  statments::vAllModes.push_back(mode::modeForwardDreamingArray);
</pre>
where modeForwardDreamingArray - unique mode name

All modes speed, after many tests, sets by default at 60% as most cozy variant.
