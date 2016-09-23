/* Teensy pedal echo demo sketch.
 *  
 * A tape-delay inspired delay sketch, with heavy filtering and 
 * subtle distortion in the feedback loop.
 *  
 * Copyright (c) 2016, Byron Jacquot, SparkFun Electronics
 * SparkFun code, firmware, and software is released under 
 * the MIT License(http://opensource.org/licenses/MIT).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "mod-delay.h"
#include "cubic-distort.h"

/***************/

AudioSynthNoiseWhite noise;

AudioMixer4              inmix;
AudioSynthWaveformDc     dlyctrl;
AudioEffectModDelay      xdly;
AudioFilterStateVariable delayfilthp;
AudioFilterStateVariable delayfiltlp;
AudioEffectCubicDistort  fbdist;
AudioMixer4              outmix;

AudioOutputI2S       i2s1;           
AudioInputI2S        i2s2;           
AudioControlSGTL5000 sgtl5000_1;

/***************/

////AudioConnection     patchCord01(i2s2, 0, infilt, 0);
////AudioConnection     patchCord02(infilt, 0, inmix, 0);
AudioConnection     patchCord01(i2s2, 0, inmix, 0);
AudioConnection     patchCord03(noise, 0, inmix, 2);
//AudioConnection     patchCord04(inmix, 0, fbdist, 0);
//AudioConnection     patchCord05(fbdist, 0, xdly, 0);
AudioConnection     patchCord04(inmix, 0, xdly, 0);
AudioConnection     patchCord06(xdly, 0, delayfilthp, 0);
//AudioConnection     patchCord06(xdly, 0, inmix, 1);
AudioConnection     patchCord07(delayfilthp, 2, delayfiltlp, 0);
//AudioConnection     patchCord08(delayfiltlp, 0, inmix, 1);
//AudioConnection     patchCord08(delayfiltlp, 0, fbdist, 0);
//AudioConnection     patchCord05(fbdist, 0, inmix, 1);
AudioConnection     patchCord05(delayfiltlp, 0, inmix, 1);


//AudioConnection     patchCord05(xdly, 0);


AudioConnection     patchCord100(dlyctrl,0, xdly, 1);

AudioConnection     patchCord09(i2s2, 0, outmix, 0);
AudioConnection     patchCord10(delayfiltlp, 0, outmix, 1);
//AudioConnection     patchCord10(xdly, 0, outmix, 1);

AudioConnection     patchCord11(outmix, 0, i2s1, 0);
AudioConnection     patchCord12(outmix, 0, i2s1, 1);

/***************/


// This is about max (97%!) for internal RAM
static const uint32_t LEN = 0x06800;
int16_t delaybuf[LEN];

uint32_t next;


/***************/

void param_update()
{
  uint16_t value;
#if 0
  // If you don't have panel controls, here are prebaked settings

  inmix.gain(1, 0.7);
  dlyctrl.amplitude(1.0);
  delayfilthp.frequency(150);
  delayfilthp.resonance(1.0);
  delayfiltlp.frequency(1500);
  

#else
  // ADC gives us 10 bits of data, but the bottom two are a bit noisy.
  // So we'll make them always 1's with a series of bitwise or with 3 ( | 0x03 ).

  static uint16_t last_time;

  // Set feedback level
  value = analogRead(A7);
  inmix.gain(1, (float)((value|0x03)*1.2/0x3ff));

  value = analogRead(A1);
  value |= 0x03;
  if( value != last_time)
  {
    dlyctrl.amplitude((float)(value )/0x3ff, 200);
    last_time = value;
  }
  //dlyctrl.amplitude(0.5);

  value = analogRead(A2);
  // 10730 = 10230 + 500
  delayfiltlp.frequency(500 + ((value| 0x03)*10));

  value = analogRead(A6);
  delayfilthp.frequency(100 + (value| 0x03));

  value = analogRead(A3);
  delayfilthp.resonance(((float)(value|0x03)/0x3ff)* 2);
  //delayfiltlp.resonance((float)value/0x3ff);

#endif  
}



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Setup");

  AudioMemory(20);

  AudioNoInterrupts();

  // Adding in some noise can trigger spontaneous breakaway.
  //noise.amplitude(0.025);

  inmix.gain(0, 1.0);
  //inmix.gain(1, 0.9);// set by knob
  inmix.gain(2, 0.02);

  //infilt.setLowpass(0, 2500, 0.7);
  //infilt.setHighpass(1, 40, 0.7);

  outmix.gain(0, 1.0);
  outmix.gain(1, 1.0);

  xdly.setbuf(LEN, delaybuf);

  //delayfiltlp.resonance(2.0);
  
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN );

  sgtl5000_1.lineInLevel(0,2);
  sgtl5000_1.lineOutLevel(13);

  // Testing...these appear to address a little background noise...
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.adcHighPassFilterDisable();
  sgtl5000_1.audioProcessorDisable();

  AudioInterrupts();

  next = millis() + 1000;

  Serial.println("Setup complete.");

}



void loop() {
  // put your main code here, to run repeatedly:
  static bool next_on = true;

  static uint32_t count = 0;

  param_update();

  if(millis() > next)
  {
    next += 125;
    count++;

    Serial.print("Diagnostics: ");
    Serial.print(" max, buffs: ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print(" ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
    xdly.inspect();
  }

}

