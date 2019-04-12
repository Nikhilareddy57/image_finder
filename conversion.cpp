#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

float* conversion(float*,int,bool);


float* conversion(float* matrix,int numColumns,bool flag)//true=>sigmoid; false=>softmax
{
	if(flag)
	{
		for(int i = 0; i <numColumns; i++)
			matrix[i] = 1/(1 + exp(-1*matrix[i]));
	}
	else
	{
		int sum =0;
		for(int i = 0; i <numColumns; i++)
			{
				matrix[i] = exp(matrix[i]);
				sum += matrix[i];
			}
		for(int i = 0; i <numColumns; i++)
			matrix[i] /= sum;		
	}
    return matrix;
	
}

