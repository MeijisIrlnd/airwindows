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
    instantiateSlider(&highCutSlider, &highCutLabel, NoiseProcessor::HIGH_CUT, "High Cut", 0.5);
    instantiateSlider(&lowCutSlider, &lowCutLabel, NoiseProcessor::LOW_CUT, "Low Cut", 0.5);
    instantiateSlider(&lShapeSlider, &lShapeLabel, NoiseProcessor::LSHAPE, "L Shape", 0.5);
    instantiateSlider(&decaySlider, &decayLabel, NoiseProcessor::DECAY, "Decay", 1);
    instantiateSlider(&distncSlider, &distncLabel, NoiseProcessor::DISTNC, "Distnc", 0);
    instantiateSlider(&dryWetSlider, &dryWetLabel, NoiseProcessor::DRY_WET, "Dry/Wet", 1);
    addAndMakeVisible(&selector);
    resized();
}

void MainComponent::instantiateSlider(juce::Slider* s, juce::Label* l, NoiseProcessor::PARAMETER p, std::string labelText, double initialValue)
{
    l->setText(labelText, juce::dontSendNotification);
    addAndMakeVisible(l);
    s->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(initialValue);
    addAndMakeVisible(s);
    s->onValueChange = [this, s, p] { noise.setParam(p, s->getValue()); };
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
    noise.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    noise.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    noise.releaseResources();
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
            uiComponents[i].second->setBounds(getWidth() / 4, itemHeight * i, getWidth() - (getWidth() / 4), itemHeight);
        }
        selector.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
    }
}
