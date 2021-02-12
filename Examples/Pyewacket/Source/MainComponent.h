#pragma once

#include <JuceHeader.h>
#include "PyewacketProcessor.h"
#include <string>
#include <vector>
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

    void instantiateSlider(juce::Slider* s, juce::Label* l, PyewacketProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    PyewacketProcessor pyewacket;
    juce::Label inputGainLabel;
    juce::Slider inputGainSlider;
    juce::Label releaseLabel;
    juce::Slider releaseSlider;
    juce::Label outputGainLabel;
    juce::Slider outputGainSlider;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
