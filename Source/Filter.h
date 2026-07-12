/*
  ==============================================================================

    Filter.h
    Created: 4 Feb 2026 8:10:30pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <JuceHeader.h>
#include "Parameters.h"

class Filter : public juce::dsp::LadderFilter<float>
{
public:
    float sampleRate;

    void updateCoefficients(float cutoff, float Q)
    {
        g = std::tan(PI * std::clamp(cutoff, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX) / sampleRate);
        k = 1.0f / Q;
        a1 = 1.0f / (1.0f + g * (g + k));
        a2 = g * a1;
        a3 = g * a2;
        //setCutoffFrequencyHz(cutoff);
        //setResonance(std::clamp(Q / 30.0f, 0.0f, 1.0f));
    }

    void reset()
    {
        g = 0.0f;
        k = 0.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        a3 = 0.0f;
        ic1eq = 0.0f;
        ic2eq = 0.0f;
    }

    float render(float x)
    {
        float v3 = x - ic2eq;
        float v1 = a1 * ic1eq + a2 * v3;
        float v2 = ic2eq + a2 * ic1eq + a3 * v3;
        ic1eq = 2.0 * v1 - ic1eq;
        ic2eq = 2.0 * v2 - ic2eq;
        return v2;
        
        updateSmoothers();
        return processSample(x, 0);
    }

private:
    const float PI = 3.1415926535897932f;
    float g, k, a1, a2, a3; // filter coefficients
    float ic1eq, ic2eq; //internal state (charge of the capacitors)
};




//float render(float x, float morph)
//{
//    float v3 = x - ic2eq;
//    float v1 = a1 * ic1eq + a2 * v3;
//    float v2 = ic2eq + a2 * ic1eq + a3 * v3;
//    ic1eq = 2.0f * v1 - ic1eq;
//    ic2eq = 2.0f * v2 - ic2eq;
//
//    float lp = v2;
//    float bp = v1;
//    float hp = x - k * v1 - v2;
//
//    // morph 0->1: LP to BP, morph 1->2: BP to HP
//    if (morph <= 1.0f)
//        return lp * (1.0f - morph) + bp * morph;
//    else
//        return bp * (2.0f - morph) + hp * (morph - 1.0f);
//}

//float render(float x, float morph) // morph: 0.0 = LP, 0.5 = BP, 1.0 = HP
//{
//    float v3 = x - ic2eq;
//    float v1 = a1 * ic1eq + a2 * v3;
//    float v2 = ic2eq + a2 * ic1eq + a3 * v3;
//    ic1eq = 2.0f * v1 - ic1eq;
//    ic2eq = 2.0f * v2 - ic2eq;
//
//    float lp = v2;
//    float bp = v1;
//    float hp = x - k * v1 - v2;
//
//    float t = morph * 2.0f; // remap to 0-2
//    if (t <= 1.0f)
//        return lp * (1.0f - t) + bp * t;
//    else
//        return bp * (2.0f - t) + hp * (t - 1.0f);
//}

//Bandpass amplitude — the BP output peaks lower than LP / HP at unity, 
//so you may want to compensate by scaling bp up by 
//Q(or some factor) to level - match the outputs perceptually when morphing.
//
//Notch is also available as lp + hp(i.e.x - k * v1) if you want to extend the morph range further.


