/* Cubic distortion waveshaper.
 * 
 * Extracted from The MusicDSP list "Moog VCF, variation #1" filter
 * http://musicdsp.org/showArchiveComment.php?ArchiveID=25
 * 
 * Implements the function:
 * output = input - (1/n*(input^3))
 * 
 * See shaper1.ods for a plotted transfer curve.
 * 
 * Which is a gradual s-shaped input:output transfer characteristic
 * for soft clipping.
 * 
 * 'n' determines the severity of the flattening of the curve,
 * and also the maximum output value. 
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

 #include "cubic-distort.h"
 #include "utility/dspinst.h"

 
void AudioEffectCubicDistort::update(void)
{
	audio_block_t *audioblock;
	int16_t *data, *end;

  int32_t calc, temp;


	audioblock = receiveWritable(0);
  if (!audioblock) return;

  data = audioblock->data;
	end = audioblock->data + AUDIO_BLOCK_SAMPLES;

	do 
	{
    calc = *data;
    calc <<= 16;

    // Cube
    temp = multiply_32x32_rshift32_rounded(calc, calc);
    temp = multiply_32x32_rshift32_rounded(calc, temp);

    // times 1/3 - peaks at 0.666
    //temp = multiply_32x32_rshift32_rounded(calc, 0x2aaaaaaa);

    // times 1/5 - peaks at 0.8
    temp = multiply_32x32_rshift32_rounded(calc, 0x19999999);

    // times 1/6 - peaks at 0.833
    //temp = multiply_32x32_rshift32_rounded(calc, 0x15555555);

    *data = (calc - temp)>> 16;
    data++;  
	} while (data < end);
	transmit(audioblock);
	release(audioblock);
}


