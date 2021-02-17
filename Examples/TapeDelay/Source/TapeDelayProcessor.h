/* ========================================
 *  TapeDelay - TapeDelay.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */


#pragma once
#include <JuceHeader.h>
#include <map>
class TapeDelayProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        DRY_WET,
        DELAY,
        FEEDBACK,
        LEAN_FT,
        DEPTH
    };
    TapeDelayProcessor();
    ~TapeDelayProcessor() override;
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    int pL[258];
    int pR[258];
    double dL[44100];
    double dR[44100];
    int gcount;
    int delay;
    int maxdelay;
    int chase;

    long double fpNShapeL;
    long double fpNShapeR;
    //default stuff
    // A and B need to be combined...
    float A;
    float B;
    float C;
    float D;
    float E;
    float F;
};