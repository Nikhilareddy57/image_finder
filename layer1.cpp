#include <iostream>
#include "header1.h"

using namespace std;

float*** make3DMatrix(int,int);
float*** read3DMatrix(char*, int,int);
float*** pool(float***,int,int);
float*** conv(float***,int,int,int,int,char*);
void printMatrix(float***,int,int,int);

int main(int argc,char* argv[]){
    float*** input_mat;
    float* result;
    input_mat = read3DMatrix(argv[1], 28,1);
    float*** mat1;
    mat1 = conv(input_mat,28,1,5,20,"conv1.txt");
    float*** mat2;
    mat2 = conv(mat1,12,20,5,50,"conv2.txt");
    float*** mat3;
    mat3 = fc(mat2,4,50,4,500,1,"fc1.txt"); 
    float*** mat4;
    mat4 = fc(mat3,1,500,1,10,2,"fc2.txt");
    float* mat5;
    mat5 = new float[10];
    for(int i=0;i<10;i++){
      mat5[i]=mat4[i][0][0];
    }
    result = conversion(mat5,10,false);
    float max = result[0];
    int answer = 0;
    for(int i=1; i<10; i++)
    {
    	if(result[i] > max)
    	{
    		answer = i;
    		max = result[i];
    	}
    }	

    FILE* outFile;
    outFile = fopen("FinalResult.txt","w");
    fprintf(outFile, "Probabilities:\n" );
    cout <<"Probabilities:\n";
    for(int i=0;i<10;i++){
      fprintf(outFile, "%d ",i);
      fprintf(outFile, "%f\n",result[i]);
      cout<<i << " "<< result[i]<<endl;
    }
    fprintf(outFile, "The most probable integer is %d\n", answer);
    cout << "The most probable integer is " << answer <<endl;
    
    return 0;
}

float*** conv(float*** input_mat,int input_size,int input_chan,int filter_size,int numFilters,char* filename){
      float*** result;
      result=fc(input_mat,input_size,input_chan,filter_size,numFilters,0,filename);
      return (pool(result,1+input_size-filter_size,numFilters));
}

float*** pool(float*** input_mat,int input_size,int input_chan){
	  float*** result;
      result = make3DMatrix((input_size/2),input_chan);
      for(int i=0;i<input_chan;i++){
           result[i] = subsampling(input_mat[i],input_size,2,true);  //stride =2 ; maxpooling
      }
      return result;
}

float*** read3DMatrix(char* fileName, int size,int height){
  FILE* inFile;
  inFile = fopen(fileName,"r");
	float*** mat;
	mat = make3DMatrix(size,height);
	for(int i=0;i<height;i++){
		for(int j=0;j<size;j++){
			for(int k=0;k<size;k++){
				fscanf(inFile,"%f",&mat[i][j][k]);
			}
		}
	}
	return mat;
}

float*** make3DMatrix(int size,int height){
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
void printMatrix(float*** mat,int r,int c,int h){
    FILE* outFile;
    outFile = fopen("conv11.txt","w");
    for(int i=0;i<h;i++){
        for(int j=0;j<r;j++){
            for(int k=0;k<c;k++){
                fprintf(outFile,"%f ",mat[i][j][k]);
            }
            fprintf(outFile,"%s","\n");
        }
        fprintf(outFile,"%s","\n");
    }
}
// g++ layer1.cpp layer2.cpp convolution.cpp subsampling.cpp matrix_activation.cpp conversion.cpp -o hello
/* printMatrix(mat4,1,1,10);
    for(int i=0;i<10;i++){
      cout<<mat4[i][0][0]<<" ";
      mat5[i]=mat4[i][0][0];
    }
    cout << "\n";
    */