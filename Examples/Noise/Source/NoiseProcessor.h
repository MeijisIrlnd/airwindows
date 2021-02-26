/* ========================================
 *  Noise - Noise.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */


#pragma once
#include <JuceHeader.h>
#include <map>

class NoiseProcessor : public juce::AudioSource
{
public: 
	enum PARAMETER
	{
		HIGH_CUT,
		LOW_CUT,
		LSHAPE,
		DECAY,
		DISTNC,
		DRY_WET
	};

    NoiseProcessor();
    ~NoiseProcessor() override;
	void setParam(PARAMETER p, float value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
	std::map<PARAMETER, float*> paramBindings;
	double sr;
	double noiseAL;
	double noiseBL;
	double noiseCL;
	double rumbleAL;
	double rumbleBL;
	double surgeL;
	double noiseAR;
	double noiseBR;
	double noiseCR;
	double rumbleAR;
	double rumbleBR;
	double surgeR;

	int position;
	int quadratic;
	bool flipL;
	bool flipR;
	bool filterflip;

	double bL[11];
	double bR[11];

	double f[11];

	long double fpNShapeL;
	long double fpNShapeR;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
};