/*
  ==============================================================================

    Drone.h
    Created: 1 Feb 2026 10:01:00pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include "Wavetables.h"
#include "WavetableOsc.h"
#include "Envelope.h"
#include "Filter.h"
#include "Parameters.h"

constexpr int OSCILLATOR_COUNT = 5; //Oscillators per drone
constexpr int LFO_COUNT = 5;

class Drone 
{

public:
    Envelope env;

    //Filter
    Filter filterL;
    Filter filterR;

    // and then have start drone function that actually starts up the oscillators

    //combine all the parameter sets, to avoid the extra loops
    void setMorphs(std::array<int, OSCILLATOR_COUNT> oscillatorsMorph_, int morphTranspose_)
    {
        oscillatorsMorph = oscillatorsMorph_;
        morphTranspose = morphTranspose_;

        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            oscillators[i].setMorph(static_cast<int>(oscillatorsMorph[i] + (NUM_WAVE_MORPH - 1) * getMod(Osc_1_Morph + (i * 4)) + morphTranspose));
        }
    }

    void setFrequencies(std::array<float, OSCILLATOR_COUNT> oscillatorsFrequency_, float freqTranspose_)
    {
        oscillatorsFrequency = oscillatorsFrequency_;
        freqTranspose = freqTranspose_;
        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            oscillators[i].setFrequency((freqTranspose) * (oscillatorsFrequency[i] + MAX_FREQ * getMod(Osc_1_Pitch + (i * 4))));
        }
    }

    void setLevels(std::array<float, OSCILLATOR_COUNT> oscillatorsLevel_, float level_)
    {
        level = level_;
        oscillatorsLevel = oscillatorsLevel_;

        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            oscillators[i].level = oscillatorsLevel[i] + getMod(Osc_1_Level + (i * 4));
        }
    }

    void setPans(std::array<float, OSCILLATOR_COUNT> oscillatorsPan_)
    {
        oscillatorsPan = oscillatorsPan_;
        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            float pan = std::clamp(oscillatorsPan[i] + getMod(Osc_1_Pan + (i * 4)), -1.0f, 1.0f);
            pansLeft[i] = std::sin(PI_OVER_4 * (1.0f - pan));
            pansRight[i] = std::sin(PI_OVER_4 * (1.0f + pan));
        }
    }

    // LFO parameters
    void setLfoMorphs(std::array<int, LFO_COUNT> lfosMorph_)
    {
        lfosMorph = lfosMorph_;
    }
    void setLfoFrequencies(std::array<float, LFO_COUNT> lfosFrequency_)
    {
        lfosFrequency = lfosFrequency_;
    }
    void setLfoLevels(std::array<float, LFO_COUNT> lfosLevel_)
    {
        lfosLevel = lfosLevel_;
    }

    void setModulator(int param, int lfoIndex)
    {
        if (param == NONE)
        {
            // if the destination for an LFO is NONE
            // set all paramMod values associated with the lfo, equal to -1
            for (int i = 1; i < PARAM_CHOICE_COUNT; ++i) {
                if (paramMod[i] == lfoIndex) {
                    paramMod[i] = -1;
                }
            }
        }
        else { // else just set paramMod value
            paramMod[param] = lfoIndex;
        }
    }

    void lfoUpdate()
    {
        for (int l = 0; l < LFO_COUNT; ++l) {
            lfos[l].setFrequency(lfosFrequency[l] + LFO_MAX_FREQ * getMod(LFO_1_Speed + (l * 3)));
            lfos[l].setMorph(lfosMorph[l] + NUM_WAVE_MORPH * getMod(LFO_1_Morph + (l * 3)));
            lfos[l].level = lfosLevel[l] + getMod(LFO_1_Depth + (l * 3));
            lfoCurrentValues[l] = 0.5f * lfos[l].getSample(); //since LFOs can swing from -1 to +1, halve it. 
            // if a modulated parameter is set at its midpoint it should swing from its lowest value to its highest
        }
    }

 

    float getMod(int param)
    { 
        if (paramMod[param] == -1) // if parameter has no mod source
        {
            return 0.0f;
        }
        else
            return lfoCurrentValues[paramMod[param]];
    }

    void stop()
    {
        env.reset();
        filterL.reset();
        filterR.reset();
        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            oscillators[i].stop();
            lfos[i].stop();
        }
        for (int i = 0; i < LFO_COUNT; ++i)
        {
            lfos[i].stop();
        }
    }

    void render(float& outputL, float& outputR)
    {
        //outputL = 0.0f;
        //outputR = 0.0f;

        lfoUpdate();
        //oscUpdate();

        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            outputL += pansLeft[i] * oscillators[i].getSample();
            outputR += pansRight[i] * oscillators[i].getSample();
        }

        float envelope = env.nextValue();

        filterL.updateCoefficients((cutoffL + maxCutoffL * envelope) + FILTER_CUTOFF_MAX * getMod(Cutoff_L), filterQL);
        filterR.updateCoefficients((cutoffR + maxCutoffR * envelope) + FILTER_CUTOFF_MAX * getMod(Cutoff_R), filterQR);

        outputL = filterL.render(outputL) * (level + getMod(Drone_Level)) * oscCountInverse * envelope;
        outputR = filterR.render(outputR) * (level + getMod(Drone_Level)) * oscCountInverse * envelope;
    }

    void release() // Invoked during a noteoff event
    {
        env.release();
    }

    void initialize(const std::vector<std::vector<std::vector<float>>> &waveTables, const std::vector<std::vector<std::vector<float>>>& LFOwaveTables, float sampleRate)
    {
        oscillators.clear();
        for (int i = 0; i < OSCILLATOR_COUNT; ++i)
        {
            oscillators.emplace_back(waveTables, sampleRate, INTERVAL, MIN_FREQ, MAX_FREQ);
            lfos.emplace_back(LFOwaveTables, sampleRate, LFO_INTERVAL, LFO_MIN_FREQ, LFO_MAX_FREQ);
        }

        lfos.clear();
        for (int i = 0; i < LFO_COUNT; ++i)
        {
            lfos.emplace_back(LFOwaveTables, sampleRate, LFO_INTERVAL, LFO_MIN_FREQ, LFO_MAX_FREQ);
        }

        paramMod.fill(-1);
    }

    void updateFilterParams(float cutoffL_, float cutoffR_, float filterQL_, float filterQR_, float filterEnvDepth_)
    {
        cutoffL = cutoffL_;
        cutoffR = cutoffR_;
        filterQL = filterQL_;
        filterQR = filterQR_;
        filterEnvDepth = filterEnvDepth_;

        if (filterEnvDepth > 0.0f) {
            maxCutoffL = filterEnvDepth * (MAX_FREQ - cutoffL);
            maxCutoffR = filterEnvDepth * (MAX_FREQ - cutoffR);
        }
        else {
            maxCutoffL = filterEnvDepth * (cutoffL - MIN_FREQ);
            maxCutoffR = filterEnvDepth * (cutoffR - MIN_FREQ);
        }
    }

private:
    std::vector<WavetableOsc> oscillators;
    float oscCountInverse = 1.0f / static_cast<float>(OSCILLATOR_COUNT); //Normalizes the drone relative the total amount of oscillators
    float maxCutoffL;
    float maxCutoffR;

    //Morph
    std::array<int, OSCILLATOR_COUNT> oscillatorsMorph;
    int morphTranspose;
    
    //Frequency
    std::array<float, OSCILLATOR_COUNT> oscillatorsFrequency;
    float freqTranspose;

    //Pans
    std::array<float, OSCILLATOR_COUNT> oscillatorsPan;
    std::array<float, OSCILLATOR_COUNT> pansLeft;
    std::array<float, OSCILLATOR_COUNT> pansRight;

    //Levels
    std::array<float, OSCILLATOR_COUNT> oscillatorsLevel;
    float level;

    //Filter
    float cutoffL;
    float cutoffR;
    float filterQL;
    float filterQR;
    float filterEnvDepth;

    //LFOs
    std::vector<WavetableOsc> lfos;
    std::array<int, LFO_COUNT> lfosMorph;
    std::array<float, LFO_COUNT> lfosFrequency;
    std::array<float, LFO_COUNT> lfosLevel;
    std::array<int, PARAM_CHOICE_COUNT> paramMod; // stores lfo indeces to know which parameters are modulated by which lfo. = -1 if parameter is not being moded
    std::array<float, LFO_COUNT> lfoCurrentValues{}; //stores a copy of the current value of each lfo. initialized to 0
    std::array<bool, LFO_COUNT> lfoHasDestination{};
};