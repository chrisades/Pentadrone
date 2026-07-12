/*
  ==============================================================================

    WavetableOsc.h
    Created: 21 Jan 2026 3:12:21pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include <vector>
#include <cmath>

constexpr int NUM_WAVE_MORPH = 127; // number of wavetables to morph through
constexpr int WAVETABLE_SIZE = 512; //number of samples for each wavetable
constexpr float INTERVAL = 3.0f; //the interval (in semi-tones) used to calculate "when" maximum harmonics will be recaculated (to avoid aliasing)
constexpr float MIN_FREQ = 27.5f;//30.868f; //minimum frequency that the synth will output: B0
constexpr float MAX_FREQ = 3951.1f; //maximum frequency that the synth will output: B7
constexpr float LFO_INTERVAL = 48.0f;
constexpr float LFO_MIN_FREQ = 0.50f;
constexpr float LFO_MAX_FREQ = 10.0f;

//512 x 512 x 84

class WavetableOsc
{
public:
    WavetableOsc(const std::vector<std::vector<std::vector<float>>> &waveTables, float sampleRate, float interval, float minFreq, float maxFreq);
    WavetableOsc(const WavetableOsc&) = delete;
    WavetableOsc& operator=(const WavetableOsc&) = delete;
    WavetableOsc(WavetableOsc&&) = default;
    WavetableOsc& operator=(WavetableOsc&&) = default;

    float getSample();
    void setFrequency(float frequency);
    void setMorph(int morph_);
    bool isPlaying() const;
    void stop();

    float level = 1.0f; // overall level for this oscillator
    int morph = 0; // chooses which of the morphed wavetables to use

private:
    float interpolateLinearly() const;
    float interpolateHermite4pt3oX() const;

    float minFreq;
    float maxFreq;
    float interval;

    float index = 0.0f;
    float indexIncrement = 0.0f;
    const std::vector<std::vector<std::vector<float>>> waveTables;
    float sampleRate;
    float inverseSampleRate;

    int harmonicIndex = 0; // chooses the wavetables with the max amount of harmonics to avoid aliasing

    //multipliers used in set_frequency, for efficiency
    float multiplier_1;
    float multiplier_2;
};
