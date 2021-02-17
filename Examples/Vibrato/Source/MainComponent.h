#pragma once

#include <JuceHeader.h>
#include "VibratoProcessor.h"
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, VibratoProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    VibratoProcessor vibrato;
    juce::Label speedLabel;
    juce::Slider speedSlider;
    juce::Label depthLabel;
    juce::Slider depthSlider;
    juce::Label fmSpeedLabel;
    juce::Slider fmSpeedSlider;
    juce::Label fmDepthLabel;
    juce::Slider fmDepthSlider;
    juce::Label invWetLabel;
    juce::Slider invWetSlider;

    juce::AudioDeviceSelectorComponent selector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
