# AutoRoller Pedal

This sketch runs on a Teensy that has been embeded into a proto pedal.

The effect has an input filter for tone control, and output low-pass filter for effect, and a modulation source consisting of a sine wave generator with rate dependent on input peak values, so the louder you play the more wah-wahs happen.

The demo expects 5 knobs:

* Hi knob at A3 -- Input filter high tones
* Lo knob at A2 -- Input filter low tones
* Tone knob at A1 -- Output filter cutoff and resonance.  CCW is low filter high Q, CW is high filter low Q
* Depth knob at A6 -- Sets modulation depth
* Speed knob at A7 -- Sets modulation osc rate

Directory contents:

* AutoRoller.ino -- The sketch.  Requires teensyduino
* AutoRollerAudioProject.png -- A picture of the Audio Tool workspace
* AutoRollerAudioProject.txt -- The output data that corresponds with the graphic.  Note:  This may differ from the sketch but should always match the graphic.  This is so you can modify the sketch but still have the source Audio Tool project, until you are happy with the changes and the png/txt can be updated.
* readme.md -- This file.