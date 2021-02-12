/* ========================================
 *  Swell - Swell.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class SwellProcessor : public juce::AudioSource
{
public:
    enum PARAMETER
    {
        THRESHOLD, 
        SWELL,
        DRY_WET
    };

    SwellProcessor();
    ~SwellProcessor() override;
    void setParam(PARAMETER p, float value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    std::map<PARAMETER, float*> paramBindings;
    double sr;
    long double fpNShapeL;
    long double fpNShapeR;

    long double swellL;
    long double swellR;
    bool louderL;
    bool louderR;

    float A;
    float B;
    float C;
};