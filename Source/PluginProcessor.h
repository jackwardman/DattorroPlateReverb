/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DattorroReverbAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DattorroReverbAudioProcessor();
    ~DattorroReverbAudioProcessor() override;

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
    float lin_interp(float inSampleX, float inSampleY, float inFloatPhase);

private:
    
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mBandwidthParameter;
    AudioParameterFloat* mDampingParameter;
    AudioParameterFloat* mFeedbackParameter;
    AudioParameterFloat* mPreDelayParameter;

    AudioBuffer<float> mPreDelayDDL;
    int mPreDelayDDLLength;
    int mPreDelayWriteInx;
    
    int sdel1;
    AudioBuffer<float> DDL1;
    int rwInx1;
    
    int sdel2;
    AudioBuffer<float> DDL2;
    int rwInx2;

    int sdel3;
    AudioBuffer<float> DDL3;
    int rwInx3;
    
    int sdel4;
    AudioBuffer<float> DDL4;
    int rwInx4;
    
    int sdel5;
    AudioBuffer<float> DDL5;
    int wInx5;
    
    float mPhase1;
    float mFrequency1;
    
    int sdel6;
    AudioBuffer<float> DDL6;
    int wInx6;
    
    float mPhase2;
    float mFrequency2;
    
    int sdel7;
    AudioBuffer<float> DDL7;
    int rwInx7;
    
    float mZ2;
    
    int sdel8;
    AudioBuffer<float> DDL8;
    int rwInx8;
    
    float mZ3;

    int sdel9;
    AudioBuffer<float> DDL9;
    int rwInx9;
    
    int sdel10;
    AudioBuffer<float> DDL10;
    int rwInx10;
    
    int sdel11;
    AudioBuffer<float> DDL11;
    int rwInx11;
    
    int sdel12;
    AudioBuffer<float> DDL12;
    int rwInx12;
    
    float mZ1;
    
    float mPreDelayTimeSmoothed;
    float mDryWetSmoothed;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DattorroReverbAudioProcessor)
};
