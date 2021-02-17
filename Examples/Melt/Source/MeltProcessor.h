/*
  ==============================================================================

    MeltProcessor.h
    Created: 17 Feb 2021 5:34:00pm
    Author:  Syl

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <map>

class MeltProcessor : public juce::AudioSource
{
public:
    enum PARAMETER
    {
        DEPTH,
        RANGE,
        OUTPUT,
        DRY_WET
    };
    MeltProcessor();
    ~MeltProcessor() override;
    void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    double sr;
    std::map<PARAMETER, float*> paramBindings;
    float dL[32002];
    float dR[32002];
    float combineL;
    float combineR;
    float scalefactorL;
    float scalefactorR;

    int minTap[32];
    int maxTap[32];
    int position[32];
    int stepTap[32];
    int stepCount;
    int slowCount;
    int gcount;

    long double fpNShapeL;
    long double fpNShapeR;
    //default stuff

    float A;
    float B;
    float C;
    float D; //parameters. Always 0-1, and we scale/alter them elsewhere.
};