/* ========================================
 *  GlitchShifter - GlitchShifter.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>
class GlitchShifterProcessor : public juce::AudioSource
{
public:
    enum PARAMETER
    {
        NOTE,
        TRIM,
        TIGHTEN,
        FEEDBACK,
        DRY_WET
    };

    GlitchShifterProcessor();
    ~GlitchShifterProcessor() override;
	void setParam(PARAMETER p, float value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    double sr;
    std::map<PARAMETER, float*> paramBindings;
	int32_t pL[131076];
	int32_t offsetL[258];
	int32_t pastzeroL[258];
	int32_t previousL[258];
	int32_t thirdL[258];
	int32_t fourthL[258];
	int32_t tempL;
	int32_t lasttempL;
	int32_t thirdtempL;
	int32_t fourthtempL;
	int32_t sincezerocrossL;
	int crossesL;
	int realzeroesL;
	double positionL;
	bool splicingL;

	double airPrevL;
	double airEvenL;
	double airOddL;
	double airFactorL;

	int32_t pR[131076];
	int32_t offsetR[258];
	int32_t pastzeroR[258];
	int32_t previousR[258];
	int32_t thirdR[258];
	int32_t fourthR[258];
	int32_t tempR;
	int32_t lasttempR;
	int32_t thirdtempR;
	int32_t fourthtempR;
	int32_t sincezerocrossR;
	int crossesR;
	int realzeroesR;
	double positionR;
	bool splicingR;

	double airPrevR;
	double airEvenR;
	double airOddR;
	double airFactorR;

	int gcount;
	int32_t lastwidth;
	bool flip;
	uint32_t fpd;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E; //parameters. Always 0-1, and we scale/alter them elsewhere.
};