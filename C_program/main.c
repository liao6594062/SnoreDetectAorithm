#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mfcc.h"
#include "readcsv.h"
#include "audio_signal_process.h"
#define M  (1<<8)

int main(int argc, char* argv[])
{
	char *fname = "C:\\Users\\EEG\\Desktop\\test.csv"; //the address of test_File 
	int row, col; // the row and col of data
	float *raw; // save data as vector  
	int num_index = 0; // the order of data elements
    unsigned char* space1 = sn_create_and_initialize_space();
    //unsigned char* space2 = ad_create_and_initialize_space();
    float *mfcc_feature = (float*)malloc(sizeof(float)*12);
    //float *imag =(float*)malloc(sizeof(float)*256);
    //float time_decibel_value = 0.0;
    //float freq_decibel_value = 0.0;
    //float *data = (float*)malloc(sizeof(float)*M);
    //float *fft_real = (float*)malloc(sizeof(float)*M);
    //float *fft_imag = (float*)malloc(sizeof(float)*M);
    
    //for(row=0; row<M; row++)
    //{
    	//data[row] = row+1;
	//}
    
    col = get_col_of_raw(fname); //get the col of raw 
	row = get_row_of_raw(fname, col); // get the row of raw 
	raw = (float*)malloc(sizeof(float) * row * col);  // get the data named raw 
	get_raw_from_csv(fname, raw, row, col); // get raw from csv 
	printf("size of matrix: %d x %d\n", row, col); // print row and col
	//memset(imag, 0, sizeof(float)*256);
	
	mfcc_feature = get_mfcc_feature(raw, space1); // get mfcc feature
	//comput_decibel_value(raw, space2); 
	//time_decibel_value = get_time_decibel_value(space2);
	//freq_decibel_value = get_freq_decibel_value(space2);
	//get_last_fft_index(fft_index, 16, 4);
	//fft(data, fft_real, fft_imag, M, (int)log2(M));
	for(num_index=0; num_index<12; num_index++)
	{
		printf("%f\n",mfcc_feature[num_index]); //print mfcc feature
	}
	//printf("%f,%f\n",time_decibel_value, freq_decibel_value);
    free_point(space1); //free point
    //ad_free_point(space2);
    return 0;
}
