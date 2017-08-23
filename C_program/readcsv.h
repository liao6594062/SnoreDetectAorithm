// 2017-03-26
// created by Yugao Li
// function: read raw from csv file
// type: header file

#ifndef READCSV_H
#define READCSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_col_of_raw(char *fname);
int get_row_of_raw(char *fname, int col);
void get_raw_from_csv(char *fname, float *raw, int row, int col);

#endif
