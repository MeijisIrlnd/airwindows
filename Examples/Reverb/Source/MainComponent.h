#pragma once

#include <JuceHeader.h>
#include "ReverbProcessor.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    void instantiateSlider(juce::Slider* s, juce::Label* l, ReverbProcessor::PARAMETER p, std::string labelText, double initialValue);
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::AudioDeviceSelectorComponent selector;
    ReverbProcessor reverb;
    juce::Label bigLabel;
    juce::Slider bigSlider;
    juce::Label wetLabel;
    juce::Slider wetSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
