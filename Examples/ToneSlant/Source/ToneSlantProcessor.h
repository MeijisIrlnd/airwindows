/* ========================================
 *  ToneSlant - ToneSlant.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>

class ToneSlantProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        VOICING,
        HIGHS
    };
    ToneSlantProcessor();
    ~ToneSlantProcessor() override;
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    long double fpNShapeL;
    long double fpNShapeR;
    //default stuff

    float A;
    float B;
    double bL[102];
    double bR[102];
    double f[102];
};