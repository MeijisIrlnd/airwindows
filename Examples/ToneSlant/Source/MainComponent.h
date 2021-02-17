#pragma once

#include <JuceHeader.h>
#include "ToneSlantProcessor.h"
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
    void instantiateSlider(juce::Slider* s, juce::Label* l, ToneSlantProcessor::PARAMETER p, std::string labelText, double initialValue);
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
    ToneSlantProcessor toneSlant;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    juce::Label voicingLabel;
    juce::Slider voicingSlider;
    juce::Label highsLabel;
    juce::Slider highsSlider;
    juce::AudioDeviceSelectorComponent selector;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
