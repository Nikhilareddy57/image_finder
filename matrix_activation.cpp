#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;


float** matrix_activation(float**,int,bool);

float** matrix_activation(float** matrix,int numRows,bool flag)  //true=>relu; false=>tanh
{
	if(flag)
	{
		for(int i =0; i<numRows; i++)
			for(int j=0; j<numRows; j++)
				if(matrix[i][j] < 0)
					matrix[i][j] = 0;
	}
	else
	{
		for(int i =0; i<numRows; i++)
			for(int j=0; j<numRows; j++)
				matrix[i][j] = tanh(matrix[i][j]);
	}
    
    return matrix;
	
}

