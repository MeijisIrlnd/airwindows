/* ========================================
 *  Pyewacket - Pyewacket.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class PyewacketProcessor : public juce::AudioSource
{
public:
    enum PARAMETER
    {
        INPUT_GAIN, 
        RELEASE,
        OUTPUT_GAIN
    };

    PyewacketProcessor();
    ~PyewacketProcessor() override;
    
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    std::map<PARAMETER, float*> paramBindings;
    double sr;
    long double fpNShapeL;
    long double fpNShapeR;

    double chase;
    double lastrectifierL;
    double lastrectifierR;

    float A;
    float B;
    float C;
};