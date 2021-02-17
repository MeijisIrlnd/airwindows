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

    instantiateSlider(&inputTrimSlider, &inputTrimLabel, IronOxideProcessor::INPUT_TRIM, "Input Trim", 0.5);
    instantiateSlider(&tapeHighSlider, &tapeHighLabel, IronOxideProcessor::TAPE_HIGH, "Tape High", 0.562341325190349);
    instantiateSlider(&tapeLowSlider, &tapeLowLabel, IronOxideProcessor::TAPE_LOW, "Tape Low", 0.562341325190349);
    instantiateSlider(&flutterSlider, &flutterLabel, IronOxideProcessor::FLUTTER, "Flutter", 0.5);
    instantiateSlider(&noiseSlider, &noiseLabel, IronOxideProcessor::NOISE, "Noise", 0.5);
    instantiateSlider(&outputTrimSlider, &outputTrimLabel, IronOxideProcessor::OUTPUT_TRIM, "Output Trim", 0.5);
    instantiateSlider(&invDryWetSlider, &invDryWetLabel, IronOxideProcessor::INV_DRY_WET, "Inv/Dry/Wet", 1);
    addAndMakeVisible(&selector);
    resized();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::instantiateSlider(juce::Slider* s, juce::Label* l, IronOxideProcessor::PARAMETER p, std::string labelText, double initialValue)
{
    l->setText(labelText, juce::dontSendNotification);
    addAndMakeVisible(l);
    s->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s->setRange(0, 1, 0.01);
    s->setValue(initialValue);
    addAndMakeVisible(s);
    uiComponents.push_back(std::make_pair(l, s));
    s->onValueChange = [this, s, p] {ironOxide.setParam(p, s->getValue()); };
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    ironOxide.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    ironOxide.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    ironOxide.releaseResources();
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
            uiComponents[i].second->setBounds(uiComponents[i].first->getX() + uiComponents[i].first->getWidth(), itemHeight * i, getWidth() - (getWidth() / 4), itemHeight);
        }
        selector.setBounds(0, uiComponents[uiComponents.size() - 1].first->getY() + uiComponents[uiComponents.size() - 1].first->getHeight(), getWidth(), getHeight() / 2);
    }
}