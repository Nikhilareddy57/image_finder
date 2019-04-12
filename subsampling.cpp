#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

float** subsampling(float**,int,int,bool);

float** subsampling(float** input,int numRows,int stride,bool flag)  //true=>maxpool; false=>avgpool
{
    float matrix[numRows][numRows];
    for(int j=0;j<numRows;j++){
        for(int i=0;i<numRows;i++){
           matrix[i][j] = input[i][j];
        }
    }
    int n = numRows,m;
    if(n%stride == 0) m = n/stride;    
    else m = n/stride + 1;              
    float** output;
	output = new float*[numRows];
	for(int i=0;i<numRows;i++){
	    output[i] = new float[numRows];
	}
	if(flag)
	{
		for(int i =0; i<n; i+=stride)
		{
			for(int j =0; j<n; j+=stride)
			{	
				float max = matrix[i][j];

				for(int p = i; p< i+stride && p<n ; p++)
					for(int q = j; q < j+stride && q < n; q++)
						if(max < matrix[p][q])
							max = matrix[p][q];

				output[i/stride][j/stride]= max;
			}
		}
	}
	else
	{

		for(int i =0; i<n; i+=stride)
		{
			for(int j =0; j<n; j+=stride)
			{	
				float avg = 0;

				for(int p = i; p< i+stride && p<n ; p++)
					for(int q = j; q < j+stride && q < n; q++)
						avg += matrix[p][q];

				avg = avg/(stride*stride);
				output[i/stride][j/stride]= avg;
			}
		}
	}
	return output;
}


