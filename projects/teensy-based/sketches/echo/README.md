# Echo Pedal

This sketch runs on a Teensy that has been embeded into a proto pedal.

**Important**: build this in a non-optimized target mode.  We were using the plain 72 MHz mode.  If you feel strongly about optimization, you'll need to slightly decrease the delay buffer size, as specified by the LEN macro.  Optimization apparently trades RAM for execution speed.

The effect emulates a vintage style tape echo, by degenerating the repeats with hipass and lopass filters.  The delay feedback can to go above unity gain, and the lopass filter has a Q control, allowing buildup of repeat artifacts.

The demo expects 5 knobs:

* Delay length at A1 -- how long it takes new input to reach the output
* Feedback level at A7 -- controls the number of echoes
* Highpass frequency at A1 -- Feedback filter highpass frequency
* Highpass resonance at A6 -- Feedback filter highpass resonance, effectively a bass boost
* Lowpass frequency at A7 -- Feedback filter lowpass frequency

Iff'n ya build it with **6** knobs, then you've got a control for the Lowpass Q, too.

The Highpass resonance contributes to the feedback.  Both it and the feedback control can cause the effect to run away.

## Directory contents:

* echo.ino -- The sketch.  Requires teensyduino
* readme.md -- This file.
* mod-delay.cpp/.h -- the delay line object...not yet mature enough to be officially contributed to Teensy Audio.
* cubic-distort.cpp/.h -- a vestigial soft-clipping distortion block.

## Performance Notes

It can be a fun effect, but it's got some ugly details.

* The distortion in the feedback loop wasn't especially useful.  The mixers already use saturating math, so they hard clip.
* There's also a white noise block feeding the input mixer, but it's usually turned off.  It's possible to introduce an almost inaudible amount, to get it to run away with no other input.
* The delay line gets wonky & distorted if the modulation input is discontinuous.  We do several things to help smooth it.
  * We throw away the noisy bottom LSBs of that ADC channel.
  * We're using a DC block to generate the timing signal for the delay, and setting the ramp time parameter to be longer than necessary.  If you want to make things sound more rubbery, turn that time way up...
  * We keep track of the last value sent to that DC block, so we aren't calling it continually, resetting the time parameter.

There's also the issue of noise floor performance.  What we've learned so far

* Having the USB port connected contributes to the 60/120 Hz in the background.
* Having the USB port connected & talking to a serial app causes even more noise.
