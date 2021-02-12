#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : selector(deviceManager, 2, 2, 2, 2, false, false, true, true)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    instantiateSlider(&thresholdSlider, &thresholdLabel, SwellProcessor::THRESHOLD, "Threshold", 0.9);
    instantiateSlider(&swellSlider, &swellLabel, SwellProcessor::SWELL, "Swell", 0.5);
    instantiateSlider(&dryWetSlider, &dryWetLabel, SwellProcessor::DRY_WET, "Dry/Wet", 1.0);
    addAndMakeVisible(&selector);

}

void MainComponent::instantiateSlider(juce::Slider* s, juce::Label* l, SwellProcessor::PARAMETER p, std::string labelText, double initialValue)
{
    l->setText(labelText, juce::dontSendNotification);
    addAndMakeVisible(l);
    s->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(initialValue);
    s->onValueChange = [this, s, p] {swell.setParam(p, s->getValue()); };
    addAndMakeVisible(s);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    swell.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    swell.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    swell.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto itemHeight = ((getHeight() / 2) / 3);
    thresholdLabel.setBounds(0, 0, getWidth() / 4, itemHeight);
    thresholdSlider.setBounds(thresholdLabel.getX() + thresholdLabel.getWidth(), 0, getWidth() - (getWidth() / 4), itemHeight);
    swellLabel.setBounds(0, itemHeight, getWidth() / 4, itemHeight);
    swellSlider.setBounds(swellLabel.getWidth() + swellLabel.getX(), itemHeight, getWidth() - (getWidth() / 4), itemHeight);
    dryWetLabel.setBounds(0, itemHeight * 2, getWidth() / 4, itemHeight);
    dryWetSlider.setBounds(dryWetLabel.getX() + dryWetLabel.getWidth(), itemHeight * 2, getWidth() - (getWidth() / 4), itemHeight);
    selector.setBounds(0, dryWetSlider.getY() + dryWetSlider.getHeight(), getWidth(), getHeight() / 2);
}
