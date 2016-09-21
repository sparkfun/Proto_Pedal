#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=110,128
AudioSynthWaveformDc     dc1;            //xy=195,380
AudioFilterBiquad        biquad3;        //xy=278,188
AudioFilterBiquad        biquad1;        //xy=279,107
AudioFilterBiquad        biquad2;        //xy=279,149
AudioEffectMultiply      multiply1;      //xy=367,334
AudioSynthWaveformSine   sine1;          //xy=389,425
AudioMixer4              mixer1;         //xy=474,154
AudioAnalyzePeak         peak1;          //xy=577,259
AudioFilterStateVariable filter1;        //xy=594,327
AudioOutputI2S           i2s2;           //xy=750,302
AudioConnection          patchCord1(i2s1, 0, biquad1, 0);
AudioConnection          patchCord2(i2s1, 0, biquad2, 0);
AudioConnection          patchCord3(i2s1, 0, biquad3, 0);
AudioConnection          patchCord4(dc1, 0, multiply1, 1);
AudioConnection          patchCord5(biquad3, 0, mixer1, 2);
AudioConnection          patchCord6(biquad1, 0, mixer1, 0);
AudioConnection          patchCord7(biquad2, 0, mixer1, 1);
AudioConnection          patchCord8(multiply1, 0, filter1, 0);
AudioConnection          patchCord9(multiply1, peak1);
AudioConnection          patchCord10(sine1, 0, filter1, 1);
AudioConnection          patchCord11(mixer1, 0, multiply1, 0);
AudioConnection          patchCord12(filter1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=728,84
// GUItool: end automatically generated code


uint32_t next;

void setup() {
  //**** General Setup ****//
  Serial.begin(115200);
  delay(1500);
  Serial.println("Sketch has started");

  //**** Timer Setup ****//
  next = millis() + 2000;

  //**** Audio Setup ****//
  AudioNoInterrupts();

  AudioMemory(35);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.unmuteHeadphone();
  sgtl5000_1.lineInLevel(1); //use 2 for 1:1
  sgtl5000_1.lineOutLevel(13);
  
  
  //sgtl5000_1.volume((float)p8hid.p8Param[0] / 256);
  //sgtl5000_2.volume((float)p8hid.p8Param[0] / 256);
  //biquad1.setLowpass(0, 200 + 5000 * ((float)p8hid.p8Param[1] / 256), 0.707);
  //mixer1.gain(0, (float)p8hid.p8Param[2] / 256);
  //biquad2.setBandpass(0, 400 + 15000 * ((float)p8hid.p8Param[3] / 256), 0.7-(0.69*(float)p8hid.p8Param[4] / 256));
  //mixer1.gain(1, (float)p8hid.p8Param[5] / 256);
  //biquad3.setHighpass(0, 400 + 15000 * ((float)p8hid.p8Param[6] / 256), 0.707);
  //mixer1.gain(2, (float)p8hid.p8Param[7] / 256);
  //filter1.frequency(20 + 7000 * (float)p8hid.p8Param[8] / 256);
  //filter1.resonance(5 * (float)p8hid.p8Param[9] / 256);
  //sine1.amplitude((float)p8hid.p8Param[10] / 256);
  //p8hid.modFreqVar = ((float)p8hid.p8Param[11] / 256);
  //p8hid.modFreqDepth = ((float)p8hid.p8Param[12] / 256);
  //dc1.amplitude((float)p8hid.p8Param[13] / 256);
  
  filter1.octaveControl(1);
  
  AudioInterrupts();
  
}
#define HIGH_KNOB A3
#define LOW_KNOB A2
#define ENV_KNOB A1
#define SPEED_KNOB A7
#define DEPTH_KNOB A6

uint16_t lastHighKnobValue;
uint16_t lastLowKnobValue;
uint16_t lastEnvKnobValue;
uint16_t lastSpeedKnobValue;
uint16_t lastDepthKnobValue;

uint16_t debugCounter = 0;

void loop() {
  //Do this at the interval defined above, don't just free run the loop
  if (millis() >= next)
  {
    next = millis() + 15;

    lastHighKnobValue = analogRead(HIGH_KNOB);
	lastLowKnobValue = analogRead(LOW_KNOB);
	lastEnvKnobValue = analogRead(ENV_KNOB);
	lastSpeedKnobValue = analogRead(SPEED_KNOB);
	lastDepthKnobValue = analogRead(DEPTH_KNOB);
	
	//Do this if enough regular loops have occured (Write output debug info)
	if( debugCounter > 300 )
	{
		debugCounter = 0;
		
		Serial.println("Knob values: ");
		
		Serial.print("High: ");
		Serial.println(lastHighKnobValue);
		
		Serial.print("Low: ");
		Serial.println(lastLowKnobValue);

		Serial.print("Env: ");
		Serial.println(lastEnvKnobValue);

		Serial.print("Speed: ");
		Serial.println(lastSpeedKnobValue);

		Serial.print("Depth: ");
		Serial.println(lastDepthKnobValue);
		
		Serial.print("Diagnostics: max: ");
		Serial.print(" ");
		Serial.print(AudioProcessorUsageMax());
		Serial.print(" ");
		Serial.println(AudioMemoryUsageMax());
		AudioProcessorUsageMaxReset();		

		Serial.println();
	}
	debugCounter++;
	
	
	

  }

}
