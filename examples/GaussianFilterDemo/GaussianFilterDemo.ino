#include <GaussFiltr.h>

#define cachedMode false

#define SIGMA 0.2

#define arrayLength 10


GaussianFilter1D filter = GaussianFilter1D(cachedMode);

void setup(){
  filter.begin(SIGMA);

  Serial.begin(115200);
  while(!Serial);
  Serial.println("\n\nGaussian Filter Example");


  Serial.println("very pseudo-random input array, might be nearly identical on every run, as time is not set:");

  float dataArray[arrayLength] = {};

  for(int i = 0; i < arrayLength; i++){
    dataArray[i] = rand() / 10000.0; 
    Serial.print(dataArray[i]);
    Serial.print(" ");
  }

  filter.filter(dataArray, arrayLength);

  Serial.println("\nsmoothed values:");
  for(int i = 0; i < arrayLength; i++){
    Serial.print(dataArray[i]);
    Serial.print(" ");
  }

  Serial.println("demo finished");

  //free memory, especially important for cached mode
  filter.end();

}

void loop(){

}