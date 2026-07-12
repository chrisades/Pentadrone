/*
  ==============================================================================

    Synth.cpp
    Created: 21 Jan 2026 1:16:52pm
    Author:  user

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"
//#include <chrono>

Synth::Synth()
{
}

//==============================================================================

void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/)
{
    float sampleRate = static_cast<float>(sampleRate_);

    //auto start = std::chrono::high_resolution_clock::now();
    const auto waveTables = WTs.generateAllWavetables(sampleRate, NUM_WAVE_MORPH, WAVETABLE_SIZE, INTERVAL, MIN_FREQ, MAX_FREQ, 2);
    //auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double, std::milli> float_ms = end - start;
    //DBG(float_ms.count());
    const auto LFOwaveTables = WTs.generateAllWavetables(sampleRate, NUM_WAVE_MORPH, WAVETABLE_SIZE, LFO_INTERVAL, LFO_MIN_FREQ, LFO_MAX_FREQ, 3);

    for (int d = 0; d < MAX_DRONES; ++d)
    {
        drones[d].initialize(waveTables, LFOwaveTables, sampleRate); //give the drones the wavtables
        drones[d].filterL.sampleRate = sampleRate; //initialize filters
        drones[d].filterR.sampleRate = sampleRate;
    }

    droneLevelSmoother.emplace(0.6f, sampleRate);
}

void Synth::deallocateResources()
{
    //do nothing
}

void Synth::reset()
{
    //reset params
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];

    for (int d = 0; d < MAX_DRONES; ++d)
    {
        Drone& drone = drones[d];
        if (drone.env.isActive() && (d == droneLastPlayed)) { //set drone parameters here for most recently active drone
            drone.setMorphs(oscillatorsMorph, droneMorphTranspose); 
            drone.setFrequencies(oscillatorsFrequency, droneFreqTranspose);
            drone.setPans(oscillatorsPan);
            drone.setLevels(oscillatorsLevel, droneLevel);
            drone.updateFilterParams(cutoffL, cutoffR, filterQL, filterQR, filterEnvDepth);

            drone.setLfoMorphs(lfosMorph);
            drone.setLfoFrequencies(lfosFrequency);
            drone.setLfoLevels(lfosLevel);
            updateDestinations();
        }
    }

    for (int sample = 0; sample < sampleCount; ++sample) {
        //updateLFOs();

        float outputLeft = 0.0f;
        float outputRight = 0.0f;

        for (int d = 0; d < MAX_DRONES; ++d)
        {
            Drone& drone = drones[d];
            if (drone.env.isActive()) {
                float droneL = 0.0f;
                float droneR = 0.0f;
                drone.render(droneL, droneR);

                outputLeft += droneL;
                outputRight += droneR;
            }
        }

        float outputLevel = 0.2; // turn into GUI parameter
        outputLeft *= outputLevel;
        outputRight *= outputLevel;

        if (outputBufferRight != nullptr) {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        }
        else {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5;
        }
    }

    for (int d = 0; d < MAX_DRONES; ++d)
    {
        Drone& drone = drones[d];
        if (!drone.env.isActive()) {
            drone.stop();
        }
    }

    //for testing purposes, protect ears and hardware
    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}

void Synth::startDrone(int droneIndex)
{
    Drone& drone = drones[droneIndex];

    //drone.setMorphs(oscillatorsMorph, droneMorphTranspose); //must change to stored parameters for each drone
    //drone.setFrequencies(oscillatorsFrequency, droneFreqTranspose); //must change to stored parameters for each drone
    //drone.setLevels(oscillatorsLevel); //must change to stored parameters for each drone
    //drone.level = droneLevel;
    //drone.filter.updateCoefficients(cutoff, filterQ);

    Envelope& env = drone.env;
    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
}

void Synth::releaseDrones() // called by PluginProcessor when hold is deselected after selection
{
    for (int d = 0; d < MAX_DRONES; ++d)
    {
        Drone& drone = drones[d];
        isHolding[d] = false;
        drone.release();
    }
}

void Synth::updateDestinations() //Update LFO destinations, destinations should be the same for all drones
{
    for (int l = 0; l < LFO_COUNT; ++l) {
        for (int d = 0; d < MAX_DRONES; ++d) {
            if (!skipDestUpdate[l]) {
                drones[d].setModulator(lfosDest[l], l);
            }
        }
    }
}

// Receives and handles MIDI data
void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0 & 0xF0) {
        // Note off : data0 == 80
        case 0x80: {
            noteOff(data1 & 0x7F);
            break;
        }

        // Note on : data0 == 90
        case 0x90: {
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0) {
                noteOn(note, velo);
            }
            else {
                noteOff(note);
            }
            break;
        }

        // Control change : data0 == B0
        case 0xB0: {
            //controlChange(data1, data2);
            break;
        }
    }
}
//==============================================================================

void Synth::noteOn(int note, int /*velocity*/)
{   
    int d;
    bool triggered = false;
    if (note == 60) { d = 0; triggered = true; } //triggered drone 1
    else if (note == 62) { d = 1; triggered = true; } //triggered drone 2
    else if (note == 64) { d = 2; triggered = true; } //triggered drone 3
    else if (note == 65) { d = 3; triggered = true; } //triggered drone 4
    else if (note == 67) { d = 4; triggered = true; } //triggered drone 5

    if (triggered) {
        if (!hold) {
            droneLastPlayed = d;
            startDrone(d);
        }
        else if(hold && !isHolding[d]) {
            droneLastPlayed = d;
            startDrone(d);
        }

    }
}

void Synth::noteOff(int note)
{
    int d;
    bool triggered = false;
    if (note == 60) { d = 0; triggered = true; } //triggered drone 1
    else if (note == 62) { d = 1; triggered = true; } //triggered drone 2
    else if (note == 64) { d = 2; triggered = true; } //triggered drone 3
    else if (note == 65) { d = 3; triggered = true; } //triggered drone 4
    else if (note == 67) { d = 4; triggered = true; } //triggered drone 5

    if (triggered) {
        if (!hold) {
            drones[d].release();
        }

        if (hold && !isHolding[d]) {
            isHolding[d] = true;
        }
        else if (hold && isHolding[d]) {
            isHolding[d] = false;
            drones[d].release();
        }
    }
}


