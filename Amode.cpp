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
    // Create a 2D data matrix of size numElement and numSample 

    float **RFData;  //defines double pointer variable
    RFData = new float* [numElement];  //allocate an array of pointer float variables

    for (int i=0; i<numElement; i++)
    {
        RFData[i] = new float[numSample]; //for each pointer in the array of pointers allocate an array of float variables
    } 


    return RFData; //return array
    
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{

    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files

    //open file
    ifstream infile; 
    infile.open(fileName);


    char line[100]; //char array to store input
    float numbers[numSample]; //float array to store input once the char arrays have been converted to floats


    //returns -1, if function fails to open the designated file
    if (!infile.good())
	{
        return -1;
	}

    for(int x=0; x<numElement; x++)
    {
        for(int i=0; i<numSample; i++)
        {
            infile.getline(line, 100); //recieves input from the file and stores it within char array
            RFData[x][i]=atof(line); //converts char array to a float and stores the value within RFData
        }
    
    }

    infile.close(); //closes file
    return 0;

}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    float depth; //initialize variable

    cout << "What is the desired imaging depth?";
    cin >> depth; //receives input for depth
    
    cout << "How many pixels is the scanline?";
    cin >> numPixel; //receives input for pixels

    float *scanlineLocation; //defines pointer float
    scanlineLocation = new float[numPixel]; //creates float array

    float newDepth=0; //initalizes and declares newDepth
    
    for(int i=0; i<numPixel; i++)

    {
        float increment = i*(depth/(numPixel-1)); //calculates change in each index for scanlineLocation
        scanlineLocation[i]=increment; //assigns value to scanlineLocation
    }


    return scanlineLocation;

}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH)
{
    float *eleLocation; //defines pointer float
    eleLocation = new float [numElement]; //allocates an array of floats

    for (int n=0; n<numElement; n++)
    {
        eleLocation[n] = (n-((numElement-1.0)/2.0))*PITCH; //calculation for eleLocation
    }

    return eleLocation;
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel)
{
    float *scanline;    //creates pointer variable for scanline
    scanline = new float[numPixel]; //allocates an array of floats
    return scanline;
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    //create arrays
    float tForward[numPixel]; 
    float tBackward[numPixel][numElement];
    float tTotal[numPixel][numElement];
    int s[numPixel][numElement]; 
    float pReal[numPixel];   
    float pImag[numPixel]; 
    


    for (int i = 0; i<numPixel; i++)
    {   
        //declares values of 0
        pReal[i] = 0; 
        pImag[i] = 0;

        tForward[i] = scanlinePosition[i]/SoS;  //calculations for tForward
        
        for (int k = 0; k<numElement; k++)
        {

            tBackward[i][k] = (sqrt(pow(scanlinePosition[i], 2) + pow(elementPosition[k], 2)))/SoS; //calculations for tBackward
            tTotal[i][k] = tForward[i] + tBackward[i][k]; //calculations for tTotal
            s[i][k]=floor(tTotal[i][k]*FS);  //calculations for s
            pReal[i] += realRFData[k][s[i][k]]; //calculations for pReal
            pImag[i] += imagRFData[k][s[i][k]]; //calculations for pImag
        }
        scanline[i] = sqrt(pow(pReal[i], 2) + pow(pImag[i], 2)); //calculations for echo magnitude
    }


    
    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{       
    //open outfile
    ofstream outfile;
    outfile.open(fileName);
    
    //returns -1, if function fails to open the designated file
    if (outfile.fail()) 
    {
        return -1;
    }

    //outputs scanlinePosition & scanline
    for(int i = 0; i<numPixel; i++)
    {
        outfile << scanlinePosition[i];
        outfile << ", ";
        outfile << scanline[i];
        outfile << endl;
    }

    outfile.close(); //closes file

    return 0;
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
    //deletes memory for 1-dimension arrays
    delete scanline;
    delete scanlinePosition;
    delete elementPosition;

    //deletes memory for 2-dimensional arrays
    for( int i=0; i<numElement; i++)
    {
        delete realRFData[i];
        delete imagRFData[i];
    }
}