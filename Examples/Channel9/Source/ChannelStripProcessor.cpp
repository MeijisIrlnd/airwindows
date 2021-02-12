/* ========================================
 *  Channel9 - Channel9.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== 
 */

#include "ChannelStripProcessor.h"


ChannelStripProcessor::ChannelStripProcessor() : currentMode(NEVE)
{
    paramBindings =
    {
        {INPUT_GAIN, &B},
        {OUTPUT_GAIN, &C}
    };

    A = 0.0;
    B = 1.0;
    C = 1.0;
    for (auto i = 0; i < 15; i++) {
        biquadA[i] = 0.0;
        biquadB[i] = 0.0;
    }
    fpdL = 1.0;
    while (fpdL < 16386) {
        fpdL = std::rand() * UINT32_MAX;
    }
    fpdR = 1.0;
    while (fpdR < 16386) {
        fpdR = std::rand() * UINT32_MAX;
    }
    iirSampleLA = 0.0;
    iirSampleRA = 0.0;
    iirSampleLB = 0.0;
    iirSampleRB = 0.0;

    lastSampleAL = lastSampleBL = lastSampleCL = 0.0;
    lastSampleAR = lastSampleBR = lastSampleCR = 0.0;
    flip = false;
    iirAmount = 0.005832;
    threshold = 0.33362176;
    cutoff = 28811.0;
}

ChannelStripProcessor::~ChannelStripProcessor()
{

}

void ChannelStripProcessor::switchMode(MODE m)
{
    switch (m) {
    case NEVE: {
        iirAmount = 0.005832;
        threshold = 0.33362176;
        cutoff = 28811.0;
        break;
    }
    case API: {
        iirAmount = 0.004096;
        threshold = 0.59969536;
        cutoff = 27216.0;
        break;
    }

    case SSL:
    {
        iirAmount = 0.004913;
        threshold = 0.84934656;
        cutoff = 23011.0;
        break;
    }

    case TEAC:
    {
        iirAmount = 0.009216;
        threshold = 0.149;
        cutoff = 18544.0;
        break;
    }

    case MACKIE:
    {
        iirAmount = 0.011449;
        threshold = 0.092;
        cutoff = 19748.0;
        break;
    }
    default:
    {
        break;
    }
    }
}

void ChannelStripProcessor::setParam(PARAMETER p, float v)
{
    if (paramBindings.find(p) != paramBindings.end())
    {
        *paramBindings[p] = v;
    }
}


void ChannelStripProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sr = sampleRate;
}

void ChannelStripProcessor::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto rpL = bufferToFill.buffer->getReadPointer(0);
    auto rpR = bufferToFill.buffer->getReadPointer(1);
    auto wpL = bufferToFill.buffer->getWritePointer(0);
    auto wpR = bufferToFill.buffer->getWritePointer(1);
    double overallScale = 1.0;
    overallScale /= 44100;
    overallScale *= sr;
    double localiirAmount = iirAmount / overallScale;
    double localThreshold = threshold;
    double density = B * 2.0;
    double phattity = density - 1.0;
    density = density > 1.0 ? 1.0 : density;
    phattity = phattity < 0.0 ? 0.0 : phattity;
    double nonLin = 5.0 - density;
    biquadB[0] = biquadA[0] = cutoff / sr;
    biquadA[1] = 1.618033988749894848204586;
    biquadB[1] = 0.618033988749894848204586;
    double K = std::tan(juce::MathConstants<double>::pi * biquadA[0]);
    double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
    biquadA[2] = K * K * norm;
    biquadA[3] = 2.0 * biquadA[2];
    biquadA[4] = biquadA[2];
    biquadA[5] = 2.0 * (K * K - 1.0) * norm;
    biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;

    K = std::tan(juce::MathConstants<double>::pi * biquadA[0]);
    norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
    biquadB[2] = K * K * norm;
    biquadB[3] = 2.0 * biquadB[2];
    biquadB[4] = biquadB[2];
    biquadB[5] = 2.0 * (K * K - 1.0) * norm;
    biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        long double ipL = *rpL;
        long double ipR = *rpR;
        if (std::fabs(ipL) < 1.18e-37) ipL = fpdL * 1.18e-37;
        if (std::fabs(ipR) < 1.18e-37) ipR = fpdR * 1.18e-37;
        long double tempSample;
        if (biquadA[0] < 0.49999) {
            tempSample = biquadA[2] * ipL + biquadA[3] * biquadA[7] + biquadA[4] * biquadA[8] - biquadA[5] * biquadA[9] - biquadA[6] * biquadA[10];
            biquadA[8] = biquadA[7]; biquadA[7] = ipL; ipL = tempSample;
            biquadA[10] = biquadA[9]; biquadA[9] = ipL; //DF1 left
            tempSample = biquadA[2] * ipR + biquadA[3] * biquadA[11] + biquadA[4] * biquadA[12] - biquadA[5] * biquadA[13] - biquadA[6] * biquadA[14];
            biquadA[12] = biquadA[11]; biquadA[11] = ipR; ipR = tempSample;
            biquadA[14] = biquadA[13]; biquadA[13] = ipR; //DF1 right
        }
        double dielectricScaleL = std::fabs(2.0 - ((ipL + nonLin) / nonLin));
        double dielectricScaleR = std::fabs(2.0 - ((ipR + nonLin) / nonLin));
        if (flip)
        {
            iirSampleLA = (iirSampleLA * (1.0 - (localiirAmount * dielectricScaleL))) + (ipL * localiirAmount * dielectricScaleL);
            ipL = ipL - iirSampleLA;
            iirSampleRA = (iirSampleRA * (1.0 - (localiirAmount * dielectricScaleR))) + (ipR * localiirAmount * dielectricScaleR);
            ipR = ipR - iirSampleRA;
        }
        else
        {
            iirSampleLB = (iirSampleLB * (1.0 - (localiirAmount * dielectricScaleL))) + (ipL * localiirAmount * dielectricScaleL);
            ipL = ipL - iirSampleLB;
            iirSampleRB = (iirSampleRB * (1.0 - (localiirAmount * dielectricScaleR))) + (ipR * localiirAmount * dielectricScaleR);
            ipR = ipR - iirSampleRB;
        }
        // Left
        long double drySampleL = ipL;
        long double drySampleR = ipR;
        ipL = ipL > 1.0 ? 1.0 : ipL;
        ipL = ipL < -1.0 ? -1.0 : ipL;
        long double phatSampleL = std::sin(ipL * 1.57079633);
        ipL *= 1.2533141373155;
        long double distSampleL = std::sin(ipL * std::fabs(ipL)) / ((std::fabs(ipL) == 0) ? 1 : std::fabs(ipL));
        ipL = distSampleL;
        if (density < 1.0) ipL = (drySampleL * (1 - density)) + (distSampleL * density);
        if (phattity > 0.0) ipL = (ipL * (1 - phattity)) + (phatSampleL * phattity);
        // Right 

        ipR = ipR > 1.0 ? 1.0 : ipR;
        ipR = ipR < -1.0 ? -1.0 : ipR;

        long double phatSampleR = std::sin(ipR * 1.57079633);
        ipR *= 1.2533141373155;
        long double distSampleR = sin(ipR * fabs(ipR)) / ((fabs(ipR) == 0.0) ? 1 : fabs(ipR));
        ipR = distSampleR;
        if (density < 1.0) ipR = (drySampleR * (1 - density)) + (distSampleR * density); //fade Spiral aspect
        if (phattity > 0.0) ipR = (ipR * (1 - phattity)) + (phatSampleR * phattity); //apply original Density on top

        // L: 
        double clamp = (lastSampleBL - lastSampleCL) * 0.381966011250105;
        clamp -= (lastSampleAL - lastSampleBL) * 0.6180339887498948482045;
        clamp += ipL - lastSampleAL;

        lastSampleCL = lastSampleBL;
        lastSampleBL = lastSampleAL;
        lastSampleAL = ipL;
        if (clamp > localThreshold) ipL = lastSampleBL + localThreshold;
        if (-clamp > localThreshold) ipL = lastSampleBL - localThreshold;
        lastSampleAL = (lastSampleAL * 0.381966011250105) + (ipL * 0.6180339887498948482045);

        // R:
        clamp = (lastSampleBR - lastSampleCR) * 0.381966011250105;
        clamp -= (lastSampleAR - lastSampleBR) * 0.6180339887498948482045;
        clamp += ipR - lastSampleAR;

        lastSampleCR = lastSampleBR;
        lastSampleBR = lastSampleAR;
        lastSampleAR = ipR;

        if (clamp > localThreshold) ipR = lastSampleBR + localThreshold;
        if (-clamp > localThreshold) ipR = lastSampleBR - localThreshold;
        lastSampleAR = (lastSampleAR * 0.381966011250105) + (ipR * 0.6180339887498948482045);

        flip = !flip;
        if (C < 1.0) {
            ipL *= C;
            ipR *= C;
        }
        if (biquadB[0] < 0.49999) {
            tempSample = biquadB[2] * ipL + biquadB[3] * biquadB[7] + biquadB[4] * biquadB[8] - biquadB[5] * biquadB[9] - biquadB[6] * biquadB[10];
            biquadB[8] = biquadB[7]; biquadB[7] = ipL; ipL = tempSample;
            biquadB[10] = biquadB[9]; biquadB[9] = ipL; //DF1 left
            tempSample = biquadB[2] * ipR + biquadB[3] * biquadB[11] + biquadB[4] * biquadB[12] - biquadB[5] * biquadB[13] - biquadB[6] * biquadB[14];
            biquadB[12] = biquadB[11]; biquadB[11] = ipR; ipR = tempSample;
            biquadB[14] = biquadB[13]; biquadB[13] = ipR; //DF1 right
        }

        int expon; frexpf((float)ipL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        ipL += ((double(fpdL) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
        frexpf((float)ipR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        ipR += ((double(fpdR) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
        *wpL = ipL;
        *wpR = ipR;
        ++rpL;
        ++rpR;
        ++wpL;
        ++wpR;
    }
}

void ChannelStripProcessor::releaseResources()
{

}
