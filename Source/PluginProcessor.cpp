/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"

//==============================================================================
PentadroneAudioProcessor::PentadroneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    castParameter(apvts, ParameterID::oscMorph1, oscMorph1Param);
    castParameter(apvts, ParameterID::oscMorph2, oscMorph2Param);
    castParameter(apvts, ParameterID::oscMorph3, oscMorph3Param);
    castParameter(apvts, ParameterID::oscMorph4, oscMorph4Param);
    castParameter(apvts, ParameterID::oscMorph5, oscMorph5Param);
    castParameter(apvts, ParameterID::oscFreq1, oscFreq1Param);
    castParameter(apvts, ParameterID::oscFreq2, oscFreq2Param);
    castParameter(apvts, ParameterID::oscFreq3, oscFreq3Param);
    castParameter(apvts, ParameterID::oscFreq4, oscFreq4Param);
    castParameter(apvts, ParameterID::oscFreq5, oscFreq5Param);
    castParameter(apvts, ParameterID::oscLevel1, oscLevel1Param);
    castParameter(apvts, ParameterID::oscLevel2, oscLevel2Param);
    castParameter(apvts, ParameterID::oscLevel3, oscLevel3Param);
    castParameter(apvts, ParameterID::oscLevel4, oscLevel4Param);
    castParameter(apvts, ParameterID::oscLevel5, oscLevel5Param);
    castParameter(apvts, ParameterID::oscPan1, oscPan1Param);
    castParameter(apvts, ParameterID::oscPan2, oscPan2Param);
    castParameter(apvts, ParameterID::oscPan3, oscPan3Param);
    castParameter(apvts, ParameterID::oscPan4, oscPan4Param);
    castParameter(apvts, ParameterID::oscPan5, oscPan5Param);

    castParameter(apvts, ParameterID::lfoMorph1, lfoMorph1Param);
    castParameter(apvts, ParameterID::lfoMorph2, lfoMorph2Param);
    castParameter(apvts, ParameterID::lfoMorph3, lfoMorph3Param);
    castParameter(apvts, ParameterID::lfoMorph4, lfoMorph4Param);
    castParameter(apvts, ParameterID::lfoMorph5, lfoMorph5Param);
    castParameter(apvts, ParameterID::lfoFreq1, lfoFreq1Param);
    castParameter(apvts, ParameterID::lfoFreq2, lfoFreq2Param);
    castParameter(apvts, ParameterID::lfoFreq3, lfoFreq3Param);
    castParameter(apvts, ParameterID::lfoFreq4, lfoFreq4Param);
    castParameter(apvts, ParameterID::lfoFreq5, lfoFreq5Param);
    castParameter(apvts, ParameterID::lfoLevel1, lfoLevel1Param);
    castParameter(apvts, ParameterID::lfoLevel2, lfoLevel2Param);
    castParameter(apvts, ParameterID::lfoLevel3, lfoLevel3Param);
    castParameter(apvts, ParameterID::lfoLevel4, lfoLevel4Param);
    castParameter(apvts, ParameterID::lfoLevel5, lfoLevel5Param);
    castParameter(apvts, ParameterID::lfoDest1, lfoDest1Param);
    castParameter(apvts, ParameterID::lfoDest2, lfoDest2Param);
    castParameter(apvts, ParameterID::lfoDest3, lfoDest3Param);
    castParameter(apvts, ParameterID::lfoDest4, lfoDest4Param);
    castParameter(apvts, ParameterID::lfoDest5, lfoDest5Param);

    castParameter(apvts, ParameterID::envAttack, envAttackParam);
    castParameter(apvts, ParameterID::envDecay, envDecayParam);
    castParameter(apvts, ParameterID::envSustain, envSustainParam);
    castParameter(apvts, ParameterID::envRelease, envReleaseParam);

    castParameter(apvts, ParameterID::droneLevel, droneLevelParam);
    castParameter(apvts, ParameterID::droneFreqTranspose, droneFreqTransposeParam);
    castParameter(apvts, ParameterID::droneMorphTranspose, droneMorphTransposeParam);

    castParameter(apvts, ParameterID::filterFreqL, filterFreqLParam);
    castParameter(apvts, ParameterID::filterResoL, filterResoLParam);
    castParameter(apvts, ParameterID::filterFreqR, filterFreqRParam);
    castParameter(apvts, ParameterID::filterResoR, filterResoRParam);
    castParameter(apvts, ParameterID::filterEnvDepth, filterEnvDepthParam);

    castParameter(apvts, ParameterID::droneHold, droneHoldParam);
    castParameter(apvts, ParameterID::quantize, quantizeParam);

    apvts.state.addListener(this);
}

PentadroneAudioProcessor::~PentadroneAudioProcessor()
{
    apvts.state.removeListener(this);
}

//==============================================================================
const juce::String PentadroneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PentadroneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PentadroneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PentadroneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PentadroneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PentadroneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PentadroneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PentadroneAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String PentadroneAudioProcessor::getProgramName(int index)
{
    return {};
}

void PentadroneAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void PentadroneAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.allocateResources(sampleRate, samplesPerBlock);
    parametersChanged.store(true);
    reset();
}

void PentadroneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    synth.deallocateResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PentadroneAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void PentadroneAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    bool expected = true;
    if (parametersChanged.compare_exchange_strong(expected, false)) {
        updateParams();
    }

    splitBufferByEvents(buffer, midiMessages);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }
}

void PentadroneAudioProcessor::splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int bufferOffset = 0;

    for (const auto metadata : midiMessages) {
        int samplesThisSegment = metadata.samplePosition - bufferOffset;
        if (samplesThisSegment > 0) {
            render(buffer, samplesThisSegment, bufferOffset);
            bufferOffset += samplesThisSegment;
        }

        //Handle event. Ignore MIDI messages such as sysex
        if (metadata.numBytes <= 3) {
            uint8_t data1 = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
            uint8_t data2 = (metadata.numBytes == 3) ? metadata.data[2] : 0;
            handleMIDI(metadata.data[0], data1, data2);
        }
    }

    // Render the audio after the last MIDI event
    // if there were no MIDI events at all, 
    // this renders the entire buffer
    int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
    if (samplesLastSegment > 0) {
        render(buffer, samplesLastSegment, bufferOffset);
    }
    midiMessages.clear();
}

void PentadroneAudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2)
{
    //char s[16];
    //snprintf(s, 16, "%02hhX %02hhX %02hhX", data0, data1, data2);
    //DBG(s);

    //data0 data1 data2
    //90 30 04
    //90 = note on, 30 = Note C2, 6A = Velocity of 106

    //// Control Change
    //if ((data0 & 0xF0) == 0xB0) {
    //    if (data1 == /*0x07*/) {
    //        //do something
    //    }
    //}

    synth.midiMessage(data0, data1, data2);
}

void PentadroneAudioProcessor::render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset)
{
    float* outputBuffers[2] = { nullptr, nullptr };
    outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset;
    if (getTotalNumOutputChannels() > 1) {
        outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
    }
    synth.render(outputBuffers, sampleCount);
}

void PentadroneAudioProcessor::updateParams()
{
    //parameters should only be updated if moved



    float sampleRate = float(getSampleRate());
    float inverseSampleRate = 1.0f / sampleRate;

    synth.envAttack =
        std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * envAttackParam->get()));

    synth.envDecay =
        std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * envDecayParam->get()));

    synth.envSustain = envSustainParam->get() / 100.0f;

    float envRelease = envReleaseParam->get();
    if (envRelease < 1.0f) {
        synth.envRelease = 0.75; //extra fast release
    }
    else {
        synth.envRelease = std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * envRelease));
    }
    
    // Left Filter
    float filterResoL = filterResoLParam->get() / 100.0f;
    synth.filterQL = 1.0f / ((1.0f - filterResoL + 1e-9) * (1.0f - filterResoL + 1e-9));
    synth.cutoffL = filterFreqLParam->get();

    // Right Filter
    float filterResoR = filterResoRParam->get() / 100.0f;
    synth.filterQR = 1.0f / ((1.0f - filterResoR + 1e-9) * (1.0f - filterResoR + 1e-9));
    synth.cutoffR = filterFreqRParam->get();

    synth.filterEnvDepth = filterEnvDepthParam->get() / 100.0f;

    synth.droneMorphTranspose = ((NUM_WAVE_MORPH - 1) / 100) * droneMorphTransposeParam->get();


    synth.oscillatorsMorph[0] = static_cast<int>((NUM_WAVE_MORPH - 1) * oscMorph1Param->get() / 100.f);
    synth.oscillatorsMorph[1] = static_cast<int>((NUM_WAVE_MORPH - 1) * oscMorph2Param->get() / 100.f);
    synth.oscillatorsMorph[2] = static_cast<int>((NUM_WAVE_MORPH - 1) * oscMorph3Param->get() / 100.f);
    synth.oscillatorsMorph[3] = static_cast<int>((NUM_WAVE_MORPH - 1) * oscMorph4Param->get() / 100.f);
    synth.oscillatorsMorph[4] = static_cast<int>((NUM_WAVE_MORPH - 1) * oscMorph5Param->get() / 100.f);
    
    if (quantizeParam->get())
    {
        synth.oscillatorsFrequency[0] = synth.quantizer.quantize(oscFreq1Param->get());
        synth.oscillatorsFrequency[1] = synth.quantizer.quantize(oscFreq2Param->get());
        synth.oscillatorsFrequency[2] = synth.quantizer.quantize(oscFreq3Param->get());
        synth.oscillatorsFrequency[3] = synth.quantizer.quantize(oscFreq4Param->get());
        synth.oscillatorsFrequency[4] = synth.quantizer.quantize(oscFreq5Param->get());
    }
    else {
        synth.oscillatorsFrequency[0] = oscFreq1Param->get();
        synth.oscillatorsFrequency[1] = oscFreq2Param->get();
        synth.oscillatorsFrequency[2] = oscFreq3Param->get();
        synth.oscillatorsFrequency[3] = oscFreq4Param->get();
        synth.oscillatorsFrequency[4] = oscFreq5Param->get();
    }

    synth.oscillatorsPan[0] = oscPan1Param->get() / 100.0f;
    synth.oscillatorsPan[1] = oscPan2Param->get() / 100.0f;
    synth.oscillatorsPan[2] = oscPan3Param->get() / 100.0f;
    synth.oscillatorsPan[3] = oscPan4Param->get() / 100.0f;
    synth.oscillatorsPan[4] = oscPan5Param->get() / 100.0f;

    float a = 12.0f;
    synth.oscillatorsLevel[0] = (powf(a, 0.01f * oscLevel1Param->get()) - 1.0f) / (a - 1);
    synth.oscillatorsLevel[1] = (powf(a, 0.01f * oscLevel2Param->get()) - 1.0f) / (a - 1);
    synth.oscillatorsLevel[2] = (powf(a, 0.01f * oscLevel3Param->get()) - 1.0f) / (a - 1);
    synth.oscillatorsLevel[3] = (powf(a, 0.01f * oscLevel4Param->get()) - 1.0f) / (a - 1);
    synth.oscillatorsLevel[4] = (powf(a, 0.01f * oscLevel5Param->get()) - 1.0f) / (a - 1);

    synth.droneLevel = (powf(a, 0.01f * droneLevelParam->get()) - 1.0f) / (a - 1);
    synth.droneFreqTranspose = (0.007f * droneFreqTransposeParam->get()) + 1.2f; //linear formula to scale [-100, 100] to [1/2, 2]

    synth.lfosMorph[0] = static_cast<int>((NUM_WAVE_MORPH - 1) * lfoMorph1Param->get() / 100.f);
    synth.lfosMorph[1] = static_cast<int>((NUM_WAVE_MORPH - 1) * lfoMorph2Param->get() / 100.f);
    synth.lfosMorph[2] = static_cast<int>((NUM_WAVE_MORPH - 1) * lfoMorph3Param->get() / 100.f);
    synth.lfosMorph[3] = static_cast<int>((NUM_WAVE_MORPH - 1) * lfoMorph4Param->get() / 100.f);
    synth.lfosMorph[4] = static_cast<int>((NUM_WAVE_MORPH - 1) * lfoMorph5Param->get() / 100.f);
    synth.lfosFrequency[0] = lfoFreq1Param->get();
    synth.lfosFrequency[1] = lfoFreq2Param->get();
    synth.lfosFrequency[2] = lfoFreq3Param->get();
    synth.lfosFrequency[3] = lfoFreq4Param->get();
    synth.lfosFrequency[4] = lfoFreq5Param->get();
    synth.lfosLevel[0] = (powf(a, 0.01f * lfoLevel1Param->get()) - 1.0f) / (a - 1);
    synth.lfosLevel[1] = (powf(a, 0.01f * lfoLevel2Param->get()) - 1.0f) / (a - 1);
    synth.lfosLevel[2] = (powf(a, 0.01f * lfoLevel3Param->get()) - 1.0f) / (a - 1);
    synth.lfosLevel[3] = (powf(a, 0.01f * lfoLevel4Param->get()) - 1.0f) / (a - 1);
    synth.lfosLevel[4] = (powf(a, 0.01f * lfoLevel5Param->get()) - 1.0f) / (a - 1);


    if (lfoDest1Param->getIndex() == synth.lfosDest[0]) {
        synth.skipDestUpdate[0] = true;
    } else synth.skipDestUpdate[0] = false;

    if (lfoDest2Param->getIndex() == synth.lfosDest[1]) {
        synth.skipDestUpdate[1] = true;
    } else synth.skipDestUpdate[1] = false;

    if (lfoDest3Param->getIndex() == synth.lfosDest[2]) {
        synth.skipDestUpdate[2] = true;
    } else synth.skipDestUpdate[2] = false;

    if (lfoDest4Param->getIndex() == synth.lfosDest[3]) {
        synth.skipDestUpdate[3] = true;
    } else synth.skipDestUpdate[3] = false;

    if (lfoDest5Param->getIndex() == synth.lfosDest[4]) {
        synth.skipDestUpdate[4] = true;
    } else synth.skipDestUpdate[4] = false;

    synth.lfosDest[0] = lfoDest1Param->getIndex();
    synth.lfosDest[1] = lfoDest2Param->getIndex();
    synth.lfosDest[2] = lfoDest3Param->getIndex();
    synth.lfosDest[3] = lfoDest4Param->getIndex();
    synth.lfosDest[4] = lfoDest5Param->getIndex();

    if (synth.hold && !droneHoldParam->get())
    {
        synth.releaseDrones();
    }
    synth.hold = droneHoldParam->get();



}

void PentadroneAudioProcessor::reset()
{
    synth.reset();
}

//==============================================================================
bool PentadroneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PentadroneAudioProcessor::createEditor()
{
    //return new PentadroneAudioProcessorEditor (*this);
    auto editor = new juce::GenericAudioProcessorEditor(*this);
    editor->setSize(500, 500);
    return editor;
}

//==============================================================================
void PentadroneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PentadroneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PentadroneAudioProcessor();
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout PentadroneAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    float twelveTETMult = std::powf(2, 1.0f / 12.0f);
    juce::StringArray choices = { "NONE", "Cutoff L", "Cutoff R", "Reso L", "Reso R", "Drone Level", "Morph Transpose", "Freq Transpose",
    "Osc 1 Morph", "Osc 1 Pitch" ,"Osc 1 Pan" ,"Osc 1 Level" ,
    "Osc 2 Morph", "Osc 2 Pitch" ,"Osc 2 Pan" ,"Osc 2 Level" ,
    "Osc 3 Morph", "Osc 3 Pitch" ,"Osc 3 Pan" ,"Osc 3 Level" , 
    "Osc 4 Morph", "Osc 4 Pitch" ,"Osc 4 Pan" ,"Osc 4 Level" , 
    "Osc 5 Morph", "Osc 5 Pitch" ,"Osc 5 Pan" ,"Osc 5 Level" , 
    "LFO 1 Morph", "LFO 1 Speed", "LFO 1 Depth",
    "LFO 2 Morph", "LFO 2 Speed", "LFO 2 Depth", 
    "LFO 3 Morph", "LFO 3 Speed", "LFO 3 Depth", 
    "LFO 4 Morph", "LFO 4 Speed", "LFO 4 Depth", 
    "LFO 5 Morph", "LFO 5 Speed", "LFO 5 Depth"};

    //HOLD
    layout.add(std::make_unique<juce::AudioParameterBool>
        (ParameterID::droneHold, "Hold", false));

    //Quantize
    layout.add(std::make_unique<juce::AudioParameterBool>
        (ParameterID::quantize, "Quantize", false));

    //FILTER PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::filterFreqL, "Filter Cutoff Left", juce::NormalisableRange<float>(FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 0.1f, 0.7f, false),
            1000.0f,
            juce::AudioParameterFloatAttributes().withLabel("Hz")));

    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::filterFreqR, "Filter Cutoff Right", juce::NormalisableRange<float>(FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 0.1f, 0.7f, false),
            1000.0f,
            juce::AudioParameterFloatAttributes().withLabel("Hz")));

    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::filterResoL, "Filter Reso Left", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            15.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::filterResoR, "Filter Reso Right", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            15.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::filterEnvDepth, "Filter Env Depth", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));

    //DRONE LEVEL
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::droneLevel, "Drone Level", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            100.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));

    //DRONE MORPH TRANPOSE
    layout.add(std::make_unique<juce::AudioParameterInt>
        (ParameterID::droneMorphTranspose, "Morph Transpose", -100, 100,
            0,
            juce::AudioParameterIntAttributes().withLabel("%")));

    //DRONE FREQUENCY TRANSPOSE
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::droneFreqTranspose, "Freq Transpose", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    
    //OSC 1 PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscMorph1,
        "Osc1 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscFreq1,
        "Osc1 Freq",
        juce::NormalisableRange<float>(MIN_FREQ, MIN_FREQ * 20.0f * twelveTETMult, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscPan1,
        "Osc1 Pan",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscLevel1,
        "Osc1 Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    // OSC 2 PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscMorph2,
        "Osc2 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        65.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscFreq2,
        "Osc2 Freq",
        //juce::NormalisableRange<float>(MIN_FREQ * 12.0f * twelveTETMult, MIN_FREQ * 32.0f * twelveTETMult, 0.01f, 0.7f, false),
        juce::NormalisableRange<float>(MIN_FREQ, MIN_FREQ * 20.0f * twelveTETMult, 0.01f, 0.7f, false),
        MIN_FREQ * 1.05f,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscPan2,
        "Osc2 Pan",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscLevel2,
        "Osc2 Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //OSC 3 PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscMorph3,
        "Osc3 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        20.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscFreq3,
        "Osc3 Freq",
        juce::NormalisableRange<float>(MIN_FREQ, MIN_FREQ * 25.0f * twelveTETMult, 0.01f, 0.7f, false),
        //juce::NormalisableRange<float>(MIN_FREQ * 27.0f * twelveTETMult, MIN_FREQ * 48.0f * twelveTETMult, 0.01f, 0.7f, false),
        MIN_FREQ * 7.0f * twelveTETMult,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscPan3,
        "Osc3 Pan",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscLevel3,
        "Osc3 Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //OSC 4 PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscMorph4,
        "Osc4 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscFreq4,
        "Osc4 Freq",
        juce::NormalisableRange<float>(MIN_FREQ, MIN_FREQ * 30.0f * twelveTETMult, 0.01f, 0.7f, false),
        //juce::NormalisableRange<float>(MIN_FREQ * 41.0f * twelveTETMult, MIN_FREQ * 61.0f * twelveTETMult, 0.01f, 0.7f, false),
        MIN_FREQ * 12.0f * twelveTETMult,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscPan4,
        "Osc4 Pan",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscLevel4,
        "Osc4 Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //OSC 5 PARAMS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscMorph5,
        "Osc5 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscFreq5,
        "Osc5 Freq",
        juce::NormalisableRange<float>(MIN_FREQ, MIN_FREQ * 35.0f * twelveTETMult, 0.01f, 0.7f, false),
        //juce::NormalisableRange<float>(MIN_FREQ * 56.0f * twelveTETMult, MIN_FREQ * 75.0f * twelveTETMult, 0.01f, 0.7f, false),
        MIN_FREQ * 16.0f * twelveTETMult,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscPan5,
        "Osc5 Pan",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::oscLevel5,
        "Osc5 Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    // ENVELOPE
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::envAttack, "Env Attack", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            10.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::envDecay, "Env Decay", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            100.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::envSustain, "Env Sustain", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            100.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (ParameterID::envRelease, "Env Release", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
            10.0f,
            juce::AudioParameterFloatAttributes().withLabel("%")));

    //LFO1
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (ParameterID::lfoDest1, "LFO1 Destination", choices,
            0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoMorph1,
        "LFO1 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoFreq1,
        "LFO1 Speed",
        juce::NormalisableRange<float>(LFO_MIN_FREQ, LFO_MAX_FREQ, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoLevel1,
        "LFO1 Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //LFO2
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (ParameterID::lfoDest2, "LFO2 Destination", choices,
            0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoMorph2,
        "LFO2 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoFreq2,
        "LFO2 Speed",
        juce::NormalisableRange<float>(LFO_MIN_FREQ, LFO_MAX_FREQ, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoLevel2,
        "LFO2 Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //LFO3
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (ParameterID::lfoDest3, "LFO3 Destination", choices,
            0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoMorph3,
        "LFO3 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoFreq3,
        "LFO3 Speed",
        juce::NormalisableRange<float>(LFO_MIN_FREQ, LFO_MAX_FREQ, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoLevel3,
        "LFO3 Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //LFO4
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (ParameterID::lfoDest4, "LFO4 Destination", choices,
            0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoMorph4,
        "LFO4 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoFreq4,
        "LFO4 Speed",
        juce::NormalisableRange<float>(LFO_MIN_FREQ, LFO_MAX_FREQ, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoLevel4,
        "LFO4 Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    //LFO5
    layout.add(std::make_unique<juce::AudioParameterChoice>
        (ParameterID::lfoDest5, "LFO5 Destination", choices,
            0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoMorph5,
        "LFO5 Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoFreq5,
        "LFO5 Speed",
        juce::NormalisableRange<float>(LFO_MIN_FREQ, LFO_MAX_FREQ, 0.01f, 0.7f, false),
        MIN_FREQ,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::lfoLevel5,
        "LFO5 Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));


    return layout;
}