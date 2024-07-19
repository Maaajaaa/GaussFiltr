#include "GaussianFilter1D.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
// GaussianFilter1D.cpp
//
//  Arduino library to run an effiecient Gaussian 1D blur filter. Possible usecases are LED Strips
//
//
//
//  Based on:
//  ---------
//  Gaussian_filter_1D_cpp by lchop, from https://github.com/lchop/Gaussian_filter_1D_cpp
//
//  created: 07/2023
//
//
//  MIT License
//
//  Copyright (c) 2023 Maja Männel
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//  History:
//
//  20230716 Created by maaajaaa
//
//  ToDo:
//  - limit kernel space
//
///////////////////////////////////////////////////////////////////////////////////////////////////
GaussianFilter1D::GaussianFilter1D(bool cachedMode)
{
    this->cachedMode = cachedMode;
    this->lastArrayLength = 0;
}

void GaussianFilter1D::begin(float sigma, int futureArrayLength)
{
    if(this->cachedMode && futureArrayLength != 0){
       kernelCache = new float[futureArrayLength];
       computeKernelCache(this.kernelCache, futureArrayLength, sigma);
    }
}

void GaussianFilter1D::filter(float data[], int data_length)
{
    if(!cachedMode){
        float tempKernelCache[data_length];
        computeKernelCache(&tempKernelCache, data_length, this->sigma);
    }else{
        //cached mode
        //check if there's a mismatch between the saved and the needed kernel
        if(data_length != this->lastArrayLength){
            delete [] kernelCache;
            kernelCache = new float[futureArrayLength];
            computeKernelCache(this.kernelCache, futureArrayLength, sigma);
        }
        float output[data_length];
        for(int i = 0; i < data_length; i++){
            //calculate each value
            output[i] = makeAndApplyKernelFromKernelCache(this->kernelCache, data_length, i, data);
        }
        //save output to data
        for(int i = 0; i < data_length; i++){
            data[i] = output[i];
        }
    }
}

void GaussianFilter1D::end()
{
    delete [] kernelCache;
}

void GaussianFilter1D::computeKernelCache(float kernelCache[], int n_points, float sigma)
{
    //Compute the kernel for the given x point
    //calculate sigma² once to speed up calculation
    float twoSigmaSquared = (2 * pow(sigma, 2));
    for (int i = 0; i < n_points * 2; i++) {
        //Compute gaussian kernel
        //kernel cache at 0 is -1*n_points
        kernelCache[i] = exp(-(pow(-1 * n_points + i, 2) / twoSigmaSquared));
    }
    if(this->cachedMode){
        this->lastArrayLength = n_points;
        this->sigma = sigma;
    }
    return;
}

float GaussianFilter1D::makeAndApplyKernelFromKernelCache(float kernelCache[], int n_points, int x_position, float y_values[])
{
    //make array for the actual kernel for the given x point
    float kernel[n_points] = {};
    float sum_kernel = 0;
    for (int i = 0; i < n_points; i++) {
        //fetch kernel vale from kernel cache
        //+ n_points as -npoints is at 0
        kernel[i] = kernelCache[i - x_position + n_points];
        //compute a weight for each kernel position
        sum_kernel += kernel[i];
    }
    //apply weight to each kernel position to give more important value to the x that are around ower x
    for (int i = 0; i < n_points; i++)
        kernel[i] = kernel[i] / sum_kernel;
    return applyKernel(n_points, x_position, kernel, y_values);
}

float GaussianFilter1D::applyKernel(int n_points, float kernel[], float y_values[])
{
    float y_filtered = 0;
    //apply filter to all the y values with the weighted kernel
    for (int i = 0; i < n_points; i++)
        y_filtered += kernel[i] * y_values[i];

    return y_filtered;
}
