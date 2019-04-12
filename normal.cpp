#include <iostream>
#include <chrono>
#include <stdlib.h>

using namespace std;

void MatrixmultNormal(float**,float**,float**,int,int,int);
void start4();

void start4(){
    int rows1=0,columns1=0,rows2=0,columns2=0;
    FILE* outFile;
    outFile = fopen("Normal.txt","w");
  for(int count =20;count<=2000;count=count+20){
    rows1=columns1=rows2=columns2=count;
    for(int repeat=0;repeat<10;repeat++){
    float **mat1;
    float **mat2;
    float **mat3;

    mat1 = (float **)malloc( rows1*sizeof(float *));
    for(int i=0;i<rows1;i++)
    mat1[i] = (float *)malloc(columns1*sizeof(float));

    mat2 = (float **)malloc( rows2*sizeof(float *));
    for(int i=0;i<rows2;i++)
    mat2[i] = (float *)malloc(columns2*sizeof(float));

    mat3 = (float **)malloc( rows1*sizeof(float *));
    for(int i=0;i<rows1;i++)
    mat3[i] = (float *)malloc(columns2*sizeof(float));

    if (mat1 == NULL || mat2 == NULL || mat3 == NULL) {
        printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        free(mat1);
        free(mat2);
        free(mat3);
    }
    if(rows2 != columns1) {
        cout << "Matrix multiplication not possible for the given matrices";
    }
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns1;j++){
            mat1[i][j] = rand() % 1000 + 1;
        }
    }
    for(int i=0;i<rows2;i++){
        for(int j=0;j<columns2;j++){
            mat2[i][j] = rand() % 1000 + 1;
        }
    }
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns2;j++){
            mat3[i][j] =0;
        }
    }
        auto start = std::chrono::high_resolution_clock::now();
        MatrixmultNormal(mat1,mat2,mat3,rows1,columns1,columns2);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        int sum = duration.count();
        if(repeat == 4){
        fprintf(outFile,"%d\n",sum);
        cout << sum << endl;
        }
        else{ 
        fprintf(outFile,"%d ",sum);
        cout << sum << " ";
        }
        free(mat1);
        free(mat2);
        free(mat3);
    }
    }
}


void MatrixmultNormal(float** mat1,float** mat2,float** mat3,int rows1,int common,int columns2){
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns2;j++){
            float sum =0.0;
            for(int k=0;k<common;k++){
                sum = sum + (mat1[i][k])*(mat2[k][j]);
            }
            mat3[i][j] = sum;
        }
    }
}