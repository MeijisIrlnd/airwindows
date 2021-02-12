#pragma once

#include <JuceHeader.h>
#include "GlitchShifterProcessor.h"
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
    ~MainComponent() override;

    void instantiateSlider(juce::Slider* s, juce::Label* l, GlitchShifterProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    GlitchShifterProcessor glitchShifter;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    juce::Label noteLabel;
    juce::Slider noteSlider;
    juce::Label trimLabel;
    juce::Slider trimSlider;
    juce::Label tightenLabel;
    juce::Slider tightenSlider;
    juce::Label feedbackLabel;
    juce::Slider feedbackSlider;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    juce::AudioDeviceSelectorComponent selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
