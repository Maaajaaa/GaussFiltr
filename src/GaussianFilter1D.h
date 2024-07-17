///////////////////////////////////////////////////////////////////////////////////////////////////
//GaussianFilter1D.h
//
// Arduino library to run an effiecient Gaussian 1D blur filter. Possible usecases are LED Strips
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
//
// ToDo: 
// - limit kernel space
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GaussianFilter1D_h
#define GaussianFilter1D_h

#include <Arduino.h>

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
    * @param futureArrayLength Length that the 
    * 
    */
   void begin(float sigma, int futureArrayLength);

   /**
    * @brief Initialization for non-cached mode, if caching is enabled, caching will be performed at first run of filter()
    */
   void begin();

   /**
    * @brief apply the filter on the given array     
    */
   void filter(float data[], int data_length);

   private:

    /**
     * @brief      Compute Kernel cache, this is used to later create kernels without needing to run the expensive exp and pow functions repeatedly
     *
     * @param[in]  n_points  The length of the future input array
     * @param[in]  sigma Sigma, standard deviation, paramter of gaussian distribution
     *
     * @return     { description_of_the_return_value }
     */
    void computeKernelCache(float kernelCache[], int n_points, float sigma);


    /**
     * @brief      make the actual kernel from the cached kernel and normalize it, for the current position, then apply the kernel (see below)
     *
     * @param[in]  kernelCache the previsously chached kernels
     * @param[in]  n_points  The length of the future input array
     * @param[in]  x_position postition of the y values that will be smoothed
     * @param[in]  y_values[] the input array that will be filtered
     *
     * @return     smoothed y value at x_position
     */
    float makeAndApplyKernelFromKernelCache(float kernelCache[], int n_points, int x_position, float y_values[]);

    /**
     * @brief      apply the kernel for the element at x_position and return it
     *
     * @param[in]  n_points  The length of the future input array
     * @param[in]  x_position postition of the y values that will be smoothed 
     * @param[in]  kernel[] the gaussian
     * @param[in]  y_values[] the input array that will be filtered
     *
     * @return     smoothed y value at x_position
     */
    float applyKernel(int n_points, int x_position, float kernel[], float y_values[]);

    protected:

    bool cachedMode;
    std::array<float> kernelCache;
    float sigma;
    int lastArrayLength;
};

#endif