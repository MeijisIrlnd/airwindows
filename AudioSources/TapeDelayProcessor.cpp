/* ========================================
 *  TapeDelay - TapeDelay.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#include "TapeDelayProcessor.h"

TapeDelayProcessor::TapeDelayProcessor()
{
    paramBindings = {
        {DELAY, &C},
        {FEEDBACK, &D},
        {LEAN_FT, &E},
        {DEPTH, &F}
    };
	A = 1.0;
	B = 0.0;
	C = 0.5;
	D = 0.0; //0 to 130%
	E = 1.0; //-1.0 to 1.0
	F = 0.0; //8 taps

	for (int count = 0; count < 257; count++) { pL[count] = 0; pR[count] = 0; }
	for (delay = 0; delay < 44100; delay++) { dL[delay] = 0.0; dR[delay] = 0.0; }
	maxdelay = 0;
	delay = 0;
	gcount = 0;
	chase = 0;

	fpNShapeL = 0.0;
	fpNShapeR = 0.0;
}


TapeDelayProcessor::~TapeDelayProcessor()
{

}

void TapeDelayProcessor::setParam(PARAMETER p, float v)
{
    // Combine dw into one...
    if (p == DRY_WET)
    {
        // 0 dry 1 wet = 0 A 1B
        //1 dry 0 wet = 1A 0B
        B = v;
        A = 1 - v;
    }
    else {
        if (paramBindings.find(p) != paramBindings.end())
        {
            *paramBindings[p] = v;
        }
    }
}
void TapeDelayProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	sr = sampleRate;
}

void TapeDelayProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* rpL = bufferToFill.buffer->getReadPointer(0);
    auto* rpR = bufferToFill.buffer->getReadPointer(1);
    auto* wpL = bufferToFill.buffer->getWritePointer(0);
    auto* wpR = bufferToFill.buffer->getWritePointer(1);
    double dry = pow(A, 2);
    double wet = pow(B, 2);
    int targetdelay = (int)(44000 * C);
    double feedback = (D * 1.3);
    double leanfat = ((E * 2.0) - 1.0);
    double fatwet = fabs(leanfat);
    int fatness = (int)floor((F * 29.0) + 3.0);
    int count;

    double storedelayL;
    double storedelayR;
    double sumL = 0.0;
    double sumR = 0.0;
    double floattotalL = 0.0;
    double floattotalR = 0.0;
    int sumtotalL = 0;
    int sumtotalR = 0;

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        long double inputSampleL = *rpL;
        long double inputSampleR = *rpR;
		static int noisesourceL = 0;
		static int noisesourceR = 850010;
		int residue;
		double applyresidue;

		noisesourceL = noisesourceL % 1700021; noisesourceL++;
		residue = noisesourceL * noisesourceL;
		residue = residue % 170003; residue *= residue;
		residue = residue % 17011; residue *= residue;
		residue = residue % 1709; residue *= residue;
		residue = residue % 173; residue *= residue;
		residue = residue % 17;
		applyresidue = residue;
		applyresidue *= 0.00000001;
		applyresidue *= 0.00000001;
		inputSampleL += applyresidue;
		if (inputSampleL < 1.2e-38 && -inputSampleL < 1.2e-38) {
			inputSampleL -= applyresidue;
		}

		noisesourceR = noisesourceR % 1700021; noisesourceR++;
		residue = noisesourceR * noisesourceR;
		residue = residue % 170003; residue *= residue;
		residue = residue % 17011; residue *= residue;
		residue = residue % 1709; residue *= residue;
		residue = residue % 173; residue *= residue;
		residue = residue % 17;
		applyresidue = residue;
		applyresidue *= 0.00000001;
		applyresidue *= 0.00000001;
		inputSampleR += applyresidue;
		if (inputSampleR < 1.2e-38 && -inputSampleR < 1.2e-38) {
			inputSampleR -= applyresidue;
		}
		//for live air, we always apply the dither noise. Then, if our result is 
		//effectively digital black, we'll subtract it again. We want a 'air' hiss

		if (gcount < 0 || gcount > 128) { gcount = 128; }
		count = gcount;
		if (delay < 0 || delay > maxdelay) { delay = maxdelay; }

		sumL = inputSampleL + (dL[delay] * feedback);
		sumR = inputSampleR + (dR[delay] * feedback);
		pL[count + 128] = pL[count] = sumtotalL = (int)(sumL * 8388608.0);
		pR[count + 128] = pR[count] = sumtotalR = (int)(sumR * 8388608.0);

		switch (fatness)
		{
		case 32: sumtotalL += pL[count + 127]; sumtotalR += pR[count + 127]; //note NO break statement.
		case 31: sumtotalL += pL[count + 113]; sumtotalR += pR[count + 113]; //This jumps to the relevant tap
		case 30: sumtotalL += pL[count + 109]; sumtotalR += pR[count + 109]; //and then includes all smaller taps.
		case 29: sumtotalL += pL[count + 107]; sumtotalR += pR[count + 107];
		case 28: sumtotalL += pL[count + 103]; sumtotalR += pR[count + 103];
		case 27: sumtotalL += pL[count + 101]; sumtotalR += pR[count + 101];
		case 26: sumtotalL += pL[count + 97]; sumtotalR += pR[count + 97];
		case 25: sumtotalL += pL[count + 89]; sumtotalR += pR[count + 89];
		case 24: sumtotalL += pL[count + 83]; sumtotalR += pR[count + 83];
		case 23: sumtotalL += pL[count + 79]; sumtotalR += pR[count + 79];
		case 22: sumtotalL += pL[count + 73]; sumtotalR += pR[count + 73];
		case 21: sumtotalL += pL[count + 71]; sumtotalR += pR[count + 71];
		case 20: sumtotalL += pL[count + 67]; sumtotalR += pR[count + 67];
		case 19: sumtotalL += pL[count + 61]; sumtotalR += pR[count + 61];
		case 18: sumtotalL += pL[count + 59]; sumtotalR += pR[count + 59];
		case 17: sumtotalL += pL[count + 53]; sumtotalR += pR[count + 53];
		case 16: sumtotalL += pL[count + 47]; sumtotalR += pR[count + 47];
		case 15: sumtotalL += pL[count + 43]; sumtotalR += pR[count + 43];
		case 14: sumtotalL += pL[count + 41]; sumtotalR += pR[count + 41];
		case 13: sumtotalL += pL[count + 37]; sumtotalR += pR[count + 37];
		case 12: sumtotalL += pL[count + 31]; sumtotalR += pR[count + 31];
		case 11: sumtotalL += pL[count + 29]; sumtotalR += pR[count + 29];
		case 10: sumtotalL += pL[count + 23]; sumtotalR += pR[count + 23];
		case 9: sumtotalL += pL[count + 19]; sumtotalR += pR[count + 19];
		case 8: sumtotalL += pL[count + 17]; sumtotalR += pR[count + 17];
		case 7: sumtotalL += pL[count + 13]; sumtotalR += pR[count + 13];
		case 6: sumtotalL += pL[count + 11]; sumtotalR += pR[count + 11];
		case 5: sumtotalL += pL[count + 7]; sumtotalR += pR[count + 7];
		case 4: sumtotalL += pL[count + 5]; sumtotalR += pR[count + 5];
		case 3: sumtotalL += pL[count + 3]; sumtotalR += pR[count + 3];
		case 2: sumtotalL += pL[count + 2]; sumtotalR += pR[count + 2];
		case 1: sumtotalL += pL[count + 1]; sumtotalR += pR[count + 1];
		}

		floattotalL = (double)(sumtotalL / fatness + 1);
		floattotalR = (double)(sumtotalR / fatness + 1);
		floattotalL /= 8388608.0;
		floattotalR /= 8388608.0;
		floattotalL *= fatwet;
		floattotalR *= fatwet;
		if (leanfat < 0) { storedelayL = sumL - floattotalL; storedelayR = sumR - floattotalR; }
		else { storedelayL = (sumL * (1 - fatwet)) + floattotalL; storedelayR = (sumR * (1 - fatwet)) + floattotalR; }

		chase += abs(maxdelay - targetdelay);
		if (chase > 9000)
		{
			if (maxdelay > targetdelay) {
				dL[delay] = storedelayL; dR[delay] = storedelayR;
				maxdelay -= 1; delay -= 1; if (delay < 0) { delay = maxdelay; }
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			if (maxdelay < targetdelay) {
				maxdelay += 1; delay += 1; if (delay > maxdelay) { delay = 0; }
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			chase = 0;
		}
		else
		{
			dL[delay] = storedelayL; dR[delay] = storedelayR;
		}

		gcount--;
		delay--;
		if (delay < 0 || delay > maxdelay) { delay = maxdelay; }
		//yes this is a second bounds check. it's cheap, check EVERY time

		inputSampleL = (inputSampleL * dry) + (dL[delay] * wet);
		inputSampleR = (inputSampleR * dry) + (dR[delay] * wet);

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

void TapeDelayProcessor::releaseResources()
{

}
