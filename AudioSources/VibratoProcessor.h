/* ========================================
 *  Vibrato - Vibrato.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class VibratoProcessor : public juce::AudioSource
{
public:
	enum PARAMETER
	{
		SPEED,
		DEPTH,
		FMSPEED,
		FMDEPTH,
		INV_WET
	};

    VibratoProcessor();
    ~VibratoProcessor() override;
	void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
	double sr;
	std::map<PARAMETER, float*> paramBindings;
	double pL[16386]; //this is processed, not raw incoming samples
	double pR[16386]; //this is processed, not raw incoming samples
	double sweep;
	double sweepB;
	int gcount;

	double airPrevL;
	double airEvenL;
	double airOddL;
	double airFactorL;
	double airPrevR;
	double airEvenR;
	double airOddR;
	double airFactorR;

	bool flip;
	uint32_t fpd;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E; //parameters. Always 0-1, and we scale/alter them elsewhere.
};