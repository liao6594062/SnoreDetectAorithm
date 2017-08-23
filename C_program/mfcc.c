#include "mfcc.h"


int sn_get_size_of_space(void) 
{
	int size;
	
	size = sizeof(snore_data_t);
	
	return size;
}
//{get size of space: end}

unsigned char *sn_create_and_initialize_space(void) 
{
	unsigned char *space;

	int size = sn_get_size_of_space();

	space = (unsigned char *)malloc(size);

	memset(space, 0, size);

	sn_initialize_space(space);

	return space;
}
//{create and initialize space: end}

void sn_initialize_space(unsigned char *space) 
{
	snore_data_t *snore_data_p = (snore_data_t*)space;
	int i;
	
	for(i=0; i<MFCC_FEATURE_NUM; i++)
	{
		snore_data_p->pre_emphsis[i] = (1+0.5*MFCC_FEATURE_NUM*sin(ONEPI*(i+1)/MFCC_FEATURE_NUM))/(1+0.5*MFCC_FEATURE_NUM);
	}
	
    snore_data_p->single_filtermelbank = (float*)malloc(sizeof(float)*(FFT_LEN/2+1));
    snore_data_p->mfcc_feature = (float*)malloc(sizeof(float)*MFCC_FEATURE_NUM);
	snore_data_p->abs_fft_data = (float*)malloc(sizeof(float)*(FFT_LEN/2+1));
}
//{initialize space: end}----------------------


void free_point(unsigned char *space)
{
	snore_data_t* snore_data_p = (snore_data_t*)space;
	
	free(snore_data_p->single_filtermelbank);
	free(snore_data_p->mfcc_feature);
	free(snore_data_p->abs_fft_data);
}
//{free point: end}

static float hamming_window(int data_index)
{
	int m = 0;
	float hamming_w = 0.0;
	
	if(FFT_LEN%2 == 0)
	{
		m = FFT_LEN/2;
	}
	else
	{
		m=(FFT_LEN+1)/2;
	}
	
	if(data_index>=m)
	{
		hamming_w = 0.54 - 0.46*cos(TWOPI*(-data_index)/(FFT_LEN-1));
	}
	else
	{
		hamming_w = 0.54 - 0.46*cos(TWOPI*data_index/(FFT_LEN-1));
	}
	
	return hamming_w;
}
//{hamming window: end}

static void  hamming_rfft(float *s_data, float *abs_fft_data)  
{
	int require_fft_len = FFT_LEN/2+1;
	int i = 0, j = 0;
	float real, imag; 
	
	for(i=0; i<require_fft_len; i++)
	{   
	    real = 0;
	    imag = 0;
		for(j=0; j<FFT_LEN; j++)
		{
			real = real+s_data[j]*(1-0.9375)*hamming_window(j)*cos(-(TWOPI*j*i)/FFT_LEN);
			imag = imag+s_data[j]*(1-0.9375)*hamming_window(j)*sin(-(TWOPI*j*i)/FFT_LEN);	
		}
		abs_fft_data[i] = pow(real, 2)+pow(imag, 2);	
	} 
}
//{hamming_rfft: rfft under hamming window. end}

static int sign(float number)
{
	if(number>0)
	{
		return 1;
	}
	else if(number<0)
	{
		return -1;
	}
	else{
		return 0;
	}
	
}
//{sign: end}

static float fabs_value(float number)
{
	if(number<0)
	{
		return (-1)*number;		
	}
	else
	{
		return number;
	}
}
//{fabs(number): end}

static float frq2mel(float frequent)
{
	return sign(frequent)*log(1+fabs_value(frequent)/700)*HZ_TO_MEL_CONSTANTS;
}
//{frq2mel: end}

static void single_melfilter(float *single_melbank, int p_index)
{
	float hz_lower = START_FREQUENT_PERCENTS*SAMPLE_FREQUENT;
	float hz_upper = END_FREQUENT_PERCENTS*SAMPLE_FREQUENT;
	float mel_lower = frq2mel(hz_lower);
	float mel_upper = frq2mel(hz_upper);
	int require_fft_len = FFT_LEN/2+1;
	float melinc = (mel_upper-mel_lower)/(FILTER_NUMBERS+1);
	int index = 0;
	float index_mel = 0.0, non_mel = 0.0;
	
	for(index=0; index<require_fft_len; index++)
	{
		index_mel = frq2mel(1.0*index/FFT_LEN*SAMPLE_FREQUENT);
		single_melbank[index] = 0.0;
		if(index_mel>=mel_lower+(p_index-1)*melinc&&index_mel<=mel_lower+(p_index+1)*melinc)
		{
			non_mel = (index_mel-mel_lower)/melinc;
			if(non_mel>=p_index)
			{
				single_melbank[index] = 2*(p_index+1-non_mel);
			}
			else
			{
				single_melbank[index] = 2*(non_mel-p_index+1);
			}
		}	
	}		
}
//{single melfilter: end}

static void achieve_mfcc_feature(float *window_data, unsigned char *space)
{
	snore_data_t* snore_data_p = (snore_data_t*)space;
	float *dct = (float*)malloc(sizeof(float)*MFCC_FEATURE_NUM);
	int temp1 = 0, temp2 = 0, temp3 = 0;
	float sum = 0.0;
	/*for(temp1=0; temp1<MFCC_FEATURE_NUM; temp1++)
	{
		printf("%7.4f",snore_data_p->pre_emphsis[temp1]);
	}
	printf("\n");*/
	hamming_rfft(window_data, snore_data_p->abs_fft_data);
	/*for(temp1=0; temp1<FFT_LEN/2+1; temp1++)
	{
		printf("%12.4f",snore_data_p->abs_fft_data[temp1]);
	}
	printf("\n");*/
	
	for(temp1=0; temp1<MFCC_FEATURE_NUM; temp1++)
	{
		dct[temp1] = 0.0;
		for(temp2=0; temp2< FILTER_NUMBERS; temp2++)
		{
			single_melfilter(snore_data_p->single_filtermelbank, temp2+1);
			/*for(k=0; k<FFT_LEN/2+1; k++)
	        {
		       printf("%7.4f",snore_data_p->single_filtermelbank[k]);
	        }
	        printf("\n");*/
			sum = 0.0;
			for(temp3=0; temp3< FFT_LEN/2+1; temp3++)
			{
				sum = sum+snore_data_p->single_filtermelbank[temp3]*snore_data_p->abs_fft_data[temp3]; 
			}
			dct[temp1] = dct[temp1]+cos((2*temp2+1)*(temp1+1)*ONEPI/(2*FILTER_NUMBERS))*log(sum/MAX_MELBANK_VALUE);
		}
		//printf("%12.6f",dct[temp1]);
		snore_data_p->mfcc_feature[temp1] = dct[temp1]*snore_data_p->pre_emphsis[temp1];
	}
	//printf("\n");
	free(dct);
}
//{achieve mfcc feature: end}

float* get_mfcc_feature(float *window_data, unsigned char* space)
{
	snore_data_t* snore_data_p = (snore_data_t*)space;
	achieve_mfcc_feature(window_data, space);
	
	return  snore_data_p->mfcc_feature;	
}
//{get mfcc feature: end}













