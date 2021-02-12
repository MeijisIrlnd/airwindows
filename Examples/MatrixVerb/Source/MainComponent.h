#pragma once

#include <JuceHeader.h>
#include "MatrixVerbProcessor.h"
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

    void instantiateSlider(juce::Slider* s, MatrixVerbProcessor::PARAMETER p, std::string labelText, juce::Label* l);
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
    MatrixVerbProcessor matrixVerb;
    juce::AudioDeviceSelectorComponent selector;
    juce::Label filterLabel;
    juce::Slider filterSlider;
    juce::Label dampingLabel;
    juce::Slider dampingSlider;
    juce::Label speedLabel;
    juce::Slider speedSlider;
    juce::Label vibratoLabel;
    juce::Slider vibratoSlider;
    juce::Label roomsizeLabel;
    juce::Slider roomsizeSlider;
    juce::Label flavourLabel;
    juce::Slider flavourSlider;
    juce::Label dryWetLabel;
    juce::Slider dryWetSlider;
    std::vector <std::pair<juce::Label*, juce::Slider*> > uiComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
