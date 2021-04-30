#pragma once

#include <JuceHeader.h>
#include "VerbicityProcessor.h"
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
    void instantiateSlider(juce::Label* l, juce::Slider* s, std::string labelText, double initialValue, VerbicityProcessor::PARAMS p);
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
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    VerbicityProcessor verbicity;
    juce::Label bignessLabel, loudnessLabel, darknessLabel, wetnessLabel;
    juce::Slider bignessSlider, loudnessSlider, darknessSlider, wetnessSlider;
    juce::AudioDeviceSelectorComponent selector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
