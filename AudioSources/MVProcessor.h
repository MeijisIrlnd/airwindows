/* ========================================
 *  MV - MV.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#pragma once
#include <JuceHeader.h>
#include <map>

class MVProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        DEPTH,
        BRIGHT,
        REGEN,
        OUTPUT,
        DRY_WET
    };

    MVProcessor();
    ~MVProcessor() override;
	void setParam(PARAMETER p, float v);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private:
    std::map<PARAMETER, float*> paramBindings;
	double sr;
	uint32_t fpd;
	//default stuff

	float A;
	float B;
	float C;
	float D;
	float E; //parameters. Always 0-1, and we scale/alter them elsewhere.

	double aAL[15150];
	double aBL[14618];
	double aCL[14358];
	double aDL[13818];
	double aEL[13562];
	double aFL[13046];
	double aGL[11966];
	double aHL[11130];
	double aIL[10598];
	double aJL[9810];
	double aKL[9522];
	double aLL[8982];
	double aML[8786];
	double aNL[8462];
	double aOL[8310];
	double aPL[7982];
	double aQL[7322];
	double aRL[6818];
	double aSL[6506];
	double aTL[6002];
	double aUL[5838];
	double aVL[5502];
	double aWL[5010];
	double aXL[4850];
	double aYL[4296];
	double aZL[4180];

	double avgAL;
	double avgBL;
	double avgCL;
	double avgDL;
	double avgEL;
	double avgFL;
	double avgGL;
	double avgHL;
	double avgIL;
	double avgJL;
	double avgKL;
	double avgLL;
	double avgML;
	double avgNL;
	double avgOL;
	double avgPL;
	double avgQL;
	double avgRL;
	double avgSL;
	double avgTL;
	double avgUL;
	double avgVL;
	double avgWL;
	double avgXL;
	double avgYL;
	double avgZL;

	double feedbackL;

	double aAR[15150];
	double aBR[14618];
	double aCR[14358];
	double aDR[13818];
	double aER[13562];
	double aFR[13046];
	double aGR[11966];
	double aHR[11130];
	double aIR[10598];
	double aJR[9810];
	double aKR[9522];
	double aLR[8982];
	double aMR[8786];
	double aNR[8462];
	double aOR[8310];
	double aPR[7982];
	double aQR[7322];
	double aRR[6818];
	double aSR[6506];
	double aTR[6002];
	double aUR[5838];
	double aVR[5502];
	double aWR[5010];
	double aXR[4850];
	double aYR[4296];
	double aZR[4180];

	double avgAR;
	double avgBR;
	double avgCR;
	double avgDR;
	double avgER;
	double avgFR;
	double avgGR;
	double avgHR;
	double avgIR;
	double avgJR;
	double avgKR;
	double avgLR;
	double avgMR;
	double avgNR;
	double avgOR;
	double avgPR;
	double avgQR;
	double avgRR;
	double avgSR;
	double avgTR;
	double avgUR;
	double avgVR;
	double avgWR;
	double avgXR;
	double avgYR;
	double avgZR;

	double feedbackR;

	//these are delay lengths and same for both sides
	int alpA, delayA;
	int alpB, delayB;
	int alpC, delayC;
	int alpD, delayD;
	int alpE, delayE;
	int alpF, delayF;
	int alpG, delayG;
	int alpH, delayH;
	int alpI, delayI;
	int alpJ, delayJ;
	int alpK, delayK;
	int alpL, delayL;
	int alpM, delayM;
	int alpN, delayN;
	int alpO, delayO;
	int alpP, delayP;
	int alpQ, delayQ;
	int alpR, delayR;
	int alpS, delayS;
	int alpT, delayT;
	int alpU, delayU;
	int alpV, delayV;
	int alpW, delayW;
	int alpX, delayX;
	int alpY, delayY;
	int alpZ, delayZ;
};