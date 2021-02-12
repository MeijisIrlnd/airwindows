/* ========================================
 *  MatrixVerb - MatrixVerb.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>

class MatrixVerbProcessor : public juce::AudioSource
{
public:
	enum PARAMETER
	{
		FILTER,
		DAMPING,
		SPEED,
		VIBRATO,
		ROOMSIZE,
		FLAVOUR,
		DRYWET
	};

    MatrixVerbProcessor();
    ~MatrixVerbProcessor() override;
	void setParam(PARAMETER p, float value);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
	std::map<PARAMETER, float*> paramBindings;
	double sr;
	long double biquadA[11];
	long double biquadB[11];
	long double biquadC[11];

	double aAL[8111];
	double aBL[7511];
	double aCL[7311];
	double aDL[6911];
	double aEL[6311];
	double aFL[6111];
	double aGL[5511];
	double aHL[4911];
	double aIL[4511];
	double aJL[4311];
	double aKL[3911];
	double aLL[3311];
	double aML[3111];

	double aAR[8111];
	double aBR[7511];
	double aCR[7311];
	double aDR[6911];
	double aER[6311];
	double aFR[6111];
	double aGR[5511];
	double aHR[4911];
	double aIR[4511];
	double aJR[4311];
	double aKR[3911];
	double aLR[3311];
	double aMR[3111];

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

	double feedbackAL, vibAL, depthA;
	double feedbackBL, vibBL, depthB;
	double feedbackCL, vibCL, depthC;
	double feedbackDL, vibDL, depthD;
	double feedbackEL, vibEL, depthE;
	double feedbackFL, vibFL, depthF;
	double feedbackGL, vibGL, depthG;
	double feedbackHL, vibHL, depthH;

	double feedbackAR, vibAR;
	double feedbackBR, vibBR;
	double feedbackCR, vibCR;
	double feedbackDR, vibDR;
	double feedbackER, vibER;
	double feedbackFR, vibFR;
	double feedbackGR, vibGR;
	double feedbackHR, vibHR;

	uint32_t fpdL;
	uint32_t fpdR;

	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	float G;
};