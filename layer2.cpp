#include <iostream>
#include "header2.h"
#include <string.h>
#include <sstream>
#include <fstream>

using namespace std;
struct Obj{
     float**** mat;
     float* bias;
};
float** make2Dmatrix(int);
float*** make3Dmatrix(int,int);
void addMatrices(float**,float**,int);
Obj readFile(char*,int,int,int);
float*** fc(float***,int,int,int,int,int,string);
void addBias(float** mat,int size,float bias);

float*** fc(float*** input_mat,int input_size,int inputChannels,int filter_size,int numFilters,int id,char* filename){
    float ****filters;
    float *bias;
    Obj obj = readFile(filename,filter_size,inputChannels,numFilters); // filename="fc1.txt"
    filters = obj.mat;
    bias = obj.bias;
    float **temp;
    temp = make2Dmatrix(1+input_size-filter_size);
    float ***result;
    result = make3Dmatrix(1+input_size-filter_size,numFilters);
    
    for(int i=0;i<numFilters;i++){
        for(int j=0;j<inputChannels;j++){
            temp = convolution(input_mat[j],input_size,filters[i][j],filter_size,0, true, 1);
            addMatrices(temp,result[i],1+input_size-filter_size);
        }
        addBias(result[i],1+input_size-filter_size,bias[i]);
        if(id == 1){
        matrix_activation(result[i],1+input_size-filter_size,true);
        }
    }
    return result;
}
Obj readFile(char* fileName,int size,int height,int numFilters){
    float ****mat;
    mat = new float***[numFilters];
    
    for(int i=0;i<numFilters;i++){
      mat[i] = new float**[height];
      for(int j=0;j<height;j++){
        mat[i][j] = new float*[size];
        for(int k=0;k<size;k++){
        mat[i][j][k] = new float[size];
        }
      }
    }
    FILE* inFile;
    inFile = fopen(fileName,"r");
    for(int i=0;i<numFilters;i++){
        for(int j=0;j<height;j++){
            for(int k=0;k<size;k++){
                for(int m=0;m<size;m++){
                  fscanf(inFile,"%f",&mat[i][j][k][m]);
                }
            }
        }
    }
    float* bias;
    bias = new float[numFilters];
    for(int i=0;i<numFilters;i++){
      fscanf(inFile,"%f",&bias[i]);
    }
    Obj obj;
    obj.mat = mat;
    obj.bias = bias;
    return obj;
}
float** make2Dmatrix(int size){
     float** mat;
     mat = new float*[size];
     for(int i=0;i<size;i++){
        mat[i] = new float[size];
     }
     return mat;
}
float*** make3Dmatrix(int size,int height){
    float ***mat;
    mat = new float**[height];
    for(int i=0;i<height;i++){
      mat[i] = new float*[size];
      for(int j=0;j<size;j++){
      mat[i][j] = new float[size];
      }
    }
    return mat;
}
void addMatrices(float** mat1,float** mat2,int size){
     for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
          mat2[i][j] = mat1[i][j]+mat2[i][j];
        }
     }
}
void addBias(float** mat,int size,float bias){
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      mat[i][j] = mat[i][j]+bias;
    }
  }
}

// g++ fc1.cpp conv.cpp -o hello