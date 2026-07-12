/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Synth.h"

namespace ParameterID
{
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    
    PARAMETER_ID(oscMorph1)
    PARAMETER_ID(oscMorph2)
    PARAMETER_ID(oscMorph3)
    PARAMETER_ID(oscMorph4)
    PARAMETER_ID(oscMorph5)
    PARAMETER_ID(oscFreq1)
    PARAMETER_ID(oscFreq2)
    PARAMETER_ID(oscFreq3)
    PARAMETER_ID(oscFreq4)
    PARAMETER_ID(oscFreq5)
    PARAMETER_ID(oscLevel1)
    PARAMETER_ID(oscLevel2)
    PARAMETER_ID(oscLevel3)
    PARAMETER_ID(oscLevel4)
    PARAMETER_ID(oscLevel5)
    PARAMETER_ID(oscPan1)
    PARAMETER_ID(oscPan2)
    PARAMETER_ID(oscPan3)
    PARAMETER_ID(oscPan4)
    PARAMETER_ID(oscPan5)

    PARAMETER_ID(lfoMorph1)
    PARAMETER_ID(lfoMorph2)
    PARAMETER_ID(lfoMorph3)
    PARAMETER_ID(lfoMorph4)
    PARAMETER_ID(lfoMorph5)
    PARAMETER_ID(lfoFreq1)
    PARAMETER_ID(lfoFreq2)
    PARAMETER_ID(lfoFreq3)
    PARAMETER_ID(lfoFreq4)
    PARAMETER_ID(lfoFreq5)
    PARAMETER_ID(lfoLevel1)
    PARAMETER_ID(lfoLevel2)
    PARAMETER_ID(lfoLevel3)
    PARAMETER_ID(lfoLevel4)
    PARAMETER_ID(lfoLevel5)
    PARAMETER_ID(lfoDest1)
    PARAMETER_ID(lfoDest2)
    PARAMETER_ID(lfoDest3)
    PARAMETER_ID(lfoDest4)
    PARAMETER_ID(lfoDest5)

    PARAMETER_ID(quantize)

    PARAMETER_ID(envAttack)
    PARAMETER_ID(envDecay)
    PARAMETER_ID(envSustain)
    PARAMETER_ID(envRelease)

    PARAMETER_ID(droneLevel)
    PARAMETER_ID(droneFreqTranspose)
    PARAMETER_ID(droneMorphTranspose)

    PARAMETER_ID(filterFreqL)
    PARAMETER_ID(filterResoL)
    PARAMETER_ID(filterFreqR)
    PARAMETER_ID(filterResoR)
    PARAMETER_ID(filterEnvDepth)

    PARAMETER_ID(droneHold)

    #undef PARAMETER_ID
}

//==============================================================================
/**
*/
class PentadroneAudioProcessor  : public juce::AudioProcessor,
                                  private juce::ValueTree::Listener
{
public:
    //==============================================================================
    PentadroneAudioProcessor();
    ~PentadroneAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PentadroneAudioProcessor)
        void splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
        void handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2);
        void render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset);
        void reset();
        Synth synth;

        //==============================================================================
        std::atomic<bool> parametersChanged{ false };
        juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
        void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override
        {
            parametersChanged.store(true);
        }
        void updateParams();

        juce::AudioParameterFloat* oscMorph1Param;
        juce::AudioParameterFloat* oscMorph2Param;
        juce::AudioParameterFloat* oscMorph3Param;
        juce::AudioParameterFloat* oscMorph4Param;
        juce::AudioParameterFloat* oscMorph5Param;

        juce::AudioParameterFloat* oscFreq1Param;
        juce::AudioParameterFloat* oscFreq2Param;
        juce::AudioParameterFloat* oscFreq3Param;
        juce::AudioParameterFloat* oscFreq4Param;
        juce::AudioParameterFloat* oscFreq5Param;

        juce::AudioParameterFloat* oscLevel1Param;
        juce::AudioParameterFloat* oscLevel2Param;
        juce::AudioParameterFloat* oscLevel3Param;
        juce::AudioParameterFloat* oscLevel4Param;
        juce::AudioParameterFloat* oscLevel5Param;

        juce::AudioParameterFloat* oscPan1Param;
        juce::AudioParameterFloat* oscPan2Param;
        juce::AudioParameterFloat* oscPan3Param;
        juce::AudioParameterFloat* oscPan4Param;
        juce::AudioParameterFloat* oscPan5Param;

        juce::AudioParameterFloat* envAttackParam;
        juce::AudioParameterFloat* envDecayParam; //might remove
        juce::AudioParameterFloat* envSustainParam; //might remove
        juce::AudioParameterFloat* envReleaseParam;

        juce::AudioParameterFloat* droneLevelParam;
        juce::AudioParameterFloat* droneFreqTransposeParam;
        juce::AudioParameterInt* droneMorphTransposeParam;

        juce::AudioParameterFloat* filterFreqLParam;
        juce::AudioParameterFloat* filterResoLParam;
        juce::AudioParameterFloat* filterFreqRParam;
        juce::AudioParameterFloat* filterResoRParam;

        juce::AudioParameterBool* droneHoldParam;

        juce::AudioParameterFloat* filterEnvDepthParam;

        juce::AudioParameterBool* quantizeParam;


        juce::AudioParameterFloat* lfoMorph1Param;
        juce::AudioParameterFloat* lfoMorph2Param;
        juce::AudioParameterFloat* lfoMorph3Param;
        juce::AudioParameterFloat* lfoMorph4Param;
        juce::AudioParameterFloat* lfoMorph5Param;

        juce::AudioParameterFloat* lfoFreq1Param;
        juce::AudioParameterFloat* lfoFreq2Param;
        juce::AudioParameterFloat* lfoFreq3Param;
        juce::AudioParameterFloat* lfoFreq4Param;
        juce::AudioParameterFloat* lfoFreq5Param;

        juce::AudioParameterFloat* lfoLevel1Param;
        juce::AudioParameterFloat* lfoLevel2Param;
        juce::AudioParameterFloat* lfoLevel3Param;
        juce::AudioParameterFloat* lfoLevel4Param;
        juce::AudioParameterFloat* lfoLevel5Param;

        juce::AudioParameterChoice* lfoDest1Param;
        juce::AudioParameterChoice* lfoDest2Param;
        juce::AudioParameterChoice* lfoDest3Param;
        juce::AudioParameterChoice* lfoDest4Param;
        juce::AudioParameterChoice* lfoDest5Param;
};
