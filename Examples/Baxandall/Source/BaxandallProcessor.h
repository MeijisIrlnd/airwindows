/*
  ==============================================================================
 *  Baxandall - Baxandall.h
 *  Copyright (c) 2016 airwindows, All rights reserved
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <map>
class BaxandallProcessor : public juce::AudioSource
{
public: 
	enum PARAMETER
	{
		BASS, 
		TREBLE,
		OUTPUT
	};

    BaxandallProcessor();
    ~BaxandallProcessor() override; 

	void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
	std::map<PARAMETER, float*> paramBindings;
	double sr;
	uint32_t fpd;

	long double trebleAL[9];
	long double trebleBL[9];
	long double bassAL[9];
	long double bassBL[9];

	long double trebleAR[9];
	long double trebleBR[9];
	long double bassAR[9];
	long double bassBR[9];
	bool flip;

	float A;
	float B;
	float C;
};