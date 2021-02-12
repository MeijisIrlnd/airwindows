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
    instantiateSlider(&intensitySlider, &intensityLabel, PopProcessor::INTENSITY, "Intensity", 0.3);
    instantiateSlider(&outputSlider, &outputLabel, PopProcessor::OUTPUT, "Output", 1.0);
    instantiateSlider(&dryWetSlider, &dryWetLabel, PopProcessor::DRY_WET, "Dry Wet", 1.0);
    addAndMakeVisible(selector);
    resized();
}

void MainComponent::instantiateSlider(juce::Slider* s, juce::Label* l, PopProcessor::PARAMETER p, std::string labelText, double initValue)
{
    l->setText(labelText, juce::dontSendNotification);
    addAndMakeVisible(l);
    s->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(initValue);
    addAndMakeVisible(s);
    s->onValueChange = [this, s, p] {pop.setParam(p, s->getValue()); };
    uiComponents.push_back(std::make_pair(l, s));
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    pop.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    pop.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    pop.releaseResources();
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
    if (uiComponents.size() != 0)
    {
        auto itemHeight = (getHeight() / 2) / uiComponents.size();
        for (auto i = 0; i < uiComponents.size(); i++)
        {
            uiComponents[i].first->setBounds(0, itemHeight * i, getWidth() / 4, itemHeight);
            uiComponents[i].second->setBounds(uiComponents[i].first->getX() + uiComponents[i].first->getWidth(), itemHeight * i, getWidth() - getWidth() / 4, itemHeight);
        }
        selector.setBounds(0, uiComponents[uiComponents.size() - 1].first->getY() + uiComponents[uiComponents.size() - 1].first->getHeight(), getWidth(), getHeight() / 2);
    }
}
