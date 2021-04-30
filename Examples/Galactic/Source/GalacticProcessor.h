/*
  ==============================================================================

    GalacticProcessor.h
    Created: 20 Apr 2021 6:03:59pm
    Author:  Syl

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <unordered_map>

class GalacticProcessor : public juce::AudioSource
{
public: 
    enum PARAMS
    {
        Replace, 
        Brightness,
        Detune, 
        Bigness,
        Dry_Wet
    };

    GalacticProcessor();
    ~GalacticProcessor() override;
	void setParam(PARAMS p, double v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
	std::unordered_map<PARAMS, float*> paramBindings;
	double sr;
	double iirAL;
	double iirBL;

	double aIL[6480];
	double aJL[3660];
	double aKL[1720];
	double aLL[680];

	double aAL[9700];
	double aBL[6000];
	double aCL[2320];
	double aDL[940];

	double aEL[15220];
	double aFL[8460];
	double aGL[4540];
	double aHL[3200];

	double aML[3111];
	double aMR[3111];
	double vibML, vibMR, depthM, oldfpd;

	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;

	double lastRefL[7];
	double thunderL;

	double iirAR;
	double iirBR;

	double aIR[6480];
	double aJR[3660];
	double aKR[1720];
	double aLR[680];

	double aAR[9700];
	double aBR[6000];
	double aCR[2320];
	double aDR[940];

	double aER[15220];
	double aFR[8460];
	double aGR[4540];
	double aHR[3200];

	double feedbackAR;
	double feedbackBR;
	double feedbackCR;
	double feedbackDR;

	double lastRefR[7];
	double thunderR;

	int countA, delayA;
	int countB, delayB;
	int countC, delayC;
	int countD, delayD;
	int countE, delayE;
	int countF, delayF;
	int countG, delayG;
	int countH, delayH;
	int countI, delayI;
	int countJ, delayJ;
	int countK, delayK;
	int countL, delayL;
	int countM, delayM;
	int cycle; //all these ints are shared across channels, not duplicated

	double vibM;

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E; //parameters. Always 0-1, and we scale/alter them elsewhere.
};