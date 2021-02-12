/* ========================================
 *  Channel9 - Channel9.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== 
 */

#pragma once
#include <JuceHeader.h>
#include <map>
class ChannelStripProcessor : public juce::AudioSource
{
public:
    enum MODE
    {
        NEVE, API, SSL, TEAC, MACKIE
    };
    enum PARAMETER
    {
        INPUT_GAIN,
        OUTPUT_GAIN
    };

    ChannelStripProcessor();
    ~ChannelStripProcessor() override;

    void switchMode(MODE m);
    void setParam(PARAMETER p, float value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    std::map<PARAMETER, float*> paramBindings;
    MODE currentMode;
    double sr;

    uint32_t fpdL;
    uint32_t fpdR;

    double iirSampleLA;
    double iirSampleRA;
    double iirSampleLB;
    double iirSampleRB;
    double lastSampleAL;
    double lastSampleBL;
    double lastSampleCL;
    double lastSampleAR;
    double lastSampleBR;
    double lastSampleCR;
    long double biquadA[15];
    long double biquadB[15];
    double iirAmount;
    double threshold;
    double cutoff;
    bool flip;

    float A;
    float B;
    float C;
};