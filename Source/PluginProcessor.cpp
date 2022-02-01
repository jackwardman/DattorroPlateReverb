/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DattorroReverbAudioProcessor::DattorroReverbAudioProcessor()
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
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet",
                                                            "Dry Wet",
                                                            0.0f,
                                                            1.0f,
                                                            0.5f));
    
    addParameter(mPreDelayParameter = new AudioParameterFloat("predelaytime",
                                                          "Pre Delay Time",
                                                          1.f,
                                                          50.f,
                                                          10.f));
    
    addParameter(mBandwidthParameter = new AudioParameterFloat("colour1",
                                                              "Colour 1",
                                                              0.0f,
                                                              1.0f,
                                                              0.5f));
    
    addParameter(mDampingParameter = new AudioParameterFloat("colour2",
                                                              "Colour 2",
                                                              0.0f,
                                                              1.0f,
                                                              0.5f));
    
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback",
                                                              "Feedback",
                                                              0.0f,
                                                              0.99f,
                                                              0.5f));
    
    
    mPreDelayWriteInx = 0;
    mZ1 = 0;
    mZ2 = 0;
    mZ3 = 0;
    mPhase1 = 0;
    mFrequency1 = 0.2;
    mPhase2 = 0;
    mFrequency1 = 0.6;
    
    rwInx1 = 0;
    rwInx2 = 0;
    rwInx3 = 0;
    rwInx4 = 0;
    wInx5 = 0;
    wInx6 = 0;
    rwInx7 = 0;
    rwInx8 = 0;
    rwInx9 = 0;
    rwInx10 = 0;
    rwInx11 = 0;
    rwInx12 = 0;
    
    mPreDelayTimeSmoothed = 0;
    mDryWetSmoothed = 0;
}

DattorroReverbAudioProcessor::~DattorroReverbAudioProcessor()
{
}

//==============================================================================
const juce::String DattorroReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DattorroReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DattorroReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DattorroReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DattorroReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DattorroReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DattorroReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DattorroReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DattorroReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void DattorroReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DattorroReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mPreDelayDDLLength = int(1 + (sampleRate * 50 * 0.001));
    
    mPreDelayDDL.setSize(1, mPreDelayDDLLength);
    mPreDelayDDL.clear();
    
    sdel1 = 210;
    sdel2 = 158;
    sdel3 = 561;
    sdel4 = 410;
    sdel5 = 1343+12;
    sdel6 = 995+12;
    sdel7 = 6241;
    sdel8 = 6590;
    sdel9 = 3931;
    sdel10 = 2664;
    sdel11 = 4641;
    sdel12 = 5505;
    
    DDL1.setSize(1, sdel1);
    DDL1.clear();
    DDL2.setSize(1, sdel2);
    DDL2.clear();
    DDL3.setSize(1, sdel3);
    DDL3.clear();
    DDL4.setSize(1, sdel4);
    DDL4.clear();
    DDL5.setSize(1, sdel5);
    DDL5.clear();
    DDL6.setSize(1, sdel6);
    DDL6.clear();
    DDL7.setSize(1, sdel7);
    DDL7.clear();
    DDL8.setSize(1, sdel8);
    DDL8.clear();
    DDL9.setSize(1, sdel9);
    DDL9.clear();
    DDL10.setSize(1, sdel10);
    DDL10.clear();
    DDL11.setSize(1, sdel11);
    DDL11.clear();
    DDL12.setSize(1, sdel12);
    DDL12.clear();
}

void DattorroReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DattorroReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void DattorroReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float* channelData;
    float* channelDataLeft;
    float* channelDataRight;
    if (totalNumInputChannels < 2) {
        channelData = buffer.getWritePointer(0);
    }
    else {
        channelDataLeft = buffer.getWritePointer(0);
        channelDataRight = buffer.getWritePointer(1);
    }
    float sampleRate = getSampleRate();
    float g1 = 0.75;
    float g2 = 0.625;
    float g3 = 0.5;
    float g4 = 0.7;
    
    // Main Time Loop
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        mPreDelayTimeSmoothed = mPreDelayTimeSmoothed - 0.0001 * (mPreDelayTimeSmoothed - *mPreDelayParameter);
        mDryWetSmoothed = mDryWetSmoothed - 0.0001 * (mDryWetSmoothed - *mDryWetParameter);

       // Calculate read indexes for the modulated delay lines
        float rInx5 = wInx5 - ((sdel5-12) + 12*sin(mPhase1));
        mPhase1 += (2*M_PI * mFrequency1/sampleRate);
        if (mPhase1 > 2*M_PI)
            mPhase1 -= 2*M_PI;
        float rInx6 = wInx6 - ((sdel6-12) + 12*sin(mPhase2));
        mPhase2 += (2*M_PI * mFrequency2/sampleRate);
        if (mPhase2 > 2*M_PI)
            mPhase2 -= 2*M_PI;
        if (rInx5 < 0)
            rInx5 += sdel5;
        if (rInx5 >= sdel5)
            rInx5 -= sdel5;
        if (rInx6 < 0)
            rInx6 += sdel6;
        if (rInx6 >= sdel6)
            rInx6 -= sdel6;
        
        int rInx5_x = int(rInx5);
        int rInx5_y = rInx5_x + 1;
        if (rInx5_y >= sdel5)
            rInx5_y -= sdel5;
        float rInx5_float = rInx5 - rInx5_x;
        
        float interp5 = lin_interp(DDL5.getSample(0, rInx5_x), DDL5.getSample(0, rInx5_y), rInx5_float);
        
        int rInx6_x = int(rInx6);
        int rInx6_y = rInx6_x + 1;
        if (rInx6_y >= sdel6)
            rInx6_y -= sdel6;
        float rInx6_float = rInx6 - rInx6_x;
        
        float interp6 = lin_interp(DDL6.getSample(0, rInx6_x), DDL6.getSample(0, rInx6_y), rInx6_float);
        
        
        // Initialise parameters and calculate the output of the pre-delay
        float g5 = *mFeedbackParameter;
        float temp1;
        if (totalNumInputChannels < 2) {
            temp1 = channelData[i];
        } else {
            temp1 = 0.5 * (channelDataLeft[i] + channelDataRight[i]);
        }
        
        float rInx_PreDelay = mPreDelayWriteInx - (mPreDelayTimeSmoothed * sampleRate * 0.001);
        
        if (rInx_PreDelay >= mPreDelayDDLLength)
            rInx_PreDelay -= mPreDelayDDLLength;
        if (rInx_PreDelay < 0)
            rInx_PreDelay += mPreDelayDDLLength;
        
        int rInx_PreDelay_x = int(rInx_PreDelay);
        int rInx_PreDelay_y = rInx_PreDelay_x + 1;
        if (rInx_PreDelay_y >= mPreDelayDDLLength)
            rInx_PreDelay_y -= mPreDelayDDLLength;
        float rInx_PreDelay_float = rInx_PreDelay - rInx_PreDelay_x;
        
        float preDelayInterp = lin_interp(mPreDelayDDL.getSample(0, rInx_PreDelay_x), mPreDelayDDL.getSample(0, rInx_PreDelay_y), rInx_PreDelay_float);
        
        
        // Model the circuit diagram
        float temp2 = mZ1 * (1 - *mBandwidthParameter) + preDelayInterp * *mBandwidthParameter;
        // APFs 1-4
        float temp3 = temp2 + -g1*DDL1.getSample(0, rwInx1);
        float temp4 = temp3 * g1 + DDL1.getSample(0, rwInx1);
        float temp5 = temp4 + -g1*DDL2.getSample(0, rwInx2);
        float temp6 = temp5 * g1 + DDL2.getSample(0, rwInx2);
        float temp7 = temp6 + -g2 * DDL3.getSample(0, rwInx3);
        float temp8 = temp7 * g2 + DDL3.getSample(0, rwInx3);
        float temp9 = temp8 + -g2 * DDL4.getSample(0, rwInx4);
        float temp10 = temp9 * g2 + DDL4.getSample(0, rwInx4);
        // Tank 1
        float temp11 = temp10 + g5 * DDL12.getSample(0, rwInx12) + g3 * interp5;
        float temp12 = -g3 * temp11 + interp5;
        float temp13 = (1 - *mDampingParameter) * DDL7.getSample(0, rwInx7) + mZ2 * *mDampingParameter;
        float temp14 = g5 * temp13 + -g4 * DDL9.getSample(0, rwInx9);
        float temp15 = g4 * temp14 + DDL9.getSample(0, rwInx9);
        // Tank 2
        float temp16 = temp10 + g5 * DDL11.getSample(0, rwInx11) + g3 * interp6;
        float temp17 = -g3 * temp16 + interp6;
        float temp18 = (1 - *mDampingParameter) * DDL8.getSample(0, rwInx8) + mZ3 * *mDampingParameter;
        float temp19 = g5 * temp18 + -g4 * DDL10.getSample(0, rwInx10);
        float temp20 = g4 * temp19 + DDL10.getSample(0, rwInx10);
        
        
        // Write step to delay lines / keep track of previous sample values
        mPreDelayDDL.setSample(0, mPreDelayWriteInx, temp1);
        mZ1 = temp2;
        DDL1.setSample(0, rwInx1, temp3);
        DDL2.setSample(0, rwInx2, temp5);
        DDL3.setSample(0, rwInx3, temp7);
        DDL4.setSample(0, rwInx4, temp9);
        DDL5.setSample(0, wInx5, temp11);
        DDL6.setSample(0, wInx6, temp16);
        DDL7.setSample(0, rwInx7, temp12);
        mZ2 = temp13;
        DDL8.setSample(0, rwInx8, temp17);
        mZ3 = temp18;
        DDL9.setSample(0, rwInx9, temp14);
        DDL10.setSample(0, rwInx10, temp19);
        DDL11.setSample(0, rwInx11, temp15);
        DDL12.setSample(0, rwInx12, temp20);
        
        
        // Calculate the output indexes
        float tapOutLeft1 = rwInx7 + 394;
        if (tapOutLeft1 >= sdel7)
            tapOutLeft1 -= sdel7;
        float tapOutLeft2 = rwInx7 + 4401;
        if (tapOutLeft2 >= sdel7)
            tapOutLeft2 -= sdel7;
        float tapOutLeft3 = rwInx9 + 2831;
        if (tapOutLeft3 >= sdel9)
            tapOutLeft3 -= sdel9;
        float tapOutLeft4 = rwInx11 + 2954;
        if (tapOutLeft4 >= sdel11)
            tapOutLeft4 -= sdel11;
        float tapOutLeft5 = rwInx8 + 2945;
        if (tapOutLeft5 >= sdel8)
            tapOutLeft5 -= sdel8;
        float tapOutLeft6 = rwInx10 + 277;
        if (tapOutLeft6 >= sdel10)
            tapOutLeft6 -= sdel10;
        float tapOutLeft7 = rwInx12 + 1578;
        if (tapOutLeft7 >= sdel12)
            tapOutLeft7 -= sdel12;
        
        float tapOutRight1 = rwInx8 + 522;
        if (tapOutRight1 >= sdel8)
            tapOutRight1 -= sdel8;
        float tapOutRight2 = rwInx8 + 5368;
        if (tapOutRight2 >= sdel8)
            tapOutRight2 -= sdel8;
        float tapOutRight3 = rwInx10 + 1817;
        if (tapOutRight3 >= sdel10)
            tapOutRight3 -= sdel10;
        float tapOutRight4 = rwInx12 + 3956;
        if (tapOutRight4 >= sdel12)
            tapOutRight4 -= sdel12;
        float tapOutRight5 = rwInx7 + 3124;
        if (tapOutRight5 >= sdel7)
            tapOutRight5 -= sdel7;
        float tapOutRight6 = rwInx9 + 496;
        if (tapOutRight6 >= sdel9)
            tapOutRight6 -= sdel9;
        float tapOutRight7 = rwInx11 + 179;
        if (tapOutRight7 >= sdel11)
            tapOutRight7 -= sdel11;

        
        mPreDelayWriteInx++;
        rwInx1++;
        rwInx2++;
        rwInx3++;
        rwInx4++;
        wInx5++;
        wInx6++;
        rwInx7++;
        rwInx8++;
        rwInx9++;
        rwInx10++;
        rwInx11++;
        rwInx12++;
        if (mPreDelayWriteInx >= mPreDelayDDLLength)
            mPreDelayWriteInx = 0;
        if (rwInx1 >= sdel1)
            rwInx1 = 0;
        if (rwInx2 >= sdel2)
            rwInx2 = 0;
        if (rwInx3 >= sdel3)
            rwInx3 = 0;
        if (rwInx4 >= sdel4)
            rwInx4 = 0;
        if (wInx5 >= sdel5)
            wInx5 = 0;
        if (wInx6 >= sdel6)
            wInx6 = 0;
        if (rwInx7 >= sdel7)
            rwInx7 = 0;
        if (rwInx8 >= sdel8)
            rwInx8 = 0;
        if (rwInx9 >= sdel9)
            rwInx9 = 0;
        if (rwInx10 >= sdel10)
            rwInx10 = 0;
        if (rwInx11 >= sdel11)
            rwInx11 = 0;
        if (rwInx12 >= sdel12)
            rwInx12 = 0;
        

        buffer.setSample(0, i, ((1 - mDryWetSmoothed) * temp1) + (mDryWetSmoothed * (DDL7.getSample(0, tapOutLeft1) + DDL7.getSample(0, tapOutLeft2) - DDL9.getSample(0, tapOutLeft3) - DDL11.getSample(0, tapOutLeft4) - DDL8.getSample(0, tapOutLeft5) - DDL10.getSample(0, tapOutLeft6) - DDL12.getSample(0, tapOutLeft7))));
            
        buffer.setSample(1, i, ((1 - mDryWetSmoothed) * temp1) + (mDryWetSmoothed * (DDL8.getSample(0, tapOutRight1) + DDL8.getSample(0, tapOutRight2) - DDL10.getSample(0, tapOutRight3) - DDL12.getSample(0, tapOutRight4) - DDL7.getSample(0, tapOutRight5) - DDL9.getSample(0, tapOutRight6) - DDL11.getSample(0, tapOutRight7))));

    }

}

//==============================================================================
bool DattorroReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DattorroReverbAudioProcessor::createEditor()
{
    return new DattorroReverbAudioProcessorEditor (*this);
}

//==============================================================================
void DattorroReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(new XmlElement("MultiTapDelay"));
    xml-> setAttribute("drywet", *mDryWetParameter);
    xml-> setAttribute("predelaytime", *mPreDelayParameter);
    xml->setAttribute("colour1", *mBandwidthParameter);
    xml->setAttribute("colour2", *mDampingParameter);
    xml->setAttribute("feedback", *mFeedbackParameter);
    
    copyXmlToBinary(*xml, destData);
}

void DattorroReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName("MultiTapDelay")) {
        *mDryWetParameter = xml->getDoubleAttribute("drywet");
        *mPreDelayParameter = xml->getDoubleAttribute("predelaytime");
        *mBandwidthParameter = xml->getDoubleAttribute("colour1");
        *mDampingParameter = xml->getDoubleAttribute("colour2");
        *mFeedbackParameter = xml->getDoubleAttribute("feedback");
    }}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DattorroReverbAudioProcessor();
}

float DattorroReverbAudioProcessor::lin_interp(float inSampleX, float inSampleY, float inFloatPhase)
{
    return (1 - inFloatPhase) * inSampleX + inFloatPhase * inSampleY;
}
