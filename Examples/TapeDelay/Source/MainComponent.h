#pragma once

#include <JuceHeader.h>
#include "TapeDelayProcessor.h"
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, TapeDelayProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    TapeDelayProcessor tapeDelay;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    juce::Label delayLabel;
    juce::Slider delaySlider;
    juce::Label feedbackLabel;
    juce::Slider feedbackSlider;
    juce::Label leanFtLabel;
    juce::Slider leanFtSlider;
    juce::Label depthLabel;
    juce::Slider depthSlider;
    juce::AudioDeviceSelectorComponent selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
