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
    instantiateSlider(&bigSlider, &bigLabel, ReverbProcessor::BIG, "Big", 0.5);
    instantiateSlider(&wetSlider, &wetLabel, ReverbProcessor::WET, "Wet", 0.5);
    addAndMakeVisible(&selector);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::instantiateSlider(juce::Slider* s, juce::Label* l, ReverbProcessor::PARAMETER p, std::string labelText, double initialValue)
{
    l->setText(labelText, juce::dontSendNotification);
    addAndMakeVisible(l);
    s->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(initialValue);
    addAndMakeVisible(s);
    s->onValueChange = [this, s, p] {reverb.setParam(p, s->getValue()); };
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    reverb.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverb.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    reverb.releaseResources();
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
    auto itemHeight = ((getHeight() / 2) / 2);
    bigLabel.setBounds(0, 0, getWidth() / 4, itemHeight);
    bigSlider.setBounds(bigLabel.getX() + bigLabel.getWidth(), 0, getWidth() - (getWidth() / 4), itemHeight);
    wetLabel.setBounds(0, bigLabel.getY() + bigLabel.getHeight(), getWidth() / 4, itemHeight);
    wetSlider.setBounds(wetLabel.getX() + wetLabel.getWidth(), wetLabel.getY(), getWidth() - (getWidth() / 4), itemHeight);
    selector.setBounds(0, wetSlider.getY() + wetSlider.getHeight(), getWidth(), getHeight() / 2);
}
