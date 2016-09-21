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
AudioFilterStateVariable filter2;        //xy=627,407
AudioFilterStateVariable filter3;        //xy=661,487
AudioFilterStateVariable filter4;        //xy=690,566
AudioOutputI2S           i2s2;           //xy=842,592
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
AudioConnection          patchCord11(sine1, 0, filter2, 1);
AudioConnection          patchCord12(sine1, 0, filter3, 1);
AudioConnection          patchCord13(sine1, 0, filter4, 1);
AudioConnection          patchCord14(mixer1, 0, multiply1, 0);
AudioConnection          patchCord15(filter1, 0, filter2, 0);
AudioConnection          patchCord16(filter2, 0, filter3, 0);
AudioConnection          patchCord17(filter3, 0, filter4, 0);
AudioConnection          patchCord18(filter4, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=728,84
// GUItool: end automatically generated code

//Used to prevent free run looping
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
  sgtl5000_1.volume(1);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.unmuteHeadphone();
  sgtl5000_1.lineInLevel(1); //use 2 for 1:1
  sgtl5000_1.lineOutLevel(13);
  
  //Set input defaults -- do this for parameters that do not get touched by knobs
  //Set input filter poles
  //low
  biquad1.setLowpass(0, 200 + 5000 * (4.0 / 256.0), 0.707);
  //mid
  biquad2.setBandpass(0, 400 + 15000 * (240.0 / 256.0), 0.7-(0.69*73.0 / 256.0));
  //high
  biquad3.setHighpass(0, 400 + 15000 * (45.0 / 256.0), 0.707);
  
  //This section sets the output filters freq response to the control inputs.  It should
  //set all 4 output filters but it sounds pretty good with just the one
  filter1.octaveControl(1);

  //The output filter does get reconfigured in the loop, but start with some known values.
  filter1.frequency(20 + 7000 * (float)20 / 256);
  filter1.resonance(5 * 200 / 256);
  filter2.frequency(20 + 7000 * (float)20 / 256);
  filter2.resonance(5 * 200 / 256);
  filter3.frequency(20 + 7000 * (float)20 / 256);
  filter3.resonance(5 * 200 / 256);
  filter4.frequency(20 + 7000 * (float)20 / 256);
  filter4.resonance(5 * 200 / 256);
  
  //Initial mod parameters
  sine1.amplitude((float)100 / 256);
  dc1.amplitude((float)255 / 256);
  
  AudioInterrupts();
  
}

//Set knob names to analog input names here (use for remapping)
#define HIGH_KNOB A3
#define LOW_KNOB A2
#define TONE_KNOB A1
#define DEPTH_KNOB A6
#define SPEED_KNOB A7

uint16_t debugCounter = 0;

void loop() {
  //Do this at the interval defined above, don't just free run the loop
  if (millis() >= next)
  {
    next = millis() + 15;
    
    //Variables for calculation
    float modFreqVar = 0;
    float modFreqDepth = 0;
    
    uint16_t lastHighKnobValue;
    uint16_t lastLowKnobValue;
    uint16_t lastToneKnobValue;
    uint16_t lastDepthKnobValue;
    uint16_t lastSpeedKnobValue;
    
    //Get the knob values
    lastHighKnobValue = analogRead(HIGH_KNOB);
    lastLowKnobValue = analogRead(LOW_KNOB);
    lastToneKnobValue = analogRead(TONE_KNOB);
    lastDepthKnobValue = analogRead(DEPTH_KNOB);
    lastSpeedKnobValue = analogRead(SPEED_KNOB);
    
    //Set audio platform parameters based on those values
    
    //  Set mixer1 gain to set input filtering
    float preAmp = 2; //Use to scale input manually
    mixer1.gain(0, preAmp*(float)lastLowKnobValue / 1024);
    mixer1.gain(1, preAmp*150.0 / 256.0);
    mixer1.gain(2, preAmp*(float)lastHighKnobValue / 1024);
    
    //  Set values for output filter.
    filter1.frequency(100 + 7000 * (float)lastToneKnobValue / 1024);
    filter1.resonance(2 * (1 - (float)lastToneKnobValue / 1024));
    filter2.frequency(100 + 7000 * (float)lastToneKnobValue / 1024);
    filter2.resonance(2 * (1 - (float)lastToneKnobValue / 1024));
    filter3.frequency(100 + 7000 * (float)lastToneKnobValue / 1024);
    filter3.resonance(2 * (1 - (float)lastToneKnobValue / 1024));
    filter4.frequency(100 + 7000 * (float)lastToneKnobValue / 1024);
    filter4.resonance(2 * (1 - (float)lastToneKnobValue / 1024));
    
    //Convert inputs to variables for ease of human thinking
    modFreqVar = ((float)lastSpeedKnobValue / 1024);
    modFreqDepth = ((float)lastDepthKnobValue / 1024);
    
    //Set the sine to 0 freq plus the mod frequency.
    sine1.frequency(0 + 1000 * modFreqVar * peak1.read());
    sine1.amplitude(modFreqDepth);
    
    //When enough regular 15ms loops have occured, send out debug data to the serial
    if( debugCounter > 50 )
    {
      debugCounter = 0;
      
      Serial.println("Knob values: ");
      
      Serial.print("High: ");
      Serial.println(lastHighKnobValue);
      
      Serial.print("Low: ");
      Serial.println(lastLowKnobValue);
      
      Serial.print("Tone: ");
      Serial.println(lastToneKnobValue);
      
      Serial.print("Depth: ");
      Serial.println(lastDepthKnobValue);
      
      Serial.print("Speed: ");
      Serial.println(lastSpeedKnobValue);
      
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
