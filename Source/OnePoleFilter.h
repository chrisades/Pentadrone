/*
  ==============================================================================

    OnePoleFilter.h
    Created: 6 Feb 2026 12:34:45pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <cmath>

class OnePoleFilter {
public:
    OnePoleFilter(float smoothingTimeInMs, float samplingRate)
    {
        a = std::exp(-2.0f * PI / (smoothingTimeInMs * 0.001f * samplingRate));
        b = 1.0f - a;
        z = 0.0f;
    }

    //~OnePoleFilter(){}

    inline float process(float in)
    {
        z = (in * b) + z * a;
        return z;
    }

private:
    float a;
    float b;
    float z;

};