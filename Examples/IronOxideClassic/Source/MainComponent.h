#pragma once

#include <JuceHeader.h>
#include "IronOxideProcessor.h"
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

    void instantiateSlider(juce::Slider* s, juce::Label* l, IronOxideProcessor::PARAMETER p, std::string labelText, double initialValue);

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
    std::vector<std::pair<juce::Label*, juce::Slider*> > uiComponents;
    IronOxideProcessor ironOxide;
    juce::AudioDeviceSelectorComponent selector;
    juce::Label inputTrimLabel;
    juce::Slider inputTrimSlider;
    juce::Label tapeHighLabel;
    juce::Slider tapeHighSlider;
    juce::Label tapeLowLabel;
    juce::Slider tapeLowSlider;
    juce::Label flutterLabel;
    juce::Slider flutterSlider;
    juce::Label noiseLabel;
    juce::Slider noiseSlider;
    juce::Label outputTrimLabel;
    juce::Slider outputTrimSlider;
    juce::Label invDryWetLabel;
    juce::Slider invDryWetSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
