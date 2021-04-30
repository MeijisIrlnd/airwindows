#pragma once

#include <JuceHeader.h>
#include "Biquad2Processor.h"
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, Biquad2Processor::PARAMETER p, std::string labelText, double initialValue);
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
    Biquad2Processor biquad;
    juce::Label typeLabel;
    juce::Slider typeSlider;
    juce::Label freqLabel;
    juce::Slider freqSlider;
    juce::Label qLabel;
    juce::Slider qSlider;
    juce::Label outputLabel;
    juce::Slider outputSlider;
    juce::Label invWetLabel;
    juce::Slider invWetSlider;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    juce::AudioDeviceSelectorComponent selector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
