#pragma once

#include <JuceHeader.h>
#include "GalacticProcessor.h"
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
    void instantiateSlider(juce::Label* l, juce::Slider* s, std::string labelText, double initialValue, GalacticProcessor::PARAMS p);
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
    GalacticProcessor galactic;
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    juce::Label replaceLabel, brightnessLabel, detuneLabel, bignessLabel, dryWetLabel;
    juce::Slider replaceSlider, brightnessSlider, detuneSlider, bignessSlider, dryWetSlider;
    juce::AudioDeviceSelectorComponent selector;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
