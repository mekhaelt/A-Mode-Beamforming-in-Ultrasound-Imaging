#include <iostream>
#include "amode.h"

using namespace std;


int main () {

    //Initializes the constant values
    const int N = 128; //Number of channels 
    const int K = 3338; //Number of Samples
    const float PITCH = 3.048e-4; //The transducer pitch
    const float SOS = 1540; //Speed of sound variable
    const float FS = 40e6; // Sampling frequency variable
    const float TS = 1.0f/FS; //Sampling time variable

    //Create the array of pointers, and the arrays the pointers point to.
    float **realRFData = createDataMatrix(N, K);
    float **imagRFData = createDataMatrix(N, K);

    int result = 0;

    //Inputs the data from the txt files. 
    result = loadRFData(realRFData, "RealRFData_mystery.txt", N, K);
    if (result == -1)
    {
        cerr << "Cannot load from realRFData.txt, exiting program" << endl;
        return -1;
    }
    result = loadRFData(imagRFData, "ImagRFData_mystery.txt",N, K);
    if (result == -1)
    {
        cerr << "Cannot load from imagRFData.txt, exiting program" << endl;
        return -1;
    }

    //Creates imaging location array here
    int numPixel = 256;
    float *scanlinePosition = genScanlineLocation(numPixel);

    //Creates element locations here
    float* elementPosition; 
    elementPosition = genElementLocation(N,PITCH);

    //Performs A-mode beamforming here
    float *scanline;
    scanline = createScanline(numPixel);
    beamform(scanline, realRFData, imagRFData, scanlinePosition, elementPosition, N, K, numPixel, FS, SOS);

    //Outputs data into a .csv file
    result = outputScanline("output.csv", scanlinePosition, scanline, numPixel);
    if (result == -1)
    {
        cerr << "Cannot write scanline to output.csv, exiting program" << endl;
        return -3;
    }

    destroyAllArrays(scanline, realRFData, imagRFData, scanlinePosition, elementPosition, N, K, numPixel);

    return 0;
}