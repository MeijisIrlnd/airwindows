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
    instantiateSlider(&filterSlider, MatrixVerbProcessor::FILTER, "Filter", &filterLabel);
    instantiateSlider(&dampingSlider, MatrixVerbProcessor::DAMPING, "Damping", &dampingLabel);
    instantiateSlider(&speedSlider, MatrixVerbProcessor::SPEED, "Speed", &speedLabel);
    instantiateSlider(&vibratoSlider, MatrixVerbProcessor::VIBRATO, "Vibrato", &vibratoLabel);
    instantiateSlider(&roomsizeSlider, MatrixVerbProcessor::ROOMSIZE, "Room size", &roomsizeLabel);
    instantiateSlider(&flavourSlider, MatrixVerbProcessor::FLAVOUR, "Flavour", &flavourLabel);
    instantiateSlider(&dryWetSlider, MatrixVerbProcessor::DRYWET, "Dry Wet", &dryWetLabel);
    resized();
    addAndMakeVisible(&selector);
}

void MainComponent::instantiateSlider(juce::Slider* s, MatrixVerbProcessor::PARAMETER p, std::string labelText, juce::Label* l)
{
    l->setText(labelText, juce::dontSendNotification);
    s->setSliderStyle(juce::Slider::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(0.5);
    addAndMakeVisible(l);
    addAndMakeVisible(s);
    s->onValueChange = [this, p, s] {matrixVerb.setParam(p, s->getValue()); };
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
    matrixVerb.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    matrixVerb.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    matrixVerb.releaseResources();
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
    if (uiComponents.size() != 0) {
        auto itemHeight = (getHeight() / 2) / uiComponents.size();
        for (auto i = 0; i < uiComponents.size(); i++)
        {
            uiComponents[i].first->setBounds(0, itemHeight * i, getWidth() / 4, itemHeight);
            uiComponents[i].second->setBounds(uiComponents[i].first->getX() + uiComponents[i].first->getWidth(), itemHeight * i, getWidth() - (getWidth() / 4), itemHeight);
        }
        selector.setBounds(0, uiComponents[uiComponents.size() - 1].first->getY() + uiComponents[uiComponents.size() - 1].first->getHeight(), getWidth(), getHeight() / 2);
    }


}
