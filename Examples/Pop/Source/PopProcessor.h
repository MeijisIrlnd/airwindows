/* ========================================
 *  Pop - Pop.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class PopProcessor : public juce::AudioSource
{
public:
	enum PARAMETER
	{
		INTENSITY,
		OUTPUT,
		DRY_WET
	};

    PopProcessor();
    ~PopProcessor() override;
	void setParam(PARAMETER p, float value);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
	std::map<PARAMETER, float*> paramBindings;
	double sr;
	uint32_t fpd;
	//default stuff

	double muVaryL;
	double muAttackL;
	double muNewSpeedL;
	double muSpeedAL;
	double muSpeedBL;
	double muCoefficientAL;
	double muCoefficientBL;
	long double thickenL;
	long double previousL;
	long double previous2L;
	long double previous3L;
	long double previous4L;
	long double previous5L;
	double dL[10001];

	double muVaryR;
	double muAttackR;
	double muNewSpeedR;
	double muSpeedAR;
	double muSpeedBR;
	double muCoefficientAR;
	double muCoefficientBR;
	long double thickenR;
	long double previousR;
	long double previous2R;
	long double previous3R;
	long double previous4R;
	long double previous5R;
	double dR[10001];

	int delay;
	bool flip;

	float A;
	float B;
	float C;
};