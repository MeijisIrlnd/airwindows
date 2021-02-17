#pragma once

#include <JuceHeader.h>
#include "MVProcessor.h"
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, MVProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    std::vector<std::pair<juce::Label*, juce::Slider* > > uiComponents;
    MVProcessor mv;
    juce::Label depthLabel;
    juce::Slider depthSlider;
    juce::Label brightLabel;
    juce::Slider brightSlider;
    juce::Label regenLabel;
    juce::Slider regenSlider;
    juce::Label outputLabel;
    juce::Slider outputSlider;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    juce::AudioDeviceSelectorComponent selector;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
