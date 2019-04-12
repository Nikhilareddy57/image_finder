#include <iostream>
#include <chrono>
#include <stdlib.h>
#include "mkl.h"

using namespace std;

void MatrixMultiplicationMKL(double*,double*,double*,int,int,int);
void start1();

void start1(){
    int rows1=0,rows2=0,columns1=0,columns2=0;
    FILE* outFile;
    outFile = fopen("Mklmult.txt","w");
    for(int count =20;count<=2000;count = count+20){
        rows1=columns1=rows2=columns2=count;
     for(int repeat =0;repeat <10;repeat++){
    double *mat1;
    double *mat2;
    double *mat3;
    mat1 = (double *)mkl_malloc( rows1*columns1*sizeof( double ), 64 );
    mat2 = (double *)mkl_malloc( rows2*columns2*sizeof( double ), 64 );
    mat3 = (double *)mkl_malloc( rows1*columns2*sizeof( double ), 64 );
    if (mat1 == NULL || mat2 == NULL || mat3 == NULL) {
        printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        mkl_free(mat1);
        mkl_free(mat2);
        mkl_free(mat3);
    }
    if(rows2 != columns1) {
        cout << "Matrix multiplication not possible for the given matrices";
    }
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns1;j++){
            mat1[(i*rows1)+j] = rand() % 1000 + 1;;
        }
    }
    for(int i=0;i<rows2;i++){
        for(int j=0;j<columns2;j++){
            mat2[(i*rows2)+j] = rand() % 1000 + 1;
        }
    }
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns2;j++){
            mat3[(i*rows1)+j] = 0;
        }
    }
        auto start = std::chrono::high_resolution_clock::now();
    MatrixMultiplicationMKL(mat1,mat2,mat3,rows1,columns1,columns2);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        int sum = duration.count();
        if(repeat == 9){
        fprintf(outFile,"%d\n",sum);
        cout << sum << endl;
        }
        else{ 
        fprintf(outFile,"%d ",sum);
        cout << sum << " ";
        }
    mkl_free(mat1);
    mkl_free(mat2);
    mkl_free(mat3);
    
    }
    }
}

void MatrixMultiplicationMKL(double* mat1,double* mat2,double* mat3,int rows1,int common,int columns2){
    double alpha = 1.0;double beta=0.0;
    cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,rows1,columns2,common,alpha,mat1,common,mat2,columns2,beta,mat3,columns2);
}