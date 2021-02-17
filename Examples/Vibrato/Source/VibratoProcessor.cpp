/* ========================================
 *  Vibrato - Vibrato.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#include "VibratoProcessor.h"

VibratoProcessor::VibratoProcessor()
{
    paramBindings =
    {
        {SPEED, &A},
        {DEPTH, &B},
        {FMSPEED, &C},
        {FMDEPTH, &D},
        {INV_WET, &E}
    };

	A = 0.3;
	B = 0.0;
	C = 0.4;
	D = 0.0;
	E = 1.0;

	for (int count = 0; count < 16385; count++) { pL[count] = 0.0; pR[count] = 0.0; }
	sweep = 3.141592653589793238 / 2.0;
	sweepB = 3.141592653589793238 / 2.0;
	gcount = 0;

	airPrevL = 0.0;
	airEvenL = 0.0;
	airOddL = 0.0;
	airFactorL = 0.0;
	airPrevR = 0.0;
	airEvenR = 0.0;
	airOddR = 0.0;
	airFactorR = 0.0;

	flip = false;

	fpd = 17;

}

VibratoProcessor::~VibratoProcessor()
{

}

void VibratoProcessor::setParam(PARAMETER p, float v)
{
    if (paramBindings.find(p) != paramBindings.end())
    {
        *paramBindings[p] = v;
    }
}

void VibratoProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void VibratoProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto* rpL = bufferToFill.buffer->getReadPointer(0);
	auto* rpR = bufferToFill.buffer->getReadPointer(1);
	auto* wpL = bufferToFill.buffer->getWritePointer(0);
	auto* wpR = bufferToFill.buffer->getWritePointer(1);
	double speed = pow(0.1 + A, 6);
	double depth = (pow(B, 3) / sqrt(speed)) * 4.0;
	double speedB = pow(0.1 + C, 6);
	double depthB = pow(D, 3) / sqrt(speedB);
	double tupi = 3.141592653589793238 * 2.0;
	double wet = (E * 2.0) - 1.0; //note: inv/dry/wet
	for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		long double inputSampleL = *rpL;
		long double inputSampleR = *rpR;
		if (fabs(inputSampleL) < 1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (fabs(inputSampleR) < 1.18e-37) inputSampleR = fpd * 1.18e-37;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		airFactorL = airPrevL - inputSampleL;
		airFactorR = airPrevR - inputSampleR;

		if (flip) {
			airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;
			airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;
		}
		else {
			airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;
			airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;
		}

		airOddL = (airOddL - ((airOddL - airEvenL) / 256.0)) / 1.0001;
		airOddR = (airOddR - ((airOddR - airEvenR) / 256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL) / 256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR) / 256.0)) / 1.0001;
		airPrevL = inputSampleL;
		airPrevR = inputSampleR;
		inputSampleL += airFactorL;
		inputSampleR += airFactorR;

		flip = !flip;
		//air, compensates for loss of highs in the interpolation

		if (gcount < 1 || gcount > 8192) { gcount = 8192; }
		int count = gcount;
		pL[count + 8192] = pL[count] = inputSampleL;
		pR[count + 8192] = pR[count] = inputSampleR;

		double offset = depth + (depth * sin(sweep));
		count += (int)floor(offset);

		inputSampleL = pL[count] * (1.0 - (offset - floor(offset))); //less as value moves away from .0
		inputSampleL += pL[count + 1]; //we can assume always using this in one way or another?
		inputSampleL += pL[count + 2] * (offset - floor(offset)); //greater as value moves away from .0
		inputSampleL -= ((pL[count] - pL[count + 1]) - (pL[count + 1] - pL[count + 2])) / 50.0; //interpolation hacks 'r us
		inputSampleL *= 0.5; // gain trim

		inputSampleR = pR[count] * (1.0 - (offset - floor(offset))); //less as value moves away from .0
		inputSampleR += pR[count + 1]; //we can assume always using this in one way or another?
		inputSampleR += pR[count + 2] * (offset - floor(offset)); //greater as value moves away from .0
		inputSampleR -= ((pR[count] - pR[count + 1]) - (pR[count + 1] - pR[count + 2])) / 50.0; //interpolation hacks 'r us
		inputSampleR *= 0.5; // gain trim

		sweep += (speed + (speedB * sin(sweepB) * depthB));
		sweepB += speedB;
		if (sweep > tupi) { sweep -= tupi; }
		if (sweep < 0.0) { sweep += tupi; } //through zero FM
		if (sweepB > tupi) { sweepB -= tupi; }
		gcount--;
		//still scrolling through the samples, remember

		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0 - fabs(wet)));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0 - fabs(wet)));
		}
		//Inv/Dry/Wet control

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

void VibratoProcessor::releaseResources()
{

}
