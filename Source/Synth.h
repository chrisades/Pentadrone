/*
  ==============================================================================

    Synth.h
    Created: 21 Jan 2026 1:16:37pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Drone.h"
#include "WavetableOsc.h"
#include "Wavetables.h"
#include "OnePoleFilter.h"
#include "Quantizer.h"

static constexpr int MAX_DRONES = 5;

class Synth {
public:
    Synth();

    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateResources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void releaseDrones();

    int droneLastPlayed = 0;


    //Effects all drones
    bool hold = 0;
    Quantizer quantizer;
    float envAttack{ 0.0f }, envDecay{ 0.0f }, envSustain{ 0.0f }, envRelease{ 0.0f };

    //Drone parameters
    std::array<int, OSCILLATOR_COUNT> oscillatorsMorph{};
    std::array<float, OSCILLATOR_COUNT> oscillatorsLevel{};
    std::array<float, OSCILLATOR_COUNT> oscillatorsFrequency{};
    std::array<float, OSCILLATOR_COUNT> oscillatorsPan{};
    int droneMorphTranspose = 0;
    float droneLevel = 0.0f;
    float droneFreqTranspose = 0.0f;
    float cutoffL{ 0.0f }, cutoffR{ 0.0f }, filterQL{ 0.0f }, filterQR{ 0.0f }, filterEnvDepth{ 0.0f };

    std::array<int, LFO_COUNT> lfosMorph{};
    std::array<float, LFO_COUNT> lfosFrequency{};
    std::array<float, LFO_COUNT> lfosLevel{};
    std::array<int, LFO_COUNT> lfosDest{};
    std::array<bool, LFO_COUNT> skipDestUpdate{};
    
    void updateDestinations();

private:
    void noteOn(int note, int velocity);
    void noteOff(int note);
    Wavetables WTs;
    std::array<Drone, MAX_DRONES> drones;
    void startDrone(int droneIndex);
    std::array<bool, MAX_DRONES> isHolding{};
    std::optional <OnePoleFilter> droneLevelSmoother;
};