/* Modulated delay line
 *  
 * Inspired by;
 * http://musicdsp.org/showArchiveComment.php?ArchiveID=154
 * 
 * But details changed for Teensy Audio.
 * No feedback within the class, just done externally
 * so we can add filters and stuff to the loop.
 * 
 * Delay time is a signal input to the block, and functions from 0 to 0x7fff,
 * scaling the delay time accordingly.
 */
/* 
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
 * 
 */
 

#include "mod-delay.h"

//#define INTERPOLATE (1)

 void AudioEffectModDelay::update(void)
{
	audio_block_t *audioblock, *controlblock;
	int16_t *data, *end, *ctrl;

  int32_t extract_index;

#ifdef INTERPOLATE
  int32_t interp_delta;
  int32_t next;
  int16_t calc;
#endif  

  if(buffer_length == 0)
  return;
  

	audioblock = receiveWritable(0);
  if (!audioblock) return;

  controlblock = receiveReadOnly(1);
  if (!controlblock) return;

  data = audioblock->data;
	end = audioblock->data + AUDIO_BLOCK_SAMPLES;

  ctrl = controlblock->data;
  
	do 
	{
    delayline_p[insert_index] = *data;
    insert_index++;
    if(insert_index >= buffer_length)
    {
      insert_index = 0;
    }

#ifdef INTERPOLATE
    // 
    interp_delta =  (buffer_length * (*ctrl));
    delay_delta = interp_delta >> 15;   // MSB's for delay len
    interp_delta &= 0x7fff;             //LSBs for interp distance
#else
    delay_delta = (buffer_length * (*ctrl)) >> 15;
#endif
      
    extract_index = insert_index-delay_delta;

    if(extract_index < 0)
    {
      extract_index = buffer_length + extract_index;
    }

#ifdef INTERPOLATE
    // Use the fractional part to interpolate between samples
    next = extract_index + 1;
    if(next >= buffer_length)
    {
      next = 0;
    }

    calc = delayline_p[next] -  delayline_p[extract_index];

    calc = (calc * interp_delta )>>15;
    calc += delayline_p[extract_index];

    *data = calc;
#else
    *data = delayline_p[extract_index];
#endif    

    data++;  
    ctrl++;
	} while (data < end);
	transmit(audioblock);
	release(audioblock);
  release(controlblock);
}


