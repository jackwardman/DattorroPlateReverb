/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DattorroReverbAudioProcessorEditor::DattorroReverbAudioProcessorEditor (DattorroReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 200);
    
    auto& params = processor.getParameters();
    
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
    
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue(); };
    mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); };
    mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); };
    
    AudioParameterFloat* preDelayParameter = (AudioParameterFloat*)params.getUnchecked(1);
    
    mPreDelaySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mPreDelaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mPreDelaySlider.setRange(preDelayParameter->range.start, preDelayParameter->range.end);
    mPreDelaySlider.setValue(*preDelayParameter);
    addAndMakeVisible(mPreDelaySlider);
    
    mPreDelaySlider.onValueChange = [this, preDelayParameter] { *preDelayParameter = mPreDelaySlider.getValue(); };
    mPreDelaySlider.onDragStart = [preDelayParameter] { preDelayParameter->beginChangeGesture(); };
    mPreDelaySlider.onDragEnd = [preDelayParameter] { preDelayParameter->endChangeGesture(); };
    
    AudioParameterFloat* bandwidthParameter = (AudioParameterFloat*)params.getUnchecked(2);
    mBandwidthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mBandwidthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mBandwidthSlider.setRange(bandwidthParameter->range.start, bandwidthParameter->range.end);
    mBandwidthSlider.setValue(*bandwidthParameter);
    addAndMakeVisible(mBandwidthSlider);
    
    mBandwidthSlider.onValueChange = [this, bandwidthParameter] { *bandwidthParameter = mBandwidthSlider.getValue(); };
    mBandwidthSlider.onDragStart = [bandwidthParameter] { bandwidthParameter->beginChangeGesture(); };
    mBandwidthSlider.onDragEnd = [bandwidthParameter] { bandwidthParameter->endChangeGesture(); };
    
    AudioParameterFloat* dampingParameter = (AudioParameterFloat*)params.getUnchecked(3);
    mDampingSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDampingSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDampingSlider.setRange(dampingParameter->range.start, dampingParameter->range.end);
    mDampingSlider.setValue(*dampingParameter);
    addAndMakeVisible(mDampingSlider);
    
    mDampingSlider.onValueChange = [this, dampingParameter] { *dampingParameter = mDampingSlider.getValue(); };
    mDampingSlider.onDragStart = [dampingParameter] { dampingParameter->beginChangeGesture(); };
    mDampingSlider.onDragEnd = [dampingParameter] { dampingParameter->endChangeGesture(); };
    
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(4);
    
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    mFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
    
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture(); };
    mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture(); };
    

}

DattorroReverbAudioProcessorEditor::~DattorroReverbAudioProcessorEditor()
{
}

//==============================================================================
void DattorroReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Dattorro's Plate Reverb", getLocalBounds(), juce::Justification::centredBottom, 1);
    
    g.drawText("Dry/Wet", ((getWidth() / 6 ) * 1) - (100 / 2), (getHeight() / 4), 100, 100, Justification::centredBottom);
    g.drawText("Pre Delay Time", ((getWidth() / 6 ) * 2) - (100 / 2), (getHeight() / 4), 100, 100, Justification::centredBottom);
    g.drawText("Colour 1", ((getWidth() / 6 ) * 3) - (100 / 2), (getHeight() / 4), 100, 100, Justification::centredBottom);
    g.drawText("Colour 2", ((getWidth() / 6 ) * 4) - (100 / 2), (getHeight() / 4), 100, 100, Justification::centredBottom);
    g.drawText("Feedback", ((getWidth() / 6 ) * 5) - (100 / 2), (getHeight() / 4), 100, 100, Justification::centredBottom);
}

void DattorroReverbAudioProcessorEditor::resized()
{
    mDryWetSlider.setBounds(((getWidth() / 6 ) * 1) - (100 / 2), (getHeight() / 4), 100, 100);
    mPreDelaySlider.setBounds(((getWidth() / 6 ) * 2) - (100 / 2), (getHeight() / 4), 100, 100);
    mBandwidthSlider.setBounds(((getWidth() / 6 ) * 3) - (100 / 2), (getHeight() / 4), 100, 100);
    mDampingSlider.setBounds(((getWidth() / 6 ) * 4) - (100 / 2), (getHeight() / 4), 100, 100);
    mFeedbackSlider.setBounds(((getWidth() / 6 ) * 5) - (100 / 2), (getHeight() / 4), 100, 100);
}
