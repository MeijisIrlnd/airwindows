/* ========================================
 *  Sidepass - Sidepass.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class SidepassProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        CUTOFF
    };

    SidepassProcessor();
    ~SidepassProcessor() override;
    void setParam(PARAMETER p, double value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private: 
    std::map<PARAMETER, float*> paramBindings;
    double sr;
    double iirSampleA;
    double iirSampleB;
    bool flip;
    long double fpNShapeL;
    long double fpNShapeR;
    float A;
};
