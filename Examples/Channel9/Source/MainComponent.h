#pragma once

#include <JuceHeader.h>
#include "ChannelStripProcessor.h"
#include <vector>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:

    //==============================================================================
    MainComponent();
    void instantiateSlider(juce::Slider* s, juce::Label* l, ChannelStripProcessor::PARAMETER p, std::string labelText, double initialValue);
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

    // UI:
    juce::ComboBox modeSelector;
    juce::Label driveLabel;
    juce::Slider driveSlider;
    juce::Label outputLabel;
    juce::Slider outputSlider;
    juce::AudioDeviceSelectorComponent deviceSelector;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    ChannelStripProcessor channelStrip;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
