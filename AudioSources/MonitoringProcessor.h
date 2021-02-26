/* ========================================
 *  Monitoring - Monitoring.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */
 
#pragma once
#include <JuceHeader.h>
#include <map>

class MonitoringProcessor : public juce::AudioSource
{
public: 
    enum PARAMETER
    {
        MONITOR
    };

    MonitoringProcessor();
    ~MonitoringProcessor() override;
    void setParam(PARAMETER p, double value);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
private: 
    std::map<PARAMETER, float*> paramBindings;
	double sr;
	long double bynL[13], bynR[13];
	long double noiseShapingL, noiseShapingR;
	//NJAD
	double aL[1503], bL[1503], cL[1503], dL[1503];
	double aR[1503], bR[1503], cR[1503], dR[1503];
	int ax, bx, cx, dx;
	//PeaksOnly
	double lastSampleL, lastSampleR;
	//SlewOnly
	double iirSampleAL, iirSampleBL, iirSampleCL, iirSampleDL, iirSampleEL, iirSampleFL, iirSampleGL;
	double iirSampleHL, iirSampleIL, iirSampleJL, iirSampleKL, iirSampleLL, iirSampleML, iirSampleNL, iirSampleOL, iirSamplePL;
	double iirSampleQL, iirSampleRL, iirSampleSL;
	double iirSampleTL, iirSampleUL, iirSampleVL;
	double iirSampleWL, iirSampleXL, iirSampleYL, iirSampleZL;

	double iirSampleAR, iirSampleBR, iirSampleCR, iirSampleDR, iirSampleER, iirSampleFR, iirSampleGR;
	double iirSampleHR, iirSampleIR, iirSampleJR, iirSampleKR, iirSampleLR, iirSampleMR, iirSampleNR, iirSampleOR, iirSamplePR;
	double iirSampleQR, iirSampleRR, iirSampleSR;
	double iirSampleTR, iirSampleUR, iirSampleVR;
	double iirSampleWR, iirSampleXR, iirSampleYR, iirSampleZR; // o/`
	//SubsOnly
	long double biquadL[11];
	long double biquadR[11];
	//Bandpasses

	uint32_t fpd;
	//default stuff

	float A;
};