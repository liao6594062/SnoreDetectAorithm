#include "audio_signal_process.h"

int ad_get_size_of_space(void) 
{
	int size;
	
	size = sizeof(audio_data_t);
	
	return size;
}
//{get size of space: end}

unsigned char *ad_create_and_initialize_space(void) 
{
	unsigned char *space;

	int size = ad_get_size_of_space();

	space = (unsigned char *)malloc(size);

	memset(space, 0, size);

	ad_initialize_space(space);

	return space;
}
//{create and initialize space: end}

void ad_initialize_space(unsigned char *space) 
{
	audio_data_t *audio_data_p = (audio_data_t*)space;

    audio_data_p->moving_filter_data = (float*)malloc(sizeof(float)*COMPUT_LEN);
    audio_data_p->audio_data_dft = (float*)malloc(sizeof(float)*(DFT_LEN/2+1));
    
    audio_data_p->time_decibel_value = 0.0;
    audio_data_p->freq_decibel_value = 0.0;
    
}
//{initialize space: end}----------------------


void ad_free_point(unsigned char *space)
{
	audio_data_t *audio_data_p = (audio_data_t*)space;
	
	free(audio_data_p->moving_filter_data);
	
}

static float comput_mean_value(float *data, int start, int len)
{
	int index = 0;
	float sum = 0.0;
	
	for(index=start; index<start+len; index++)
	{
		sum = sum+data[index];
	}
	
	return(sum/len);
}

static void moving_average_filter(float *audio_data, unsigned char *space) 
{
   audio_data_t *audio_data_p = (audio_data_t*)space;
   int index = 0, filter_time = 0;
   float *temp = (float*)malloc(sizeof(float)*COMPUT_LEN);
   
   memcpy(audio_data_p->moving_filter_data, audio_data, sizeof(float)*COMPUT_LEN);
   memcpy(temp, audio_data_p->moving_filter_data, sizeof(float)*COMPUT_LEN);
   
   for(filter_time=0; filter_time<FILTER_TIMES; filter_time++)
   {
   	 for(index=FILTER_LEN-1; index<COMPUT_LEN; index++)
   	 {
   	 	audio_data_p->moving_filter_data[index]	= comput_mean_value(temp, index-FILTER_LEN+1, FILTER_LEN);
	 }
	 
	 memcpy(temp, audio_data_p->moving_filter_data, sizeof(float)*COMPUT_LEN);
   }
   
   free(temp);
    	
}

static void  abs_rdft(float *s_data, float *abs_dft_data)  
{
	int require_dft_len = DFT_LEN/2+1;
	int i = 0, j = 0;
	float real, imag; 
	
	for(i=0; i<require_dft_len; i++)
	{   
	    real = 0;
	    imag = 0;
		for(j=0; j<DFT_LEN; j++)
		{
			real = real+s_data[j]*cos(-(TWO_PI*j*i)/DFT_LEN); //得进一步优化，更新fft算法 
			imag = imag+s_data[j]*sin(-(TWO_PI*j*i)/DFT_LEN);	
		}
		abs_dft_data[i] = pow(real, 2)+pow(imag, 2);	
	} 
}

static void  get_last_fft_index(int *fft_index, int fft_len, int tree_deep)
{
	int i = 0, j = 0, k = 0;
	int step = 0, step_font = 0;
	int temp1 = 0, temp2 = 0;
	
	if(log2(fft_len)<=tree_deep)
	{
		tree_deep = (int)(log2(fft_len));
	}
	
	for(i=0; i<fft_len; i++)
	{
		fft_index[i] = i;
	}
	
	for(i=1; i<=tree_deep; i++)
	{
		step = 1<<i;
		step_font = 1<<(i-1);
		
		for(j=1; j<=step_font; j++)
		{
			for(k=0; k<fft_len/step; k++)
			{
				temp1 = fft_index[(j-1)*fft_len/step_font]+k*step;
				temp2 = fft_index[(j-1)*fft_len/step_font+1]+k*step;
				fft_index[(j-1)*fft_len/step_font+k] = temp1;
				fft_index[(j-1)*fft_len/step_font+k+fft_len/step] = temp2;
			}
		}	
	}	
}

void  fft(float *data, float *fft_real, float *fft_imag, int fft_len, int tree_deep)
{
	int i = 0, j = 0, k = 0;
	int class_num = 0, every_class_num = 0;
	float temp1_real = 0.0, temp1_imag = 0.0;
	float temp2_real = 0.0, temp2_imag = 0.0;
	float wnk_real = 0.0, wnk_imag = 0.0;
	int *fft_index = (int*)malloc(sizeof(int)*fft_len);
	
	if(fabs(tree_deep-log2(fft_len))>=log2(1+1.0/fft_len))
	{
		printf("error: fft_len must be 2^k\n");
	}
	else
	{
	   get_last_fft_index(fft_index, fft_len, tree_deep);
	   
	   for(i=0; i< fft_len; i++)
	   {
	   	  fft_real[i] = data[fft_index[i]];
	   }
	   
	   memset(fft_imag, 0.0, sizeof(float)*fft_len);
	   free(fft_index);
	   
	   for(i=tree_deep-1; i>=0; i--)
	   {
		  class_num = 1<<i;
		  every_class_num = fft_len/class_num;
		  for(j=0; j< class_num; j++)
		  {
			 for(k=0; k< every_class_num/2; k++)
			 {
			   temp1_real = fft_real[j*every_class_num+k];
			   temp1_imag = fft_imag[j*every_class_num+k];
			   temp2_real = fft_real[j*every_class_num+k+every_class_num/2];
			   temp2_imag = fft_imag[j*every_class_num+k+every_class_num/2];
			  
			   wnk_real = cos(-TWO_PI*k/every_class_num);
			   wnk_imag = sin(-TWO_PI*k/every_class_num);
			  
			   fft_real[j*every_class_num+k] = temp1_real+wnk_real*temp2_real-wnk_imag*temp2_imag;
			   fft_imag[j*every_class_num+k] = temp1_imag+wnk_real*temp2_imag+wnk_imag*temp2_real;
			   fft_real[j*every_class_num+k+every_class_num/2] = temp1_real-wnk_real*temp2_real+wnk_imag*temp2_imag;
			   fft_imag[j*every_class_num+k+every_class_num/2] = temp1_imag-wnk_real*temp2_imag-wnk_imag*temp2_real;	
			}
		}
	}
	
	/*for(i=0; i<fft_len; i++)
	{
		printf("%f ", fft_real[i]);
    }
    printf("\n");
    for(i=0; i<fft_len; i++)
	{
		printf("%f ", fft_imag[i]);
    }
    printf("\n");*/
  }
 	
}

void comput_decibel_value(float *audio_data, unsigned char *space)
{
	audio_data_t *audio_data_p = (audio_data_t*)space;
	float sum = 0.0;
	int index = 0;
	
	moving_average_filter(audio_data, space);
	
	for(index=0; index< COMPUT_LEN; index++)
	{
		sum = sum + pow(audio_data_p->moving_filter_data[index],2)/COMPUT_LEN;
	}
	audio_data_p->time_decibel_value = 10*log10(sum);
	
	abs_rdft(audio_data_p->moving_filter_data, audio_data_p->audio_data_dft);
	
	sum = 0.0;
	for(index=1; index<DFT_LEN/2+1; index++)
	{
		sum = sum + audio_data_p->audio_data_dft[index]/DFT_LEN*2.0;
	}

	if((DFT_LEN-1)%2==0)
	{
		sum = sum+audio_data_p->audio_data_dft[0]/DFT_LEN;
	}
	else
	{
		sum = sum+audio_data_p->audio_data_dft[0]/DFT_LEN-audio_data_p->audio_data_dft[DFT_LEN/2]/DFT_LEN; 
	}
	audio_data_p->freq_decibel_value = 10*log10(sum);
		 
}


float get_time_decibel_value(unsigned char *space)
{
	audio_data_t *audio_data_p = (audio_data_t*)space;
	
	return  audio_data_p->time_decibel_value;
}

float get_freq_decibel_value(unsigned char *space)
{
	audio_data_t *audio_data_p = (audio_data_t*)space;
	
	return  audio_data_p->freq_decibel_value;
}

