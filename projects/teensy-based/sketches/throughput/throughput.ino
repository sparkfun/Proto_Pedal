#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=200,69
AudioOutputI2S           i2s2;           //xy=365,94
AudioEffectMultiply      mult1;
AudioSynthWaveformDc     dc1;

AudioConnection          patchCord1(i2s1, 0, mult1, 0);
AudioConnection          patchCord2(dc1, 0, mult1, 1);
AudioConnection          patchCord3(mult1, 0, i2s2, 0);
AudioConnection          patchCord4(mult1, 0, i2s2, 1);

AudioControlSGTL5000     sgtl5000_1;     //xy=302,184
// GUItool: end automatically generated code

uint32_t next;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  delay(1500);

  Serial.println("Setup");

  AudioMemory(15);

  AudioNoInterrupts();


  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN );

  sgtl5000_1.lineInLevel(2);
  sgtl5000_1.lineOutLevel(13);

  dc1.amplitude(1.0);

  AudioInterrupts();

  next = millis() + 2000;

}

void loop() {
  // put your main code here, to run repeatedly:

  static const int pot_channels[5] = {A1, A7, A6, A3, A2};
  uint16_t         pot_values[5];

  static bool led = false;

  uint16_t len_pot;

  len_pot = analogRead(A1);

  //drum1.frequency(20 + (len_pot^2));

  if (millis() >= next)
  {
    next = millis() + 1000;

    for(uint8_t i = 0; i < 5; i++)
    {
      pot_values[i] = analogRead(pot_channels[i]);
    }

    Serial.print("Pot values: ");
    
    for(uint8_t i = 0; i < 5; i++)
    {
      Serial.print(pot_values[i]);
      Serial.print(' ');
    }
    Serial.println();
    
    Serial.print("Diagnostics: max: ");
    Serial.print(" ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print(" ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();

    //dc1.amplitude((led?0.0:1.0),500);

    //um1.noteOn();
  }

}
