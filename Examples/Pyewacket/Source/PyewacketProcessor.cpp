/*
  ==============================================================================

    PyewacketProcessor.cpp
    Created: 10 Feb 2021 5:35:35pm
    Author:  Syl

  ==============================================================================
*/

#include "PyewacketProcessor.h"

PyewacketProcessor::PyewacketProcessor()
{
    paramBindings =
    {
        {INPUT_GAIN, &A},
        {RELEASE, &B},
        {OUTPUT_GAIN, &C}
    };
    A = 0.5;
    B = 0.5;
    C = 0.5;
    chase = 1.0;
    lastrectifierL = 0.0;
    lastrectifierR = 0.0;
    fpNShapeL = 0.0;
    fpNShapeR = 0.0;
}

PyewacketProcessor::~PyewacketProcessor()
{

}

void PyewacketProcessor::setParam(PARAMETER p, float value)
{
    if (paramBindings.find(p) != paramBindings.end())
    {
        *paramBindings[p] = value;
    }
}

void PyewacketProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void PyewacketProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* rpL = bufferToFill.buffer->getReadPointer(0);
    auto* rpR = bufferToFill.buffer->getReadPointer(1);
    auto* wpL = bufferToFill.buffer->getWritePointer(0);
    auto* wpR = bufferToFill.buffer->getWritePointer(1);
    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= sr;
    long double fpOld = 0.618033988749894848204586; //golden ratio!
    long double fpNew = 1.0 - fpOld;
    long double inputSampleL;
    long double inputSampleR;
    long double drySampleL;
    long double drySampleR;
    double bridgerectifier;
    double temprectifier;
    double inputSense;

    double inputGain = pow(10.0, ((A * 24.0) - 12.0) / 20.0);
    double attack = ((B + 0.5) * 0.006) / overallscale;
    double decay = ((B + 0.01) * 0.0004) / overallscale;
    double outputGain = pow(10.0, ((C * 24.0) - 12.0) / 20.0);
    double wet;
    double maxblur;
    double blurdry;
    double out;
    double dry;
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

		if (inputGain != 1.0) {
			inputSampleL *= inputGain;
			inputSampleR *= inputGain;
		}
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		//we will take the greater of either channel and just use that, then apply the result
		//to both stereo channels.
		if (chase < inputSense) chase += attack;
		if (chase > 1.0) chase = 1.0;
		if (chase > inputSense) chase -= decay;
		if (chase < 0.0) chase = 0.0;
		//chase will be between 0 and ? (if input is super hot)
		out = wet = chase;
		if (wet > 1.0) wet = 1.0;
		maxblur = wet * fpNew;
		blurdry = 1.0 - maxblur;
		//scaled back so that blur remains balance of both
		if (out > fpOld) out = fpOld - (out - fpOld);
		if (out < 0.0) out = 0.0;
		dry = 1.0 - wet;

		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;

		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		temprectifier = 1 - cos(bridgerectifier);
		bridgerectifier = ((lastrectifierL * maxblur) + (temprectifier * blurdry));
		lastrectifierL = temprectifier;
		//starved version is also blurred by one sample
		if (inputSampleL > 0) inputSampleL = (inputSampleL * dry) + (bridgerectifier * out);
		else inputSampleL = (inputSampleL * dry) - (bridgerectifier * out);

		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		temprectifier = 1 - cos(bridgerectifier);
		bridgerectifier = ((lastrectifierR * maxblur) + (temprectifier * blurdry));
		lastrectifierR = temprectifier;
		//starved version is also blurred by one sample
		if (inputSampleR > 0) inputSampleR = (inputSampleR * dry) + (bridgerectifier * out);
		else inputSampleR = (inputSampleR * dry) - (bridgerectifier * out);

		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
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

void PyewacketProcessor::releaseResources()
{

}