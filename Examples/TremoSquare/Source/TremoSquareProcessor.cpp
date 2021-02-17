/* ========================================
 *  TremoSquare - TremoSquare.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */


#include "TremoSquareProcessor.h"

TremoSquareProcessor::TremoSquareProcessor()
{
    paramBindings = {
        {FREQUENCY, &A},
        {DRY_WET, &B}
    };
    A = 0.5;
    B = 1.0;
    osc = 0.0;
    polarityL = false;
    muteL = false;
    polarityR = false;
    muteR = false;
    fpd = 17;
}

TremoSquareProcessor::~TremoSquareProcessor()
{

}

void TremoSquareProcessor::setParam(PARAMETER p, float v)
{
	if (paramBindings.find(p) != paramBindings.end())
	{
		*paramBindings[p] = v;
	}
}

void TremoSquareProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void TremoSquareProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* rpL = bufferToFill.buffer->getReadPointer(0);
    auto* rpR = bufferToFill.buffer->getReadPointer(1);
    auto* wpL = bufferToFill.buffer->getWritePointer(0);
    auto* wpR = bufferToFill.buffer->getWritePointer(1);
    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= sr;
	double increment = pow(A, 4) / (50.0 * overallscale);
	double wet = B;

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        long double inputSampleL = *rpL;
        long double inputSampleR = *rpR;

		if (fabs(inputSampleL) < 1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (fabs(inputSampleR) < 1.18e-37) inputSampleR = fpd * 1.18e-37;
		long double drySampleL = inputSampleL;
		long double drySampleR = inputSampleR;

		osc += increment; if (osc > 1.0) osc = 0.0;
		//this is our little oscillator code

		if (inputSampleL < 0) {
			if (polarityL == true) muteL = (osc < 0.5);
			polarityL = false;
		}
		else {
			if (polarityL == false) muteL = (osc < 0.5);
			polarityL = true;
		}

		if (inputSampleR < 0) {
			if (polarityR == true) muteR = (osc < 0.5);
			polarityR = false;
		}
		else {
			if (polarityR == false) muteR = (osc < 0.5);
			polarityR = true;
		}

		if (muteL) inputSampleL = 0.0;
		if (muteR) inputSampleR = 0.0;

		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0 - wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0 - wet));
		}

		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
		frexpf((float)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += ((double(fpd) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
		//end 32 bit stereo floating point dither

		*wpL = inputSampleL;
		*wpR = inputSampleR;
		++rpL;
		++rpR;
		++wpL;
		++wpR;
    }
}

void TremoSquareProcessor::releaseResources()
{

}