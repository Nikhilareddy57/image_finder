#include <iostream>
#include <stdlib.h>
#include <chrono>

using namespace std;

struct SetOfMatrices{
    float** mat1;
    float** mat2;
    float** mat3;
    int rows1;
    int common;
    int columns2;
    int start;
    int end;
    int threadId;
}SetOfMatrices;

void MatrixmultPthread(float**,float**,float**,int,int,int,int);
void* multi(void*);
void start2();

void start2(){
    int rows1=0,columns1=0,rows2=0,columns2=0;
    FILE* outFile;
    outFile = fopen("Pthreads.txt","w");
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
            mat2[i][j] = rand() % 1000 + 1;;
        }
    }
    for(int i=0;i<rows1;i++){
        for(int j=0;j<columns2;j++){
            mat3[i][j] =0;
        }
    }
        auto start = std::chrono::high_resolution_clock::now();
    MatrixmultPthread(mat1,mat2,mat3,rows1,columns1,columns2,4);
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
    free(mat1);
    free(mat2);
    free(mat3);
    }
    }
}

void* multi(void* arguments){
    struct SetOfMatrices *args = (struct SetOfMatrices *)arguments;
    float sum = 0.0;
    int start = args->start;
    int end = args->end;
    int common = args->common;
    int columns2 = args->columns2;
    for(int i=start;i<end;i++){
        for(int j=0;j<columns2;j++){
            float sum =0.0;
            for(int k=0;k<common;k++){
                sum = sum + (args->mat1[i][k])*(args->mat2[k][j]);
            }
            (*args).mat3[i][j] = sum;
        }
    }
}

void MatrixmultPthread(float** mat1,float** mat2,float** mat3,int rows1,int common,int columns2,int numThreads){
    pthread_t threads[numThreads]; 
    struct SetOfMatrices args[numThreads];
    for(int i=0;i<numThreads;i++){
        args[i].mat3 = mat3;
    }
    for (int i=0;i<numThreads; i++) { 
        args[i].mat1=mat1;
        args[i].mat2=mat2;
        args[i].common=common;
        args[i].rows1=rows1;
        args[i].columns2=columns2;
        args[i].threadId=i;
        args[i].start = i*(rows1/numThreads);
        if(i != numThreads-1){
           args[i].end = (i+1)*(rows1/numThreads);
        }else{
           args[i].end = rows1;
        }
        pthread_create(&threads[i], NULL, multi, (void*)(&args[i]));
    }
    for (int i = 0; i < numThreads; i++){  
        pthread_join(threads[i], NULL);
    }
}