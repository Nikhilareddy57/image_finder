#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "header3.h"

using namespace std;

class Matrix{
        public: int numRows=0;
        public: int numColumns=0;
        public: float **element;
    Matrix(float** matrix,int numRows,int numColumns,int padSize){
        this->numRows = numRows+2*padSize;
        this->numColumns = numColumns+2*padSize;
        element = new float*[numRows+2*padSize];
        for(int i=0;i<numRows+2*padSize;i++)
         element[i] = new float[numColumns+2*padSize];
        for(int i=0;i<padSize;i++){
            for(int j=0;j<numColumns+2*padSize;j++){
                element[i][j] = 0.0;
                element[numRows+padSize+i][j] = 0.0;
            }
            for(int j=0;j<numRows+2*padSize;j++){
                element[j][i] = 0.0;
                element[j][numColumns+padSize+i] = 0.0;
            }
        }
        for(int i=padSize;i<numRows+padSize;i++){
            for(int j=padSize;j<numColumns+padSize;j++){
                element[i][j] = matrix[(i-padSize)][(j-padSize)];
            }
        }
        
    }
     
    Matrix(int numRows,int numColumns){
        this->numRows = numRows;
        this->numColumns = numColumns;
        element = new float*[numRows];
        for(int i=0;i<numRows;i++)
         element[i] = new float[numColumns];
        for(int i=0;i<numRows;i++){
            for(int j=0;j<numColumns;j++){
                element[i][j] = 0.0;
            }
        }
    }
};

Matrix convolution1(Matrix,Matrix);
Matrix convolution2(Matrix,Matrix,int);
float** convolution(float**,int,float**,int,int,bool,int);
double* changeType(float**,int,int);

Matrix convolution1(Matrix inputMatrix,Matrix kernal){
    int rows = 1+inputMatrix.numRows-kernal.numRows;
    int columns = 1+inputMatrix.numColumns-kernal.numColumns;
    Matrix outputMatrix(rows,columns);
    for(int i=0;i<=inputMatrix.numRows-kernal.numRows;i++){
        for(int j=0;j<=inputMatrix.numColumns-kernal.numColumns;j++){
            float sum = 0.0;
            for(int k1=i;k1<i+kernal.numRows;k1++){
                for(int k2=j;k2<j+kernal.numColumns;k2++){
                    sum = sum + (inputMatrix.element[k1][k2])*(kernal.element[(k1-i)][(k2-j)]);
                }
            }
            outputMatrix.element[i][j] = sum;
        }
    }
    return outputMatrix;
}

Matrix convolution2(Matrix inputMatrix,Matrix kernal,int id){
    int rows = (1+inputMatrix.numRows-kernal.numRows)*(1+inputMatrix.numColumns-kernal.numColumns);
    int columns = (kernal.numRows)*(kernal.numColumns);
    Matrix resultMatrix((1+inputMatrix.numRows-kernal.numRows)*(1+inputMatrix.numColumns-kernal.numColumns),1);
    Matrix outputMatrix((1+inputMatrix.numRows-kernal.numRows),(1+inputMatrix.numColumns-kernal.numColumns));
    Matrix topazMatrix(rows,columns);
    Matrix columnVector(kernal.numRows*kernal.numColumns,1);
    int rowNum = 0;
    for(int i=0;i<kernal.numRows;i++){
        for(int j=0;j<kernal.numRows;j++){
            columnVector.element[rowNum][0] = kernal.element[i][j];
            rowNum = rowNum+1;
        }
    }
    rowNum = 0;
    for(int i=0;i<=inputMatrix.numRows-kernal.numRows;i++){
        for(int j=0;j<=inputMatrix.numColumns-kernal.numColumns;j++){
            for(int k1=i;k1<i+kernal.numRows;k1++){
                for(int k2=j;k2<j+kernal.numColumns;k2++){
                   int columnNum = (k1-i)*(kernal.numColumns)+(k2-j);
                   topazMatrix.element[rowNum][columnNum] = (inputMatrix.element[k1][k2]);
                }
            }
            
            rowNum = rowNum+1;
        }
    }
    if(id==1||id==2){
        double* mat1 = changeType(topazMatrix.element,topazMatrix.numRows,topazMatrix.numColumns);
        double* mat2 = changeType(columnVector.element,columnVector.numRows,columnVector.numColumns);
        double* mat3 = changeType(resultMatrix.element,resultMatrix.numRows,resultMatrix.numColumns);
        if(id==1){
        MatrixMultiplicationMKL(mat1,mat2,mat3,topazMatrix.numRows,topazMatrix.numColumns,columnVector.numColumns);
        }
        else{
        MatrixMultiplicationOpenBlas(mat1,mat2,mat3,topazMatrix.numRows,topazMatrix.numColumns,columnVector.numColumns);
        }
      
        for(int i=0;i<outputMatrix.numRows;i++){
            for(int j=0;j<outputMatrix.numColumns;j++){
                outputMatrix.element[i][j] = mat3[i*outputMatrix.numColumns+j];
            }
        }
        
        return outputMatrix;
    }   
    else{
        if(id==3){
        MatrixmultPthread(topazMatrix.element,columnVector.element,resultMatrix.element,topazMatrix.numRows,topazMatrix.numColumns,columnVector.numColumns,4);
        }
        else{
        MatrixmultNormal(topazMatrix.element,columnVector.element,resultMatrix.element,topazMatrix.numRows,topazMatrix.numColumns,columnVector.numColumns);
        }

        for(int i=0;i<outputMatrix.numRows;i++){
            for(int j=0;j<outputMatrix.numColumns;j++){
                outputMatrix.element[i][j] = resultMatrix.element[i*outputMatrix.numColumns+j][0];
            }
        }
        
        return outputMatrix;
    }
}

float** convolution(float** mat1,int numRows1,float** mat2,int numRows2,int padSize,bool flag,int id){
    Matrix input(mat1,numRows1,numRows1,padSize);
    Matrix kernal(mat2,numRows2,numRows2,0);
    if(flag){
         Matrix output= convolution2(input,kernal,id);
         return output.element;
    }else{
         Matrix output= convolution1(input,kernal);
         return output.element;
    }
}

double* changeType(float** mat,int rows,int columns){
     double* result;
     result = (double *)malloc( rows*columns*sizeof( double ));
     for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            result[(i*columns)+j] = mat[i][j];  //rows replaced with columns;columns replaced with j;
        }
     }
     return result;
}
