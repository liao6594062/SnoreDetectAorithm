// 2017-03-26
// created by Yugao Li
// function: read raw from csv file
// type: header file
//--------------------------------------------------------------

#include "readcsv.h"

//get col of csv------------------------------------------------
int get_col_of_raw(char *fname){
	int col = 0;
	char ch;
	FILE *fp;

  
	fp = fopen(fname, "r");
	if(fp == NULL){
		printf("Error: can not open this file\n");
		exit(0);
	}

	
	while(1){
		ch = fgetc(fp);
		if(ch == ','){
			col ++;
		}
		if(ch == '\n'){
			col ++;
			return col;
		}
		if(ch == EOF){
			return col;
		}
		
	}
	fclose(fp);
}


//get row of csv------------------------------------------------
int get_row_of_raw(char *fname, int col){
	int row, char_num_per_row;
	char *buf;
	FILE *fp;

    
	row = 0;
	char_num_per_row = 10*col + 1;
    buf = (char*)malloc(sizeof(char)*char_num_per_row);
	fp = fopen(fname, "r");

	if(fp == NULL){
		printf("Error: can not open this file\n");
		exit(0);
	}

	while(fgets(buf,char_num_per_row, fp) != NULL){
		row ++;
	}
	free(buf);
	fclose(fp);
	return row;
}

//get raw from csv----------------------------------------------
void get_raw_from_csv(char *fname, float *raw, int row, int col){
	register int i, j;
	int char_num_per_row;
	char *buf;
	char *str;
	FILE *fp;


    
	char_num_per_row = 50*col + 1;
    buf = (char*)malloc(sizeof(char)*char_num_per_row);
	fp = fopen(fname, "r");

	if(fp == NULL){
		printf("Error: can not open this file\n");
		exit(0);
	}

	for(i=0; i<row; i++){
		fgets(buf,char_num_per_row, fp);
		str = strtok(buf, " ,");
		if(str == NULL){
			printf("Error is from strtok!\n");
			break;
		}
		
		raw[i*col] = (float)(atof(str));
		for(j=1; j<col; j++){
			str = strtok(NULL, " ,");
			if(str == NULL){
				printf("Error is from strtok!!\n");
				exit(0);}
			raw[i*col + j] = (float)(atof(str));
		}
	
	}

	free(buf);
	fclose(fp);
	return ;
}
