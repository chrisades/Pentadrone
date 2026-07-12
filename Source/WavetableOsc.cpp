/*
  ==============================================================================

    WavetableOsc.cpp
    Created: 21 Jan 2026 3:12:36pm
    Author:  user

  ==============================================================================
*/

#include "WavetableOsc.h"

//to avoid large wtPhaseInc values, as the oscillator frequency
//increases, the table length should decrease.

WavetableOsc::WavetableOsc(const std::vector<std::vector<std::vector<float>>> &waveTables, float sampleRate, float interval, float minFreq, float maxFreq)
    : waveTables{ waveTables },
    sampleRate{ sampleRate },
    interval{ interval },
    minFreq{ minFreq },
    maxFreq{ maxFreq }
{
    inverseSampleRate = 1.0f / sampleRate;

    multiplier_1 = 1.0f / minFreq;
    multiplier_2 = 1.0f / logf(std::powf(2.0f, interval / 12.0f));
}

float WavetableOsc::getSample()
{
    index = std::fmod(index, static_cast<float>(WAVETABLE_SIZE));
    const auto sample = interpolateHermite4pt3oX();
    //const auto sample = interpolateLinearly();
    index += indexIncrement;
    return level * sample;
}

void WavetableOsc::setMorph(int morph_)
{
    if (morph_ < 0) {
        //morph = -1 * morph_;
        morph = 0;
    }
    else if (morph_ > (NUM_WAVE_MORPH - 1)) {
        //morph = 2 * (NUM_WAVE_MORPH - 1) - morph_;
        morph = (NUM_WAVE_MORPH - 1);
    }
    else {
        morph = morph_;
    }
}

void WavetableOsc::setFrequency(float frequency)
{
    if (frequency < minFreq) {
        frequency = minFreq;
    }
    else if (frequency > maxFreq) {
        frequency = maxFreq;
    }

    // update hamronic index based on frequency given
    harmonicIndex = static_cast<int>(std::ceilf(std::logf(frequency * multiplier_1) * multiplier_2));

    indexIncrement = frequency * static_cast<float>(WAVETABLE_SIZE) * inverseSampleRate;
}

bool WavetableOsc::isPlaying() const
{
    return indexIncrement != 0.0f;
}

void WavetableOsc::stop()
{
    index = 0.f;
    indexIncrement = 0.0f;
}

float WavetableOsc::interpolateLinearly() const
{
    const auto i0 = static_cast<size_t>(index);
    const auto i1 = (i0 + 1) & (WAVETABLE_SIZE - 1);
    const auto x = index - static_cast<float>(i0);
    return waveTables[harmonicIndex][morph][i1] * x + (1.0f - x) * waveTables[harmonicIndex][morph][i0];
}

float WavetableOsc::interpolateHermite4pt3oX() const //can this be made constexpr?
{
    //const auto i0 = static_cast<size_t>(index);
    //const auto im1 = (i0 + WAVETABLE_SIZE - 1) % WAVETABLE_SIZE;
    //const auto i1 = static_cast<size_t>(std::ceil(index)) % WAVETABLE_SIZE;
    //const auto i2 = static_cast<size_t>(std::ceil(index) + 1) % WAVETABLE_SIZE;
    //const auto x = index - static_cast<float>(i0);

    //make sure WAVETABLE_SIZE is a power of 2 for the & optimization (instead of using %)
    const auto i0 = static_cast<size_t>(index);
    const auto im1 = (i0 - 1) & (WAVETABLE_SIZE - 1); 
    const auto i1 = (i0 + 1) & (WAVETABLE_SIZE - 1);
    const auto i2 = (i0 + 2) & (WAVETABLE_SIZE - 1);
    const auto x = index - static_cast<float>(i0);
    float sample_i0 = waveTables[harmonicIndex][morph][i0]; //see if calling a pointer is faster
    float sample_im1 = waveTables[harmonicIndex][morph][im1];
    float sample_i1 = waveTables[harmonicIndex][morph][i1];
    float sample_i2 = waveTables[harmonicIndex][morph][i2];
    float c0 = sample_i0;
    float c1 = 0.5f * (sample_i1 - sample_im1);
    float c2 = sample_im1 - 2.5f * sample_i0 + 2.0f * sample_i1 - 0.5f * sample_i2;
    float c3 = 0.5f * (sample_i2 - sample_im1) + 1.5f * (sample_i0 - sample_i1);
    return ((c3 * x + c2) * x + c1) * x + c0;

    //const auto i0 = static_cast<size_t>(index);
    //const auto im1 = (i0 - 1) & (WAVETABLE_SIZE - 1);
    //const auto i1 = (i0 + 1) & (WAVETABLE_SIZE - 1);
    //const auto i2 = (i0 + 2) & (WAVETABLE_SIZE - 1);
    //const auto x = index - static_cast<float>(i0);
    //return (((0.5f * (waveTables[harmonicIndex][morph][i2] - waveTables[harmonicIndex][morph][im1]) + 1.5f
    //    * (waveTables[harmonicIndex][morph][i0] - waveTables[harmonicIndex][morph][i1])) * x +
    //    (waveTables[harmonicIndex][morph][im1] - 2.5f * waveTables[harmonicIndex][morph][i0] + 2.0f
    //        * waveTables[harmonicIndex][morph][i1] - 0.5f * waveTables[harmonicIndex][morph][i2])) * x +
    //    (0.5f * (waveTables[harmonicIndex][morph][i1] - waveTables[harmonicIndex][morph][im1]))) * x +
    //    waveTables[harmonicIndex][morph][i0];
}


