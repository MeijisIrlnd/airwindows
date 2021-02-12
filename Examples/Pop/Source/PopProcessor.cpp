/* ========================================
 *  Pop - Pop.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#include "PopProcessor.h"

PopProcessor::PopProcessor()
{
	paramBindings = {
		{INTENSITY, &A},
		{OUTPUT, &B},
		{DRY_WET, &C}
	};

	A = 0.3;
	B = 1.0;
	C = 1.0;
	fpd = 17;

	for (int count = 0; count < 10000; count++) { dL[count] = 0; dR[count] = 0; }
	delay = 0;
	flip = false;

	muSpeedAL = 10000;
	muSpeedBL = 10000;
	muCoefficientAL = 1;
	muCoefficientBL = 1;
	thickenL = 1;
	muVaryL = 1;
	previousL = 0.0;
	previous2L = 0.0;
	previous3L = 0.0;
	previous4L = 0.0;
	previous5L = 0.0;

	muSpeedAR = 10000;
	muSpeedBR = 10000;
	muCoefficientAR = 1;
	muCoefficientBR = 1;
	thickenR = 1;
	muVaryR = 1;
	previousR = 0.0;
	previous2R = 0.0;
	previous3R = 0.0;
	previous4R = 0.0;
	previous5R = 0.0;
}

PopProcessor::~PopProcessor()
{

}

void PopProcessor::setParam(PARAMETER p, float value)
{
	if (paramBindings.find(p) != paramBindings.end())
	{
		*paramBindings[p] = value;
	}
}


void PopProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void PopProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto* rpL = bufferToFill.buffer->getReadPointer(0);
	auto* rpR = bufferToFill.buffer->getReadPointer(1);
	auto* wpL = bufferToFill.buffer->getWritePointer(0);
	auto* wpR = bufferToFill.buffer->getWritePointer(1);
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= sr;

	double highGainOffset = pow(A, 2) * 0.023;
	double threshold = 1.001 - (1.0 - pow(1.0 - A, 5));
	double muMakeupGain = sqrt(1.0 / threshold);
	//gain settings around threshold
	double release = (A * 100000.0) + 300000.0;
	int maxdelay = (int)(1450.0 * overallscale);
	if (maxdelay > 9999) maxdelay = 9999;
	release /= overallscale;
	double fastest = sqrt(release);
	//speed settings around release
	double output = B;
	double wet = C;
	for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		long double inputSampleL = *rpL;
		long double inputSampleR = *rpR;
		if (std::fabs(inputSampleL) < 1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (std::fabs(inputSampleR) < 1.18e-37) inputSampleR = fpd * 1.18e-37;

		long double drySampleL = inputSampleL;
		long double drySampleR = inputSampleR;

		dL[delay] = inputSampleL;
		dR[delay] = inputSampleR;
		delay--;
		if (delay < 0 || delay > maxdelay) { delay = maxdelay; }
		//yes this is a second bounds check. it's cheap, check EVERY time
		inputSampleL = (inputSampleL * thickenL) + (dL[delay] * (1.0 - thickenL));
		inputSampleR = (inputSampleR * thickenR) + (dR[delay] * (1.0 - thickenR));

		long double lowestSampleL = inputSampleL;
		if (std::fabs(inputSampleL) > std::fabs(previousL)) lowestSampleL = previousL;
		if (std::fabs(lowestSampleL) > std::fabs(previous2L)) lowestSampleL = (lowestSampleL + previous2L) / 1.99;
		if (std::fabs(lowestSampleL) > std::fabs(previous3L)) lowestSampleL = (lowestSampleL + previous3L) / 1.98;
		if (std::fabs(lowestSampleL) > std::fabs(previous4L)) lowestSampleL = (lowestSampleL + previous4L) / 1.97;
		if (std::fabs(lowestSampleL) > std::fabs(previous5L)) lowestSampleL = (lowestSampleL + previous5L) / 1.96;
		previous5L = previous4L;
		previous4L = previous3L;
		previous3L = previous2L;
		previous2L = previousL;
		previousL = inputSampleL;
		inputSampleL *= muMakeupGain;
		double punchinessL = 0.95 - std::fabs(inputSampleL * 0.08);
		if (punchinessL < 0.65) punchinessL = 0.65;
		long double lowestSampleR = inputSampleR;
		if (std::fabs(inputSampleR) > std::fabs(previousR)) lowestSampleR = previousR;
		if (std::fabs(lowestSampleR) > std::fabs(previous2R)) lowestSampleR = (lowestSampleR + previous2R) / 1.99;
		if (std::fabs(lowestSampleR) > std::fabs(previous3R)) lowestSampleR = (lowestSampleR + previous3R) / 1.98;
		if (std::fabs(lowestSampleR) > std::fabs(previous4R)) lowestSampleR = (lowestSampleR + previous4R) / 1.97;
		if (std::fabs(lowestSampleR) > std::fabs(previous5R)) lowestSampleR = (lowestSampleR + previous5R) / 1.96;
		previous5R = previous4R;
		previous4R = previous3R;
		previous3R = previous2R;
		previous2R = previousR;
		previousR = inputSampleR;
		inputSampleR *= muMakeupGain;
		double punchinessR = 0.95 - std::fabs(inputSampleR * 0.08);
		if (punchinessR < 0.65) punchinessR = 0.65;
		//adjust coefficients for L
		if (flip)
		{
			if (std::fabs(lowestSampleL) > threshold)
			{
				muVaryL = threshold / std::fabs(lowestSampleL);
				muAttackL = std::sqrt(std::fabs(muSpeedAL));
				muCoefficientAL = muCoefficientAL * (muAttackL - 1.0);
				if (muVaryL < threshold)
				{
					muCoefficientAL = muCoefficientAL + threshold;
				}
				else
				{
					muCoefficientAL = muCoefficientAL + muVaryL;
				}
				muCoefficientAL = muCoefficientAL / muAttackL;
			}
			else
			{
				muCoefficientAL = muCoefficientAL * ((muSpeedAL * muSpeedAL) - 1.0);
				muCoefficientAL = muCoefficientAL + 1.0;
				muCoefficientAL = muCoefficientAL / (muSpeedAL * muSpeedAL);
			}
			muNewSpeedL = muSpeedAL * (muSpeedAL - 1);
			muNewSpeedL = muNewSpeedL + std::fabs(lowestSampleL * release) + fastest;
			muSpeedAL = muNewSpeedL / muSpeedAL;
		}
		else
		{
			if (std::fabs(lowestSampleL) > threshold)
			{
				muVaryL = threshold / std::fabs(lowestSampleL);
				muAttackL = std::sqrt(std::fabs(muSpeedBL));
				muCoefficientBL = muCoefficientBL * (muAttackL - 1);
				if (muVaryL < threshold)
				{
					muCoefficientBL = muCoefficientBL + threshold;
				}
				else
				{
					muCoefficientBL = muCoefficientBL + muVaryL;
				}
				muCoefficientBL = muCoefficientBL / muAttackL;
			}
			else
			{
				muCoefficientBL = muCoefficientBL * ((muSpeedBL * muSpeedBL) - 1.0);
				muCoefficientBL = muCoefficientBL + 1.0;
				muCoefficientBL = muCoefficientBL / (muSpeedBL * muSpeedBL);
			}
			muNewSpeedL = muSpeedBL * (muSpeedBL - 1);
			muNewSpeedL = muNewSpeedL + std::fabs(lowestSampleL * release) + fastest;
			muSpeedBL = muNewSpeedL / muSpeedBL;

		}
		//got coefficients, adjusted speeds for L
		//adjust coefficients for R
		if (flip)
		{
			if (std::fabs(lowestSampleR) > threshold)
			{
				muVaryR = threshold / std::fabs(lowestSampleR);
				muAttackR = std::sqrt(std::fabs(muSpeedAR));
				muCoefficientAR = muCoefficientAR * (muAttackR - 1.0);
				if (muVaryR < threshold)
				{
					muCoefficientAR = muCoefficientAR + threshold;
				}
				else
				{
					muCoefficientAR = muCoefficientAR + muVaryR;
				}
				muCoefficientAR = muCoefficientAR / muAttackR;
			}
			else
			{
				muCoefficientAR = muCoefficientAR * ((muSpeedAR * muSpeedAR) - 1.0);
				muCoefficientAR = muCoefficientAR + 1.0;
				muCoefficientAR = muCoefficientAR / (muSpeedAR * muSpeedAR);
			}
			muNewSpeedR = muSpeedAR * (muSpeedAR - 1);
			muNewSpeedR = muNewSpeedR + std::fabs(lowestSampleR * release) + fastest;
			muSpeedAR = muNewSpeedR / muSpeedAR;
		}
		else
		{
			if (std::fabs(lowestSampleR) > threshold)
			{
				muVaryR = threshold / std::fabs(lowestSampleR);
				muAttackR = std::sqrt(std::fabs(muSpeedBR));
				muCoefficientBR = muCoefficientBR * (muAttackR - 1);
				if (muVaryR < threshold)
				{
					muCoefficientBR = muCoefficientBR + threshold;
				}
				else
				{
					muCoefficientBR = muCoefficientBR + muVaryR;
				}
				muCoefficientBR = muCoefficientBR / muAttackR;
			}
			else
			{
				muCoefficientBR = muCoefficientBR * ((muSpeedBR * muSpeedBR) - 1.0);
				muCoefficientBR = muCoefficientBR + 1.0;
				muCoefficientBR = muCoefficientBR / (muSpeedBR * muSpeedBR);
			}
			muNewSpeedR = muSpeedBR * (muSpeedBR - 1);
			muNewSpeedR = muNewSpeedR + std::fabs(lowestSampleR * release) + fastest;
			muSpeedBR = muNewSpeedR / muSpeedBR;
		}
		//got coefficients, adjusted speeds for R
		long double coefficientL = highGainOffset;
		if (flip) coefficientL += std::pow(muCoefficientAL, 2);
		else coefficientL += std::pow(muCoefficientBL, 2);
		inputSampleL *= coefficientL;
		thickenL = (coefficientL / 5) + punchinessL;//0.80;
		thickenL = (1.0 - wet) + (wet * thickenL);

		long double coefficientR = highGainOffset;
		if (flip) coefficientR += std::pow(muCoefficientAR, 2);
		else coefficientR += std::pow(muCoefficientBR, 2);
		inputSampleR *= coefficientR;
		thickenR = (coefficientR / 5) + punchinessR;//0.80;
		thickenR = (1.0 - wet) + (wet * thickenR);

		//applied gain correction to control output level- tends to constrain sound rather than inflate it

		long double bridgerectifier = std::fabs(inputSampleL);
		if (bridgerectifier > 1.2533141373155) bridgerectifier = 1.2533141373155;
		bridgerectifier = std::sin(bridgerectifier * std::fabs(bridgerectifier)) / ((std::fabs(bridgerectifier) == 0.0) ? 1 : std::fabs(bridgerectifier));
		//using Spiral instead of Density algorithm
		if (inputSampleL > 0) inputSampleL = (inputSampleL * coefficientL) + (bridgerectifier * (1 - coefficientL));
		else inputSampleL = (inputSampleL * coefficientL) - (bridgerectifier * (1 - coefficientL));
		//second stage of overdrive to prevent overs and allow bloody loud extremeness

		bridgerectifier = std::fabs(inputSampleR);
		if (bridgerectifier > 1.2533141373155) bridgerectifier = 1.2533141373155;
		bridgerectifier = std::sin(bridgerectifier * std::fabs(bridgerectifier)) / ((std::fabs(bridgerectifier) == 0.0) ? 1 : std::fabs(bridgerectifier));
		//using Spiral instead of Density algorithm
		if (inputSampleR > 0) inputSampleR = (inputSampleR * coefficientR) + (bridgerectifier * (1 - coefficientR));
		else inputSampleR = (inputSampleR * coefficientR) - (bridgerectifier * (1 - coefficientR));
		//second stage of overdrive to prevent overs and allow bloody loud extremeness

		flip = !flip;

		if (output < 1.0) { inputSampleL *= output; inputSampleR *= output; }
		if (wet < 1.0) {
			inputSampleL = (drySampleL * (1.0 - wet)) + (inputSampleL * wet);
			inputSampleR = (drySampleR * (1.0 - wet)) + (inputSampleR * wet);
		}
		//begin 32 bit stereo floating point dither
		int expon; std::frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * std::pow(2, expon + 62);
		std::frexpf((float)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * std::pow(2, expon + 62);
		//end 32 bit stereo floating point dither
		*wpL = inputSampleL;
		*wpR = inputSampleR;
		++rpL;
		++rpR;
		++wpL;
		++wpR;
	}

}

void PopProcessor::releaseResources()
{

}
