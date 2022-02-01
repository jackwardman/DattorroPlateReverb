/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DattorroReverbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DattorroReverbAudioProcessorEditor (DattorroReverbAudioProcessor&);
    ~DattorroReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    Slider mDryWetSlider;
    Slider mBandwidthSlider;
    Slider mDampingSlider;
    Slider mFeedbackSlider;
    Slider mPreDelaySlider;
    
    DattorroReverbAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DattorroReverbAudioProcessorEditor)
};
