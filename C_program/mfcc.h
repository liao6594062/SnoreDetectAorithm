#ifndef MFCC_H
#define MFCC_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define TWOPI                      6.283185307179586f
#define ONEPI                      3.141592653589793f
#define FILTER_NUMBERS             24
#define SAMPLE_FREQUENT            16000.00f
#define START_FREQUENT_PERCENTS    0.00f
#define END_FREQUENT_PERCENTS      0.50f
#define FFT_LEN                    256
#define HZ_TO_MEL_CONSTANTS        1127.01048033416f
#define MAX_MELBANK_VALUE          1.9959f
#define MFCC_FEATURE_NUM           12


typedef struct snore_data_s{
	
	float *single_filtermelbank; //the result of single filter melbank 
	float pre_emphsis[MFCC_FEATURE_NUM]; //pre_emphsis weight
	float *mfcc_feature; //mfcc feature
	float *abs_fft_data; //abs(fft data)
		
}snore_data_t;
             
int sn_get_size_of_space(void); //get size of space
unsigned char *sn_create_and_initialize_space(void); //create and initialize space
void sn_initialize_space(unsigned char *space); //initialize space
void free_point(unsigned char *space); //free space

float* get_mfcc_feature(float *window_data, unsigned char* space);//get mfcc feature


#endif 
