/* ========================================
 *  ToneSlant - ToneSlant.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#include "ToneSlantProcessor.h"

ToneSlantProcessor::ToneSlantProcessor()
{
    paramBindings =
    {
        {VOICING, &A},
        {HIGHS, &B}
    };
    A = 0.0;
    B = 0.0;
    for (int count = 0; count < 102; count++) { bL[count] = 0.0; bR[count] = 0.0; f[count] = 0.0; }
    fpNShapeL = 0.0;
    fpNShapeR = 0.0;
}

ToneSlantProcessor::~ToneSlantProcessor()
{

}

void ToneSlantProcessor::setParam(PARAMETER p, float v)
{
    if (paramBindings.find(p) != paramBindings.end())
    {
        *paramBindings[p] = v;
    }
}


void ToneSlantProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void ToneSlantProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* rpL = bufferToFill.buffer->getReadPointer(0);
    auto* rpR = bufferToFill.buffer->getReadPointer(1);
    auto* wpL = bufferToFill.buffer->getWritePointer(0);
    auto* wpR = bufferToFill.buffer->getWritePointer(1);

    double inputSampleL;
    double inputSampleR;
    double correctionSampleL;
    double correctionSampleR;
    double accumulatorSampleL;
    double accumulatorSampleR;
    double drySampleL;
    double drySampleR;
    double overallscale = (A * 99.0) + 1.0;
    double applySlant = (B * 2.0) - 1.0;


    f[0] = 1.0 / overallscale;
    //count to f(gain) which will be 0. f(0) is x1
    for (int count = 1; count < 102; count++) {
        if (count <= overallscale) {
            f[count] = (1.0 - (count / overallscale)) / overallscale;
            //recalc the filter and don't change the buffer it'll apply to
        }
        else {
            bL[count] = 0.0; //blank the unused buffer so when we return to it, no pops
            bR[count] = 0.0; //blank the unused buffer so when we return to it, no pops
        }
    }
    
    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        for (int count = overallscale; count >= 0; count--) {
            bL[count + 1] = bL[count];
            bR[count + 1] = bR[count];
        }
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

		bL[0] = accumulatorSampleL = drySampleL = inputSampleL;
		bR[0] = accumulatorSampleR = drySampleR = inputSampleR;

		accumulatorSampleL *= f[0];
		accumulatorSampleR *= f[0];

		for (int count = 1; count < overallscale; count++) {
			accumulatorSampleL += (bL[count] * f[count]);
			accumulatorSampleR += (bR[count] * f[count]);
		}

		correctionSampleL = inputSampleL - (accumulatorSampleL * 2.0);
		correctionSampleR = inputSampleR - (accumulatorSampleR * 2.0);
		//we're gonna apply the total effect of all these calculations as a single subtract

		inputSampleL += (correctionSampleL * applySlant);
		inputSampleR += (correctionSampleR * applySlant);
		//our one math operation on the input data coming in

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

void ToneSlantProcessor::releaseResources()
{

}
