/*
  ==============================================================================
 *  Baxandall - Baxandall.h
 *  Copyright (c) 2016 airwindows, All rights reserved
  ==============================================================================
*/

#include "BaxandallProcessor.h"

BaxandallProcessor::BaxandallProcessor()
{
	paramBindings =
	{
		{BASS, &A},
		{TREBLE, &B},
		{OUTPUT, &C}
	};

	A = 0.5;
	B = 0.5;
	C = 0.5;
	for (int x = 0; x < 9; x++) {
		trebleAL[x] = 0.0;
		trebleBL[x] = 0.0;
		bassAL[x] = 0.0;
		bassBL[x] = 0.0;
		trebleAR[x] = 0.0;
		trebleBR[x] = 0.0;
		bassAR[x] = 0.0;
		bassBR[x] = 0.0;
	}
	flip = false;
	fpd = 17;
}

BaxandallProcessor::~BaxandallProcessor()
{

}

void BaxandallProcessor::setParam(PARAMETER p, float value)
{
	if (paramBindings.find(p) != paramBindings.end())
	{
		*paramBindings[p] = value;
	}
}


void BaxandallProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	sr = sampleRate;
}

void BaxandallProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto* rpL = bufferToFill.buffer->getReadPointer(0);
	auto* rpR = bufferToFill.buffer->getReadPointer(1);
	auto* wpL = bufferToFill.buffer->getWritePointer(0);
	auto* wpR = bufferToFill.buffer->getWritePointer(1);

	double trebleGain = pow(10.0, ((A * 30.0) - 15.0) / 20.0);
	double trebleFreq = (4410.0 * trebleGain) / sr;
	if (trebleFreq > 0.45) trebleFreq = 0.45;
	trebleAL[0] = trebleBL[0] = trebleAR[0] = trebleBR[0] = trebleFreq;
	double bassGain = pow(10.0, ((B * 30.0) - 15.0) / 20.0);
	double bassFreq = pow(10.0, -((B * 30.0) - 15.0) / 20.0);
	bassFreq = (8820.0 * bassFreq) / sr;
	if (bassFreq > 0.45) bassFreq = 0.45;
	bassAL[0] = bassBL[0] = bassAR[0] = bassBR[0] = bassFreq;
	trebleAL[1] = trebleBL[1] = trebleAR[1] = trebleBR[1] = 0.4;
	bassAL[1] = bassBL[1] = bassAR[1] = bassBR[1] = 0.2;
	double output = pow(10.0, ((C * 30.0) - 15.0) / 20.0);

	double K = std::tan(juce::MathConstants<double>::pi * trebleAL[0]);
	double norm = 1.0 / (1.0 + K / trebleAL[1] + K * K);
	trebleBL[2] = trebleAL[2] = trebleBR[2] = trebleAR[2] = K * K * norm;
	trebleBL[3] = trebleAL[3] = trebleBR[3] = trebleAR[3] = 2.0 * trebleAL[2];
	trebleBL[4] = trebleAL[4] = trebleBR[4] = trebleAR[4] = trebleAL[2];
	trebleBL[5] = trebleAL[5] = trebleBR[5] = trebleAR[5] = 2.0 * (K * K - 1.0) * norm;
	trebleBL[6] = trebleAL[6] = trebleBR[6] = trebleAR[6] = (1.0 - K / trebleAL[1] + K * K) * norm;

	K = std::tan(juce::MathConstants<double>::pi * bassAL[0]);
	norm = 1.0 / (1.0 + K / bassAL[1] + K * K);
	bassBL[2] = bassAL[2] = bassBR[2] = bassAR[2] = K * K * norm;
	bassBL[3] = bassAL[3] = bassBR[3] = bassAR[3] = 2.0 * bassAL[2];
	bassBL[4] = bassAL[4] = bassBR[4] = bassAR[4] = bassAL[2];
	bassBL[5] = bassAL[5] = bassBR[5] = bassAR[5] = 2.0 * (K * K - 1.0) * norm;
	bassBL[6] = bassAL[6] = bassBR[6] = bassAR[6] = (1.0 - K / bassAL[1] + K * K) * norm;

	for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		long double inputSampleL = *rpL;
		long double inputSampleR = *rpR;
		if (std::fabs(inputSampleL) < 1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (std::fabs(inputSampleR) < 1.18e-37) inputSampleR = fpd * 1.18e-37;

		if (output != 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}

		inputSampleL = std::sin(inputSampleL);
		inputSampleR = std::sin(inputSampleR);

		long double trebleSampleL;
		long double bassSampleL;
		long double trebleSampleR;
		long double bassSampleR;

		if (flip)
		{
			trebleSampleL = (inputSampleL * trebleAL[2]) + trebleAL[7];
			trebleAL[7] = (inputSampleL * trebleAL[3]) - (trebleSampleL * trebleAL[5]) + trebleAL[8];
			trebleAL[8] = (inputSampleL * trebleAL[4]) - (trebleSampleL * trebleAL[6]);
			trebleSampleL = inputSampleL - trebleSampleL;

			bassSampleL = (inputSampleL * bassAL[2]) + bassAL[7];
			bassAL[7] = (inputSampleL * bassAL[3]) - (bassSampleL * bassAL[5]) + bassAL[8];
			bassAL[8] = (inputSampleL * bassAL[4]) - (bassSampleL * bassAL[6]);

			trebleSampleR = (inputSampleR * trebleAR[2]) + trebleAR[7];
			trebleAR[7] = (inputSampleR * trebleAR[3]) - (trebleSampleR * trebleAR[5]) + trebleAR[8];
			trebleAR[8] = (inputSampleR * trebleAR[4]) - (trebleSampleR * trebleAR[6]);
			trebleSampleR = inputSampleR - trebleSampleR;

			bassSampleR = (inputSampleR * bassAR[2]) + bassAR[7];
			bassAR[7] = (inputSampleR * bassAR[3]) - (bassSampleR * bassAR[5]) + bassAR[8];
			bassAR[8] = (inputSampleR * bassAR[4]) - (bassSampleR * bassAR[6]);
		}

		else
		{
			trebleSampleL = (inputSampleL * trebleBL[2]) + trebleBL[7];
			trebleBL[7] = (inputSampleL * trebleBL[3]) - (trebleSampleL * trebleBL[5]) + trebleBL[8];
			trebleBL[8] = (inputSampleL * trebleBL[4]) - (trebleSampleL * trebleBL[6]);
			trebleSampleL = inputSampleL - trebleSampleL;

			bassSampleL = (inputSampleL * bassBL[2]) + bassBL[7];
			bassBL[7] = (inputSampleL * bassBL[3]) - (bassSampleL * bassBL[5]) + bassBL[8];
			bassBL[8] = (inputSampleL * bassBL[4]) - (bassSampleL * bassBL[6]);

			trebleSampleR = (inputSampleR * trebleBR[2]) + trebleBR[7];
			trebleBR[7] = (inputSampleR * trebleBR[3]) - (trebleSampleR * trebleBR[5]) + trebleBR[8];
			trebleBR[8] = (inputSampleR * trebleBR[4]) - (trebleSampleR * trebleBR[6]);
			trebleSampleR = inputSampleR - trebleSampleR;

			bassSampleR = (inputSampleR * bassBR[2]) + bassBR[7];
			bassBR[7] = (inputSampleR * bassBR[3]) - (bassSampleR * bassBR[5]) + bassBR[8];
			bassBR[8] = (inputSampleR * bassBR[4]) - (bassSampleR * bassBR[6]);
		}
		flip = !flip;

		trebleSampleL *= trebleGain;
		bassSampleL *= bassGain;
		inputSampleL = bassSampleL + trebleSampleL; 
		trebleSampleR *= trebleGain;
		bassSampleR *= bassGain;
		inputSampleR = bassSampleR + trebleSampleR; 

		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;

		inputSampleL = std::asin(inputSampleL);

		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;

		inputSampleR = std::asin(inputSampleR);

		int expon; std::frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
		std::frexpf((float)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += ((double(fpd) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));

		*wpL = inputSampleL;
		*wpR = inputSampleR;
		++rpL;
		++rpR;
		++wpL;
		++wpR;
	}
}

void BaxandallProcessor::releaseResources()
{

}
