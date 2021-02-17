/* ========================================
 *  TremoSquare - TremoSquare.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */


#pragma once
#include <JuceHeader.h>
#include <map>

class TremoSquareProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER 
    {
        FREQUENCY,
        DRY_WET
    };

    TremoSquareProcessor();
    ~TremoSquareProcessor() override;
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private: 
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    double osc;
    bool polarityL;
    bool muteL;
    bool polarityR;
    bool muteR;
    uint32_t fpd;
    //default stuff

    float A;
    float B;
};