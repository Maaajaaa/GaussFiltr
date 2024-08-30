# 1D Gaussian filter library for Arduino

Implementation of a 1D [Gaussian Filter](https://en.wikipedia.org/wiki/Gaussian_Filter).

Features a cached mode for faster re-runs of the filter, provided the sigma stays the same. This will however go at the expense of a few extra bytes of RAM (around 8-12*32bit + array overhead for most sigma values).

```
#include <GaussFiltr.h>

#define cachedMode false

#define SIGMA 0.2

float floatArray[10] = {0, 1,2,3,4,5,6,7,8,9};

GaussianFilter1D filter = GaussianFilter1D(cachedMode);

filter.begin(SIGMA);

filter.filter(floatArray);

//floatArray is now filtered

```
more implementation details in the 