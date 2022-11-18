#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

float **createDataMatrix(int numElement, int numSample)
{
    float **RFData;
    RFData = new float* [numElement];

    for (int i=0; i<numElement; i++)
    {
        RFData[i] = new float[numSample];
    } 

    return RFData;
    // Create a 2D data matrix of size numElement and numSample nnknknkn
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{
    ifstream infile(fileName);
    char line[100];
    float numbers[numSample];



    if (fileName.fail())
	{
        return -1;
	}

    for(int x=0; x<numElement; x++)
    {
        for(int i=0; i<numSample; i++)
        {
            infile.getline(line, 100);
            numbers[i]=atof(line);
        }

        RFData[x]=numbers;
    
    }

    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    int depth;

    cout << "What is the desired imaging depth?";
    cin >> depth;
    
    cout << "How many pixels is the scanline?"
    cin >> numPixel;

    float scanlineLocation[numPixel];

    float increment = (numPixel-1)/depth;
    float newDepth=0;
    
    for(int i=0; i<numPixel; i++)

    {
        scanlineLocation[i]=newDepth;
        newDepth+=increment;
    }


    return scanlineLocation;

}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH)
{
    float eleLocation[128];
   
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel)
{
    
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{
    
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
    
}