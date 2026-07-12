/*
  ==============================================================================

    Quantizer.h
    Created: 8 Feb 2026 6:29:35pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <vector>

class Quantizer {
public:
    Quantizer()
    {
        quantizedFreqs.clear();
        float multiplier = std::powf(2.0f, 1.0f / 12.0f);

        pitch = MIN_FREQ;
        float limit = MAX_FREQ * multiplier;

        while (pitch <= limit) {
            quantizedFreqs.push_back(pitch);
            pitch *= multiplier;
        }
    }

    float quantize(float freq)
    {
        int i = 0;

        //assuming max and min freqs are already a quantized frequency
        if (freq >= MAX_FREQ)
        {
            return MAX_FREQ;
        }

        if (freq <= MIN_FREQ)
        {
            return MIN_FREQ;
        }

        while (quantizedFreqs[i] < MAX_FREQ) {

            if ((freq > quantizedFreqs[i]) && (freq <= quantizedFreqs[i + 1])) {
                if ((freq - quantizedFreqs[i]) <= (quantizedFreqs[i + 1] - freq)) {
                    return quantizedFreqs[i];
                }
                else { return quantizedFreqs[i + 1]; }
            }

            ++i;
        }

        return freq; // if nothing happens
    }


private:
    std::vector<float> quantizedFreqs;
    float pitch;
};