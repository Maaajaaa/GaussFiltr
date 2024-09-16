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
//  20230729 Added Kernel radius, which reduces memory consumption by a lot and increases speed too
//
//  ToDo:
//
///////////////////////////////////////////////////////////////////////////////////////////////////
GaussianFilter1D::GaussianFilter1D(bool cachedMode)
{
    this->cachedMode = cachedMode;
    this->kernelSize = 0;
}

void GaussianFilter1D::begin(float sigma, float epsilon)
{
    //calculate kernel radius, based on: https://stackoverflow.com/a/68050503
    int kernelRadius = ceil(sqrt(-2*sigma*sigma*log(epsilon*0.5*sqrt(TWO_PI))));
    //add one more for the centre kernel value
    this->kernelSize = kernelRadius + 1;
    this->sigma = sigma;

    if(this->cachedMode && this->kernelSize != 0){
       kernelCache = new float[this->kernelSize];
       computeSemiKernelCache(this->kernelCache, sigma);
    }
}

void GaussianFilter1D::filter(float data[], int data_length)
{
    float output[data_length];
    //non-cached mode
    if(!cachedMode){
        delete [] kernelCache;
        kernelCache = new float[kernelSize];
        computeSemiKernelCache(this->kernelCache, this->sigma);
    }

    for(int i = 0; i < data_length; i++){
        //calculate each value
        output[i] = applyFilterFromSemiKernelCache(this->kernelCache, data_length, i, data);
    }

    //free the memory again in non-cached mode
    if(!cachedMode){
        delete [] kernelCache;
    }

    //save output to data
    for(int i = 0; i < data_length; i++){
        data[i] = output[i];
    }
    return;
}

void GaussianFilter1D::end()
{
    delete [] kernelCache;
}

#ifdef GAUSSIAN_FILTER_DEBUG

String GaussianFilter1D::getSemiKernel()
{
    if(cachedMode){
        String str;
        for (int i = 0; i < this->kernelSize; i++)
        {
            /* code */
            str += String(this->kernelCache[i], 9);
            str += String(", ");        
        }
        str += String(" ; ");
        str += String(this->kernelSize);
        return str;
    }else{
        return String("no kernel cache is stored in non-cached mode");
    }
}

#endif

void GaussianFilter1D::computeSemiKernelCache(float kernelCache[], float sigma)
{
    //Compute the kernel for the given x point
    //calculate sigma² once to speed up calculation
    const float twoSigmaSquared = (2 * pow(sigma, 2));

    //we don't need to calculate position 0 as it is 1.0
    kernelCache[0] = 1.0;
    //add the skipped middle point of the kernel to the sum
    this->kernelSum += 1.0;

    for (int i = 1; i < this->kernelSize; i++) {
        //Compute half of the gaussian kernel (it's symmetric and 1.0 in the centre)
        kernelCache[i] = exp(-(pow(i, 2) / twoSigmaSquared));
        this->kernelSum +=  kernelCache[i];
    }
    //apply weight to the kernel
    for (int i = 0; i < kernelSize; i++){
        kernelCache[i] = kernelCache[i] / kernelSum;
    }
    return;
}

float GaussianFilter1D::applyFilterFromSemiKernelCache(float kernelCache[], int n_points, int x_position, float y_values[])
{
    //don't move past  the boundaries of the array
    float y_filtered = 0.0;
    //add the radius to the left side
    int filterBegin = x_position - (kernelSize - 1);
    if(filterBegin < 0){
        filterBegin = 0;
    }

    for(int i = filterBegin; i<x_position; i++){
        //reading the kernel backwards
        y_filtered += y_values[i] * kernelCache[kernelSize-(i-filterBegin)];
    }

    //add the radius to the left side
    int filterEnd = x_position + (kernelSize - 1);
    //don't move past  the boundaries of the array
    if(filterEnd > n_points){
        filterEnd = n_points;
    }

    for(int i = x_position; i<filterEnd; i++){
        //reading the kernel forward
        y_filtered += y_values[i] * kernelCache[i-x_position];
    }

    #ifdef GAUSSIAN_FILTER_DEBUG

    Serial.print("x: ");
    Serial.print(x_position);
    Serial.print("\tbegin: ");
    Serial.print(filterBegin);
    Serial.print("\tend: ");
    Serial.print(filterEnd);
    Serial.print("\tvalue: ");
    Serial.print(y_values[x_position]);
    Serial.print("\tresult: ");
    Serial.println(y_filtered);

    #endif

    return y_filtered;
}
