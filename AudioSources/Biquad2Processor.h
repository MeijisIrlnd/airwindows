/* ========================================
 *  Biquad2 - Biquad2.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class Biquad2Processor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        TYPE,
        FREQ,
        Q,
        OUTPUT,
        INV_WET
    };

    Biquad2Processor();
    ~Biquad2Processor() override;
    void setParam(PARAMETER p, double value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private: 
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    long double biquad[15]; //note that this stereo form doesn't require L and R forms!
//This is because so much of it is coefficients etc. that are the same on both channels.
//So the stored samples are in 7-8-9-10 and 11-12-13-14, and freq/res/coefficients serve both.

    double bL[11];
    double bR[11];
    double f[11];
    double frequencychase;
    double resonancechase;
    double outputchase;
    double wetchase;
    double frequencysetting;
    double resonancesetting;
    double outputsetting;
    double wetsetting;
    double chasespeed;

    uint32_t fpd;

    float A;
    float B;
    float C;
    float D;
    float E;
};