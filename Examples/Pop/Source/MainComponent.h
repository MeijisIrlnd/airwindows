#pragma once

#include <JuceHeader.h>
#include "PopProcessor.h"
#include <vector>
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, PopProcessor::PARAMETER p, std::string labelText, double initValue);
    ~MainComponent() override;

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
    PopProcessor pop;
    juce::AudioDeviceSelectorComponent selector;
    juce::Label intensityLabel;
    juce::Slider intensitySlider;
    juce::Label outputLabel;
    juce::Slider outputSlider;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
