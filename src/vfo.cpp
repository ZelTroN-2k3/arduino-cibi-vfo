/*
 * Copyright (c) 2019, Vincent Hervieux vincent.hervieux@gmail.com
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the author Vincent Hervieux, nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***
 *** Main entry point
 ***/
#include "cibi.h"
#include "config.h"
#include "dds.h"
#include "display.h"
#include "input.h"
#include "vfo.h"
#ifdef WATCHDOG
#include "watchdog.h"
#endif

/// Globals:
//U8G2_SSD1309_128X64_NONAME0_F_HW_I2C display(U8G2_R0, OLED_RESET, SCL /* A5 */ , SDA /* A4 */); // working with 2.42" only
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, OLED_RESET, SCL /* A5 */ , SDA /* A4 */); // working with 2.42" and 0.96" displays
DDS dds;
Input input;
Config config(input, display, dds);
Cibi cibi(config, input, display, dds);

#ifdef WATCHDOG
wdt_interrupt()
{
#ifdef DEBUG
  Serial.println(F("Arduino Cibi VFO - wdt_interrupt"));
#endif
  config.setCibiFreq(cibi.getCurrentFreq());
}
#endif

/// Arduino setup
void setup()
{
  // 0X3C multiplied by 2 for U8G2
  const static byte I2C_OLED_ADDRESS_ PROGMEM = 0x78;
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("Arduino Cibi VFO - setup - begin"));
#endif
  // initialize the Display I2C address (for the 128x64), force a reset
  display.setI2CAddress(I2C_OLED_ADDRESS_);
  // slow down I2C to 100kHz
  display.setBusClock(100000);
  display.begin();
  display.clear();
  // custom message/bitmap begins here
  display.setFont(u8g2_font_6x10_tr);
  display.setFontRefHeightExtendedText();
  display.setDrawColor(1);
  display.setFontDirection(0);
  display.drawStr(35, 10, "Starting...");
  display.drawStr(12, 50, "arduino-cibi-vfo");
  display.sendBuffer();
  delay(1000);
  // custom message/bitmap ends here
  input.init();
  /* load config from EEPROM */
  config.open();
  /* init DDS crystal */
  dds.init();
  if(0 != config.getVFOAdj())
  {
#ifdef DEBUG
 //   Serial.print(F("Set correction: "));Serial.println(config.getVFOAdj());
#endif
    dds.setOutputCorrection(config.getVFOAdj());
  }
#ifdef DEBUG
    Serial.println(F("Setup - end"));
#endif
#ifdef WATCHDOG
  wdt_enable();
#endif
}

/// Arduino main loop
bool saveConfig = false;
int mode = -1;
void loop()
{

  int new_mode;
  bool update_display = false;
  /* read inputs */
  input.read();
  new_mode = input.getApplicationMode();
  if(mode != new_mode)
  {
    display.clearDisplay();
    mode = new_mode;
    update_display = true;
  }
  switch(mode)
  {
    case Input::CONFIG_MODE:
      config.loop(update_display);
      /* Save config upon exit */
      saveConfig = true;
      break;
#ifdef TRANSVERTER
    case Input::TRANSVERTER_MODE:
#endif
      /* TODO: transverter.loop(update_display) */
    case Input::TRANSCEIVER_MODE:
    default:
      if(saveConfig)
      {
        config.save();
        saveConfig = false;
      }
      cibi.loop(update_display);
      break;
  }
#ifdef WATCHDOG
  wdt_reset();
#endif
}
