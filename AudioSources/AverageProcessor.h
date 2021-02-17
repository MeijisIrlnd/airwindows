/*
  ==============================================================================
/* ========================================
 *  Average - Average.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>

class AverageProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        AVERAGE,
        DRY_WET
    };
    AverageProcessor();
    ~AverageProcessor() override;
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private: 
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    double bL[11];
    double f[11];
    double bR[11];

    long double fpNShapeL;
    long double fpNShapeR;
    //default stuff

    float A;
    float B;
};
