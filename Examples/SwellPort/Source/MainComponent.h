#pragma once

#include <JuceHeader.h>
#include "SwellProcessor.h"
#include <string>
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

    void instantiateSlider(juce::Slider* s, juce::Label* l, SwellProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    SwellProcessor swell;
    juce::Label thresholdLabel;
    juce::Slider thresholdSlider;
    juce::Label swellLabel;
    juce::Slider swellSlider;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    juce::AudioDeviceSelectorComponent selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
