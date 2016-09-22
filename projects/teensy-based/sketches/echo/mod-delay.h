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

#pragma once
 
#ifndef _mod_delay_h_
#define _mod_delay_h_


#include "AudioStream.h"

class AudioEffectModDelay   : public AudioStream
{
public:
	AudioEffectModDelay   (void) : AudioStream(2, inputQueueArray) 
	{
    buffer_length = 0;
	}
	
	virtual void update(void);

	// Set the parameters
	void setbuf(int32_t delay_len, int16_t* delay_buf )
  {
    delayline_p = delay_buf;
    insert_index = 0;
    buffer_length = delay_len;

    for(int32_t i = 0; i < buffer_length; i++)
    {
      delayline_p[i] = 0;
    }
  };

  void inspect(void)
  {
    Serial.print(insert_index, HEX);
    Serial.print(' ');
    Serial.print(delay_delta, HEX);
    Serial.print(' ');
    Serial.println(buffer_length, HEX);
    
  };
  
private:
	audio_block_t *inputQueueArray[2];

	int16_t *delayline_p;
 
  int32_t insert_index;
  int32_t buffer_length;
  int32_t delay_delta;
};

#endif
