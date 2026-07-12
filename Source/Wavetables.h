/*
  ==============================================================================

    Wavetables.h
    Created: 22 Jan 2026 3:08:39pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <vector>
#include <cmath>

const float PI = std::atanf(1.0f) * 4.0f;
const float PI_OVER_4 = std::atanf(1.0f);

// This struct creates a 2D vector of wavetables that the WavetableOsc will cycle through

class Wavetables{

public:

    //Create a 3D vector IxMxN, N->samples in each wavetable, M->wavetable shape, I->amount of harmonics to avoid aliasing
    std::vector<std::vector<std::vector<float>>> generateAllWavetables(float sampleRate_, int M_, int N_, float interval, float minFreq, float maxFreq, int tableSet)
    {
        sampleRate = sampleRate_;
        M = M_;
        N = N_;

        neededWavetables = static_cast<size_t>(M);

        std::vector<std::vector<std::vector<float>>> allWaveTables;
        allWaveTables.clear();
        float multiplier = std::powf(2.0f, interval / 12.0f);

        pitch = minFreq;
        float freqLimit = maxFreq * multiplier;

        while (pitch <= freqLimit) {
            if (tableSet == 1) {
                allWaveTables.push_back(generateWavetablesFromNodes(generateNodeWavetables_1()));
            }
            if (tableSet == 2) {
                allWaveTables.push_back(generateWavetablesFromNodes(generateNodeWavetables_2()));
            }
            if (tableSet == 3) {
                allWaveTables.push_back(generateWavetablesFromNodes(generateNodeWavetables_3()));
            }
            pitch *= multiplier;
        }
        return allWaveTables;
    }

private:

    int M; // number of wavetables to morph through
    int N; //number of samples for each wavetable
    size_t neededWavetables;
    float sampleRate;
    float pitch; //frequency set to calculate the max number of harmonics in order to avoid aliasing


    std::vector<std::vector<float>> generateWavetablesFromNodes(std::vector<std::vector<float>> nodeWavetables)
    {
        int L = nodeWavetables.size();
        std::vector<std::vector<float>> waveTablesFromNodes;
        waveTablesFromNodes.clear();
        waveTablesFromNodes.reserve(neededWavetables);

        int batchSize = (M - L) / (L - 1);
        int batchSizeRemainder = (M - L) % (L - 1);

        // for each batch
        for (int i = 0; i < (L - 1); ++i)
        {
            const auto startTable = nodeWavetables[i];
            const auto endTable = nodeWavetables[i + 1]; 
            waveTablesFromNodes.push_back(startTable); //add the starting table

            if (batchSizeRemainder > 0 && i == (L - 2)) 
            {
                //takes care of edge case where batchSizeRemainder is not zero
                //makes sure that there are exactly M wavetables inside waveTablesFromNodes
                batchSize = batchSize + batchSizeRemainder;
            }

            for (auto m = 1; m <= batchSize; ++m) { 
                std::vector<float> mTable(N); // create an N size wavetable of zeroes
                for (int n = 0; n < N; ++n) { // fill this wave table with the appropriate values
                    float sampleInc = (endTable[n] - startTable[n]) * (static_cast<float>(m) / static_cast<float>(batchSize + 1));
                    mTable[n] = startTable[n] + sampleInc;
                }
                waveTablesFromNodes.push_back(mTable); // add the m-th generated wavetable
            }
        }
        waveTablesFromNodes.push_back(nodeWavetables[L - 1]); //add the last node wavtetable table
        return waveTablesFromNodes;
    }

    //std::vector<std::vector<float>> generateWavetables() //create a 2D vector including all the "in-between" wavetables
    //{
    //    std::vector<std::vector<float>> waveTables;
    //    waveTables.clear();
    //
    //    const auto startTable = generateSineWavetable();
    //    const auto endTable = generateSawWavetable();
    //    // Iterates through M first
    //    waveTables.push_back(startTable); //add the starting table
    //    for (auto m = 1; m < M - 1; ++m) {
    //        std::vector<float> mTable(N); // create an N size wavetable of zeroes
    //        for (int n = 0; n < N; ++n) { // fill this wave table with the appropriate values
    //            float sampleInc = (endTable[n] - startTable[n]) * (static_cast<float>(m) / static_cast<float>(M));
    //            mTable[n] = startTable[n] + sampleInc;
    //        }
    //        waveTables.push_back(mTable); // add the wavetable
    //    }
    //    waveTables.push_back(endTable); //add the ending table
    //    return waveTables;
    //
        //// Iterating through N first
        //std::vector<std::vector<float>> waveTables(M - 2, std::vector<float>(N));
        //waveTables.insert(waveTables.begin(), startTable); //add startTable to start
        //waveTables.push_back(endTable); //add endTable to end
    //
        //for (int n = 0; n < N; ++n) { //each sample position
        //    float sampleInc = (waveTables[M - 1][n] - waveTables[0][n]) / static_cast<float>(M);
        //    for (int m = 1; m < M - 1; ++m) { //for each waveTable between start and end
        //        waveTables[m][n] = waveTables[m - 1][n] + sampleInc; //increment
        //    }
        //}
        ////print2DVector(waveTables);
        //return waveTables;
    //}
    //void print2DVector(const std::vector<std::vector<float>>& matrix) {
    //    // Iterate through each row
    //    for (const auto& row : matrix) {
    //        // Iterate through each element in the row
    //        for (const auto& element : row) {
    //            DBG(element << " ");
    //        }
    //        // New line after each row
    //        DBG("\n");
    //    }
    //}

    std::vector<std::vector<float >> generateNodeWavetables_1()
    {
        std::vector<std::vector<float>> nodeWaveTables;
        nodeWaveTables.clear();
        nodeWaveTables.push_back(generateSquareWavetable());
        nodeWaveTables.push_back(generateTriWavetable());
        nodeWaveTables.push_back(generateSawWavetable());
        return nodeWaveTables;
    }

    std::vector<std::vector<float >> generateNodeWavetables_2()
    {
        std::vector<std::vector<float>> nodeWaveTables;
        nodeWaveTables.clear();
        nodeWaveTables.push_back(generateHwrSineWavetable());
        nodeWaveTables.push_back(generateSquareWavetable());
        nodeWaveTables.push_back(generateTriWavetable());
        nodeWaveTables.push_back(generateSawWavetable());
        return nodeWaveTables;
    }

    std::vector<std::vector<float >> generateNodeWavetables_3()
    {
        std::vector<std::vector<float>> nodeWaveTables;
        nodeWaveTables.clear();
        nodeWaveTables.push_back(generateSineWavetable());
        nodeWaveTables.push_back(generateSquareWavetable());
        nodeWaveTables.push_back(generateTriWavetable());
        nodeWaveTables.push_back(generateSawWavetable());
        return nodeWaveTables;
    }

    std::vector<float> generateSineWavetable()
    {
        std::vector<float> sineWavetable(N);
        for (auto i = 0; i < N; ++i) {
            sineWavetable[i] = std::sinf(2.0f * PI * 
                static_cast<float>(i) / static_cast<float>(N));
        }
        //normalizeWavetable(sineWavetable);
        return sineWavetable;
    }

    std::vector<float> generateHwrSineWavetable() //Half wave rectified
    {
        std::vector<float> HwrSineWavetable(N);
        for (auto i = 0; i < N; ++i) {
            float s = std::sinf(2.0f * PI * static_cast<float>(i) / static_cast<float>(N));
            HwrSineWavetable[i] = 0.5f * (std::abs(s) + s);
        }
        //normalizeWavetable(HwrSineWavetable);
        return HwrSineWavetable;
    }

    std::vector<float> generateSawWavetable()
    {
        std::vector<float> sawWavetable(N);
        int Harmonics = sampleRate / (2 * pitch);
        //DBG("harmonics = " << Harmonics);
        for (auto i = 0; i < N; ++i) { // for every sample
            sawWavetable[i] = 0;
            for (auto j = 1; j <= Harmonics; ++j) { //sum harmonics
                // Lanczos sigma to smooth the wave
                float sigma = PI * static_cast<float>(j) / static_cast<float>(Harmonics);
                float sigma2 = (std::sinf(sigma) / sigma);
                sawWavetable[i] +=
                    std::pow(-1.0f, static_cast<float>(j))
                    * (1.0f / static_cast<float>(j))
                    * std::sinf(static_cast<float>(j)*(-2.0f * PI * static_cast<float>(i) / static_cast<float>(N)))
                    * sigma2;
            }
        }
        normalizeWavetable(sawWavetable);
        return sawWavetable;
    }

    std::vector<float> generateSquareWavetable()
    {
        std::vector<float> squareWavetable(N);
        int Harmonics = ((sampleRate / (2 * pitch)) - 1) / 2;
        //DBG("harmonics = " << Harmonics);
        for (auto i = 0; i < N; ++i) { // for every sample
            squareWavetable[i] = 0;
            for (auto j = 0; j <= Harmonics; ++j) { //sum harmonics
                // Lanczos sigma to smooth the wave
                float sigma = (2 * static_cast<float>(j) + 1.0f) / (2 * static_cast<float>(Harmonics) + 1.0f);
                float sigma2 = (std::sinf(PI * sigma) / (PI * sigma));
                squareWavetable[i] +=
                    (1.0f / (2.0f * static_cast<float>(j) + 1.0f))
                    * std::sinf(((2.0f * static_cast<float>(j)) + 1.0f) * (-2.0f * PI * static_cast<float>(i) / static_cast<float>(N)))
                    * std::powf(sigma2, 2.0f);
            }
        }
        normalizeWavetable(squareWavetable);
        return squareWavetable;
    }

    std::vector<float> generateTriWavetable()
    {
        std::vector<float> triWavetable(N);
        int Harmonics = ((sampleRate / (2 * pitch)) - 1) / 2;
        for (auto i = 0; i < N; ++i) { // for every sample
            triWavetable[i] = 0;
            for (auto j = 0; j <= Harmonics; ++j) { //sum harmonics
                // Lanczos sigma to smooth the wave
                triWavetable[i] +=
                    std::pow(-1.0f, static_cast<float>(j))
                    * std::powf((1.0f / (2.0f * static_cast<float>(j) + 1.0f)), 2.0f)
                    * std::sinf(((2.0f * static_cast<float>(j)) + 1.0f) * (-2.0f * PI * static_cast<float>(i) / static_cast<float>(N)));
            }
        }
        normalizeWavetable(triWavetable);
        return triWavetable;
    }

    //generate noise
    // PW
    // PN
    //generate vowels
    //generate horn
    //generate flute
    //generate harmonica

    void normalizeWavetable(std::vector<float>& data) {
        if (data.empty()) return;

        float absMax = 0.0f;
        for (float& x : data) {
            if (absMax < std::abs(x)) {
                absMax = static_cast<float>(std::abs(x));
            }
        }

        if (absMax <= 10e-6 && absMax >= -10e-6) {
            return;
        }

        for (float& x : data) {
            x = x / absMax;
        }
    }
};