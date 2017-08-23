#ifndef AUDIO_SIGNAL_PROCESS_H
#define AUDIO_SIGNAL_PROCESS_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


#define  COMPUT_LEN                 256
#define  FILTER_LEN                 5
#define  FILTER_TIMES               5
#define  DFT_LEN                    256
#define  TWO_PI                     6.283185307179586  


typedef  struct  audio_data_s
{
	float *moving_filter_data;
	float *audio_data_dft;
	float time_decibel_value;
	float freq_decibel_value;
	
}audio_data_t;


int ad_get_size_of_space(void); //get size of space
unsigned char *ad_create_and_initialize_space(void); //create and initialize space
void ad_initialize_space(unsigned char *space); //initialize space
void ad_free_point(unsigned char *space); //free space

void comput_decibel_value(float *audio_data, unsigned char *space);//计算分贝数 
float get_time_decibel_value(unsigned char *space);//获取时域分贝数 
float get_freq_decibel_value(unsigned char *space);//获取频域分贝数 
void  fft(float *data, float *fft_real, float *fft_imag, int fft_len, int tree_deep); //fft
//void fft(float *real, float *imag, int fft_len, int k);


#endif
