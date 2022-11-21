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

    float **RFData;  //initialize double pointer variable
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
    float numbers[numSample]; //float array to store input once they have been converted to float


    //returns -1, if function fails to open the designated file
    if (infile.fail())
	{
        return -1;
	}

    for(int x=0; x<numElement; x++)
    {
        for(int i=0; i<numSample; i++)
        {
            infile.getline(line, 100); //recieves input from the file and stores it within char array
            numbers[i]=atof(line); //converts char array to a float and stores the value within numbers
        }

        RFData[x]=numbers; //assigns numbers to RFData[x]
    
    }

    infile.close(); //closes file
    return 0;

}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    int depth; //initialize variable

    cout << "What is the desired imaging depth?";
    cin >> depth; //receives input for depth
    
    cout << "How many pixels is the scanline?";
    cin >> numPixel; //receives input for pixels

    float scanlineLocation[numPixel]; //creates array

    float increment = (numPixel-1)/depth; //calculates how much each value should increment by
    float newDepth=0; //initalizes and declares newDepth
    
    for(int i=0; i<numPixel; i++)

    {
        scanlineLocation[i]=newDepth; //assigns value of newDepth to scanlineLocation
        newDepth+=increment; //modifies value of newDepth
    }


    return scanlineLocation;

}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH)
{
    float eleLocation[numElement]; //creates float array

    for (int n=0; n<numElement; n++)
    {
        eleLocation[n] = (n-(numElement-1)/2)*PITCH; //calculation for eleLocation
        cout << eleLocation[n]; //outputs eleLocation[n]
    }
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
    //creates float arrays
    float tForward[numPixel]; 
    float tBackward[numPixel, numElement];
    float tTotal[numPixel, numElement];

    int **s; //creates douple pointer int
    s = new int*[numPixel]; //Allocate an array of pointer int variables

    float *pReal;   //initializes pointer float
    pReal = new float [numPixel]; //Allocates an array of pointer float variables
    float *pImag; //initializes pointer float
    pImag = new float [numPixel]; // Allocates an array of pointer float variables


    for (int i = 0; i<numPixel; i++)
    {   
        //declares value of 0
        pReal[i] = 0; 
        pImag[i] = 0;

        tForward[i] = scanlinePosition[i]/SoS;  //calculations for tForward
        
        for (int k = 0; k<numElement; k++)
        {

            tBackward[i][k] = (sqrt(pow(scanlinePosition[i], 2) + pow(elementPosition[k], 2)))/SoS; //calculations for tBackward
            tTotal[i][k] = tForward[i] + tBackward[i][k]; //calculations for tTotal
            s[i][k]=floor(time[i][k]*FS);  //calculations for s
            pReal[i] += realRFData[k][s[i][k]]; //calculations for pReal
            pImag[i] += imagRFData[k][s[i][k]]; //calculations for pImag
        }
        scanline[i] = sqrt(pow(pReal[i], 2) + pow(pImag[i], 2)); //calculates echo magnitude
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
    for( int i=o; i<numElement; i++)
    {
        delete realRFData[i];
        delete imagRFData[i];
    }
}