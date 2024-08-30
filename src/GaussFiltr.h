///////////////////////////////////////////////////////////////////////////////////////////////////
//GaussianFilter1D.h
//
// Arduino library to run an efficient Gaussian 1D blur filter. Possible usecases are LED Strips
//
// 
//
// Based on:
// ---------
// Gaussian_filter_1D_cpp by lchop, from https://github.com/lchop/Gaussian_filter_1D_cpp
//
// created: 07/2023
//
//
// MIT License
//
// Copyright (c) 2023 Maja Männel
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// History:
//
// 20230716 Created by maaajaaa
// 20230729 Added Kernel radius, which reduces memory consumption by a lot and increases speed too
//
// ToDo: 
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GaussianFilter1D_h
#define GaussianFilter1D_h

#include <Arduino.h>
#include <array>
#include <math.h>

class GaussianFilter1D {
    public:

    /**
     * @brief Constructor.
     * @param cachedMode Cached mode speeds up for repeated calculations using the same sigma paramters, at the expense of slightly increased memory consumption
    */
    GaussianFilter1D(bool cachedMode);

   /**
    * @brief Initialization for cached mode, if plain initialisation is used, caching will be performed at first run of filter()
    * @param[in]  sigma Sigma, standard deviation, paramter of gaussian distribution, typically between 0.1 and 
    * @param[in]  epsilon Epsilon, desired minimum precision, used to calculate kernel window size
    * 
    */
    void begin(float sigma, float epsilon = 0.01);

    /**
    * @brief apply the filter on the given array
    * @param[in] data array
    * @param[in] data_length number of elements in array  
    */
    void filter(float data[], int data_length);

    /**
    * @brief end the filter and clear allocated memory
    */    
    void end();

    #ifdef GAUSSIAN_FILTER_DEBUG
    String getSemiKernel();
    #endif

   private:

    /**
     * @brief      Compute Kernel cache, this is used to later create kernels without needing to run the expensive exp and pow functions repeatedly
     *
     * @param[in]  sigma Sigma, standard deviation, paramter of gaussian distribution
     *
     * @return     { description_of_the_return_value }
     */
    void computeSemiKernelCache(float kernelCache[], float sigma);


    /**
     * @brief      apply the cached semi-kernel to one position of the array
     *
     * @param[in]  kernelCache the previsously chached kernels
     * @param[in]  n_points  The length of the future input array
     * @param[in]  x_position postition of the y values which will be smoothed
     * @param[in]  y_values[] the input array which will be filtered
     *
     * @return     smoothed y value at x_position
     */
    float applyFilterFromSemiKernelCache(float kernelCache[], int n_points, int x_position, float y_values[]);

    protected:

    bool cachedMode;
    float* kernelCache;
    float kernelSum;
    float sigma;
    int kernelSize;
};

#endif