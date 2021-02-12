/* ========================================
 *  Swell - Swell.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#include "SwellProcessor.h"

SwellProcessor::SwellProcessor()
{
	paramBindings =
	{
		{THRESHOLD, &A},
		{SWELL, &B},
		{DRY_WET, &C}
	};

	A = 0.9;
	B = 0.5;
	C = 1.0;

	swellL = 0.0;
	swellR = 0.0;
	louderL = false;
	louderR = false;

	fpNShapeL = 0.0;
	fpNShapeR = 0.0;
}

SwellProcessor::~SwellProcessor()
{

}

void SwellProcessor::setParam(PARAMETER p, float value)
{
	if (paramBindings.find(p) != paramBindings.end())
	{
		*paramBindings[p] = value;
	}
}


void SwellProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	sr = sampleRate;
}

void SwellProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto* rpL = bufferToFill.buffer->getReadPointer(0);
	auto* rpR = bufferToFill.buffer->getReadPointer(1);
	auto* wpL = bufferToFill.buffer->getWritePointer(0);
	auto* wpR = bufferToFill.buffer->getWritePointer(1);
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= sr;

	double thresholdOn = pow(A, 2) * B;
	double speedOn = (pow(B, 2) * 0.001) / overallscale;
	double thresholdOff = thresholdOn * B;
	double speedOff = (sin(B) * 0.01) / overallscale;
	double wet = C;
	double dry = 1.0 - wet;

	long double drySampleL;
	long double drySampleR;
	long double inputSampleL;
	long double inputSampleR;
	for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		inputSampleL = *rpL;
		inputSampleR = *rpR;
		if (inputSampleL < 1.2e-38 && -inputSampleL < 1.2e-38) {
			static int noisesource = 0;
			//this declares a variable before anything else is compiled. It won't keep assigning
			//it to 0 for every sample, it's as if the declaration doesn't exist in this context,
			//but it lets me add this denormalization fix in a single place rather than updating
			//it in three different locations. The variable isn't thread-safe but this is only
			//a random seed and we can share it with whatever.
			noisesource = noisesource % 1700021; noisesource++;
			int residue = noisesource * noisesource;
			residue = residue % 170003; residue *= residue;
			residue = residue % 17011; residue *= residue;
			residue = residue % 1709; residue *= residue;
			residue = residue % 173; residue *= residue;
			residue = residue % 17;
			double applyresidue = residue;
			applyresidue *= 0.00000001;
			applyresidue *= 0.00000001;
			inputSampleL = applyresidue;
		}
		if (inputSampleR < 1.2e-38 && -inputSampleR < 1.2e-38) {
			static int noisesource = 0;
			noisesource = noisesource % 1700021; noisesource++;
			int residue = noisesource * noisesource;
			residue = residue % 170003; residue *= residue;
			residue = residue % 17011; residue *= residue;
			residue = residue % 1709; residue *= residue;
			residue = residue % 173; residue *= residue;
			residue = residue % 17;
			double applyresidue = residue;
			applyresidue *= 0.00000001;
			applyresidue *= 0.00000001;
			inputSampleR = applyresidue;
			//this denormalization routine produces a white noise at -300 dB which the noise
			//shaping will interact with to produce a bipolar output, but the noise is actually
			//all positive. That should stop any variables from going denormal, and the routine
			//only kicks in if digital black is input. As a final touch, if you save to 24-bit
			//the silence will return to being digital black again.
		}
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		if (fabs(inputSampleL) > thresholdOn && louderL == false) louderL = true;
		if (fabs(inputSampleL) < thresholdOff && louderL == true) louderL = false;
		if (louderL == true) swellL = (swellL * (1.0 - speedOn)) + speedOn;
		else swellL *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleL *= swellL;

		if (fabs(inputSampleR) > thresholdOn && louderR == false) louderR = true;
		if (fabs(inputSampleR) < thresholdOff && louderR == true) louderR = false;
		if (louderR == true) swellR = (swellR * (1.0 - speedOn)) + speedOn;
		else swellR *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleR *= swellR;

		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * dry);
			inputSampleR = (inputSampleR * wet) + (drySampleR * dry);
		}

		//stereo 32 bit dither, made small and tidy.
		int expon; frexpf((float)inputSampleL, &expon);
		long double dither = (rand() / (RAND_MAX * 7.737125245533627e+25)) * pow(2, expon + 62);
		inputSampleL += (dither - fpNShapeL); fpNShapeL = dither;
		frexpf((float)inputSampleR, &expon);
		dither = (rand() / (RAND_MAX * 7.737125245533627e+25)) * pow(2, expon + 62);
		inputSampleR += (dither - fpNShapeR); fpNShapeR = dither;
		//end 32 bit dither
		*wpL = inputSampleL;
		*wpR = inputSampleR;
		++rpL;
		++rpR;
		++wpL;
		++wpR;
	}
}

void SwellProcessor::releaseResources()
{

}
