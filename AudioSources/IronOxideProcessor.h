/* ========================================
 *  IronOxideClassic - IronOxideClassic.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class IronOxideProcessor : public juce::AudioSource
{
public: 
	enum PARAMETER
	{
		INPUT_TRIM,
		TAPE_HIGH,
		TAPE_LOW,
		FLUTTER,
		NOISE,
		OUTPUT_TRIM,
		INV_DRY_WET
	};

    IronOxideProcessor();
    ~IronOxideProcessor() override;
	void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
	double sr;
	std::map<PARAMETER, float*> paramBindings;
	double iirSamplehAL;
	double iirSamplehBL;
	double iirSampleAL;
	double iirSampleBL;
	double dL[264];
	double fastIIRAL;
	double fastIIRBL;
	double slowIIRAL;
	double slowIIRBL;
	double fastIIHAL;
	double fastIIHBL;
	double slowIIHAL;
	double slowIIHBL;
	double prevInputSampleL;

	double iirSamplehAR;
	double iirSamplehBR;
	double iirSampleAR;
	double iirSampleBR;
	double dR[264];
	double fastIIRAR;
	double fastIIRBR;
	double slowIIRAR;
	double slowIIRBR;
	double fastIIHAR;
	double fastIIHBR;
	double slowIIHAR;
	double slowIIHBR;
	double prevInputSampleR;

	int gcount;
	bool flip;

	double flL[100];
	double flR[100];

	int fstoredcount;
	double rateof;
	double sweep;
	double nextmax;


	long double fpNShapeL;
	long double fpNShapeR;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	float G;
};